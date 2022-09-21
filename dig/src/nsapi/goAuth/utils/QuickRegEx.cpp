//--file: QuickRegEx.cpp---------------------------------------------
//
#include "QuickRegEx.h"
#include <string.h>


#ifdef __OS_WINNT__
	#define CASEINSENSITIVECMP stricmp
#elif defined __OS_SOLARIS__
	#define CASEINSENSITIVECMP strcasecmp
#endif


//#define __OS_SOLARIS__




/////////////////////////////////////////////////////////////////////
//
// matchQuickRegEx
//
// Do a directory match using patterns like: 
//    
//    /premium/*/movies/*.html 
// and
//    /premium/*/movies/*
//
// The 1st one will match HTML requests that are in a directory 
// structure which matches /premium/*/movies/
//
// The 2nd one will match any request that begins with a match 
// to the regular expression /premium/*/movies/...  and ends 
// in anything.
//
// Return "true" if the string matches the pattern,  false if not.
//
//
// For added functionality,  regular expression matching of patterns 
// containing '?' characters is in here... it's just really slow 
// if you're doing a lot of comparisons (it's does a char-by-char 
// comparison instead of a strcmp)
//
// Regular expressions like:
//       /premium/*/*/abc???ghi.h???
//
// will match string like:
//
//      /premium/stuff/articles/abcdefghi.html
//
bool matchQuickRegEx( const char * lpszString,const char * lpszPattern ) 
{

	char* szCurStringPtr  =   (char*)   lpszString;
	char* szCurPatternPtr =   (char*)   lpszPattern;

	#ifdef _DEBUG
	  char*	szStringEnd  = szCurStringPtr  + strlen ( szCurStringPtr ) + 1;
	  char*	szPatternEnd = szCurPatternPtr + strlen ( szCurPatternPtr ) + 1;
	#endif _DEBUG

	char* szStringWalk  = 0;
	char* szPatternWalk = 0;
	char* szStringHtml  = 0;
	char* szPatternHtml = 0;
	bool  bReturnCode   = true;      
	bool  bHit0         = false;

	unsigned int cStrLen=0, cPatLen=0, i=0;


    /** If there are no wild cards just return a stricmp */
	if( 0 == strchr( lpszPattern, '*' ) )  
	{
		/** toml hack 1: begin.  look for pattern matching on '?'... */
		if ( 0 == strchr( lpszPattern, '?' ) )
		{
			/** There are no '*' and no '?' characters */
			  if ( 0 == CASEINSENSITIVECMP(lpszPattern, lpszString) )
			    return true;
			  else		
			    return false;
		}
		else
		{
			/** There is a '?' but no '*' character */
			cStrLen = strlen( szCurStringPtr );
			cPatLen = strlen( szCurPatternPtr );

			if ( cStrLen != cPatLen ) 
				return false;

			for ( i=0; i<cStrLen; i++ )
			{
				if ( ( szCurStringPtr[i] != szCurPatternPtr[i] ) && ( szCurPatternPtr[i] != '?' ) )
				{
					return false;
				}
			}

			return true;
		}
		/** toml hack 1: end */
	}



	/**
	  * We'll treat *.html different than a single level wildcard 
	  * so find the last instance of '/' with a reverse strchr
	  * and swap in a '\0'
	  *
	  */
	szCurStringPtr    =   strchr ( szCurStringPtr,  '/' );  
	szCurPatternPtr   =   strchr ( szCurPatternPtr, '/' );
	bReturnCode       =   true;      
  
	if ( ( szCurStringPtr == 0 ) || ( szCurPatternPtr == 0 ) ) 
	{
		return false;
	}


    /** Continute to loop until we hit a dir !strcmp or '\0' */
	while( bReturnCode ) 
	{

		#ifdef _DEBUG
		  //assert ( szStringWalk <= szStringEnd );
		  //assert ( szPatternWalk <= szPatternEnd );
		#endif _DEBUG
	
		if ( (szCurStringPtr == 0) || (szCurPatternPtr == 0) ) 
		{
			break;
		}

		szStringWalk    =   strchr (szCurStringPtr,   '/' );
		szPatternWalk   =   strchr (szCurPatternPtr,  '/' );

		/**
		  * If we hit 0 we need to do a full compare on the strings.  
		  * (i.e. don't reset the '/' to a '\0' for the next compare.  
		  * In the case where both are 0 check for the *. pattern.  
		  * If they are both 0, we check for "*.", if it exists, 
		  * stricmp the .* after the wildcard.  If that "*." does 
		  * not exist, or the strings are xor (0) return a stricmp 
		  * for the difference.  It will catch both 
		  *     "/test/blah/*.html" == "/test/index.html" 
		  * as false and 
		  *     "/test/index.html" == "/test/index2.html" 
		  * as false.
          *
		  **/
		if ( (0!=szStringWalk) && (0!=szPatternWalk) ) 
		{
			*szStringWalk       = '\0';
			*szPatternWalk      = '\0';
		}
		else 
		if ( (0==szStringWalk) && (0==szPatternWalk) && (0!= strstr( szCurPatternPtr, "*." )) )  
		{
	
			szStringHtml  = strchr( lpszString, '.');
			szPatternHtml = strchr( lpszPattern, '.');

			/**
			  * Check to make sure a period exists in both.  If not return
			  * Then check if the string equates to ".*"  If it does return
			  * true.  Last check String against the Pattern.
			  *
			  **/
			if ( (0==szStringHtml) || (0==szPatternHtml) ) 
			{
				return false;
			}
			else 
			if ( 0 == strcmp ( szPatternHtml, ".*" ) ) 
			{  
				bReturnCode = true;
			}
			else 
			{
				/** This is what causes a ".jpeg" request to not match a ".html" pattern */
				  if ( 0 == CASEINSENSITIVECMP( szStringHtml, szPatternHtml ) )
				    { bReturnCode = true; }
				  else
			    {  bReturnCode = false; }
			}

			break;
		}
		/** toml hack 2: begin.  if pattern is a "*",  it all matches...*/
		else if ( 0 == strcmp( szCurPatternPtr, "*" ) )
		{
			if ( 0 != szStringWalk  ) { *szStringWalk   =   '/'; }
			if ( 0 != szPatternWalk ) { *szPatternWalk  =   '/'; }
			return true;
		}
		/** toml hack 2: end */

		/** toml hack 3: begin:  if pattern contains '?',  test individual characters */
		else if ( 0 != strchr( szCurPatternPtr, '?' ) )
		{
			cStrLen = strlen( szCurStringPtr );
			cPatLen = strlen( szCurPatternPtr );

			if ( cStrLen != cPatLen ) 
			{
				bReturnCode = false;
				break;
			}

			bReturnCode = true;
			
			for ( i=0; i<cStrLen; i++ )
			{
				if ( ( szCurStringPtr[i] != szCurPatternPtr[i] ) && ( szCurPatternPtr[i] != '?' ) )
				{
					bReturnCode = false;
					break;
				}
			}

			break;
		}
		/** toml hack 3: end */
		else  
		{
			if ( 0 == CASEINSENSITIVECMP( szCurStringPtr, szCurPatternPtr ) )
				{ bReturnCode = true; }
			else
				{ bReturnCode = false; }

			break;
		}


		/** *********************************************************
		  *
		  * If a wildcard is found, it always matches true, so 
		  * loop with bReturnCode as true.  Also check for 
		  * *.a which can appear in a dir.
		  *
		  **/
		if ( 0 == strcmp ( szCurPatternPtr, "*" ) ) 
		{
			bReturnCode = true;
		}
		/** toml hack 4: begin - match on question marks inside a directory... */
		else if ( 0 != strchr( szCurPatternPtr, '?' ) )
		{
			cStrLen = strlen( szCurStringPtr );
			cPatLen = strlen( szCurPatternPtr );

			if ( cStrLen != cPatLen ) 
			{
				bReturnCode = false;
			}
			else
			{
				bReturnCode = true;
				
				for ( i=0; i<cStrLen; i++ )
				{
					if ( ( szCurStringPtr[i] != szCurPatternPtr[i] ) && ( szCurPatternPtr[i] != '?' ) )
					{
						bReturnCode = false;
						continue;
					}
				}
			}
		}
		/** toml hack 4: end */
		else if ( strstr ( szCurPatternPtr, "*." )  != 0 ) 
	    {
			szStringHtml  =   strchr (lpszString, '.');
			szPatternHtml =   strchr (lpszPattern, '.');

			if ( (0==szStringHtml) || (0==szPatternHtml) ) 
			{ 
				return false; 
			}

			if ( 0==CASEINSENSITIVECMP( szStringHtml, szPatternHtml ) )
			{ bReturnCode = true; }
			else
			{ bReturnCode = false; }

		}
		else 
		{
			if ( 0 == CASEINSENSITIVECMP( szCurStringPtr, szCurPatternPtr ) )
			{ bReturnCode = true; }
			else
			{ bReturnCode = false; }
			
		}
   
		// Strtok destroys the string to tokenize.  We chose to tok our own strings
		// for speed, so replace the original '\' (now a '\0') with '\'

		*szStringWalk   =   '/';
		*szPatternWalk  =   '/';

		//increment for the next token.
		szCurStringPtr  = ++  szStringWalk;
		szCurPatternPtr = ++  szPatternWalk;
	}


	return bReturnCode;
}



/** Testing code */


//void testReg( char * pszStr, char * pszExp )
//{
//	bool b = matchQuickRegEx( pszStr, pszExp );
//	printf ("Str: [%34s]     Expr: [%26s]     Pattern match? [%5s]\r\n", pszStr, pszExp, (b==true ? "true" : "false") );
//	return;
//}


//
//	testReg( "/premium/stuff/index.html", "/premium/*" );
//	testReg( "/premium/stuff/",           "/premium/*" );
//	testReg( "/premium/stuff",            "/premium/*" );
//	testReg( "/premium/index.html",       "/premium/*" );
//	testReg( "/premium/",                 "/premium/*" );
//	testReg( "/premium",                  "/premium/*" );
//	printf ("\r\n");
//
//  testReg( "/premium/stuff/articles/index.html",   "/premium/*/articles/*.html" );
//	testReg( "/premium/stuff/articles/",             "/premium/*/articles/*.html" );
//	testReg( "/premium/stuff/articles",              "/premium/*/articles/*.html" );
//	testReg( "/premium/stuff/index.html",            "/premium/*/articles/*.html" );
//	testReg( "/premium/stuff/",                      "/premium/*/articles/*.html" );
//	testReg( "/premium/stuff",                       "/premium/*/articles/*.html" );
//	testReg( "/premium/index.html",                  "/premium/*/articles/*.html" );
//	testReg( "/premium/",                            "/premium/*/articles/*.html" );
//	testReg( "/premium",                             "/premium/*/articles/*.html" );
//	printf ("\r\n");
//
//	testReg( "/premium/stuff/fubar/index.html",    "/premium/*/*.html" );
//	testReg( "/premium/stuff/fubar/",              "/premium/*/*.html" );
//	testReg( "/premium/stuff/fubar",               "/premium/*/*.html" );
//	testReg( "/premium/stuff/index.html",          "/premium/*/*.html" );
//	testReg( "/premium/stuff/",                    "/premium/*/*.html" );
//	testReg( "/premium/stuff",                     "/premium/*/*.html" );
//	testReg( "/premium/index.html",                "/premium/*/*.html" );
//	testReg( "/premium/",                          "/premium/*/*.html" );
//	testReg( "/premium",                           "/premium/*/*.html" );
//	printf ("\r\n");
//
//	testReg( "/premium/stuff/index.html", "/premium/*.html" );
//	testReg( "/premium/stuff/",           "/premium/*.html" );
//	testReg( "/premium/stuff",            "/premium/*.html" );
//	testReg( "/premium/index.html",       "/premium/*.html" );
//	testReg( "/premium/",                 "/premium/*.html" );
//	testReg( "/premium",                  "/premium/*.html" );
//	printf ("\r\n");
//
//	testReg( "/premium/index.html",   "/premium/*.html" );
//	testReg( "/premium/index.jpeg",   "/premium/*.html" );
//	printf ("\r\n");
//
//	testReg( "/premium/index.html",   "/premium/*.*" );
//	testReg( "/premium/index.jpeg",   "/premium/*.*" );
//	testReg( "/premium/index.jpeg",   "/premium/*.*" );
//	printf ("\r\n");
//
//	testReg( "/premium/stuff/index.html",   "/premium/*/*.*" );
//	testReg( "/premium/stuff/index.jpeg",   "/premium/*/*.*" );
//	testReg( "/premium/index.jpeg",         "/premium/*/*.*" );
//	printf ("\r\n");
//	
//	testReg( "/premium/index.html",         "/premium/index.html" );
//	testReg( "/premium/",                   "/premium/index.html" );
//	printf ("\r\n");
//
//	testReg( "/premium/abcdefghi/index.html",      "/premium/abc???ghi/*.html" );
//	testReg( "/premium/abcdefghj/index.html",      "/premium/abc???ghi/*.html" );
//	testReg( "/premium/abcdefgh.html",             "/premium/abc???gh.html" );
//	testReg( "/premium/abcabcgh.html",             "/premium/abc???gh.html" );
//	testReg( "/premium/abcaaaaa.html",             "/premium/abc???gh.html" );
//
//	testReg( "/premium/stuff/abcdefgh.html",       "/premium/*/abc???gh.h???" );
//	testReg( "/premium/stuff/abcdefgh.hhhh",       "/premium/*/abc???gh.h???" );
//	printf ("\r\n");
//

