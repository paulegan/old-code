//--file: CSimpleXMLParser.cpp---------------------------------------
//
// The definition for a Simple XML Parser.
//
//
// NOTE: Your build environment must #define either __OS_WINNT__ 
//       or __OS_SOLARIS__ for this test file to work.
//
//
// Usage:
//
//   CSimpleXMLParser parser;
//   const char * pszDir = "C:\\user\\xml";
//   const char * pszfile = "input.xml";
//   CXMLNode * p = 0;
//
//   if ( true == parser.parse( pszDir, pszFile ) ) 
//   {
//		printf ( "Successful parse of .ini file [%s] in dir [%s]\r\n", pszFile, pszDir );
//
//		p = parser.copyRoot();
//
//		return true;
//	}
//	else
//	{
//		printf ( "Unsuccessful parse of .ini file [%s] in dir [%s]\r\n", pszFile, pszDir );
//
//		unsigned int cLen = parser.getErrorCount();
//		for ( unsigned int j=0; j<cLen; j++ )
//		{
//			printf( "\tXML Error: [%s]\r\n", parser.getError(j) );
//			CLog::log( pszErr );
//		}
//
//		return false;
//	} 
//

#include <fstream.h>
#include <string.h>
#include <stdio.h>
#include "CSimpleXMLParser.h"
#include "CXMLNode.h"


#ifdef __OS_WINNT__
	const char * g_pszFileDelim = "\\";
#else
	const char * g_pszFileDelim = "/";
#endif


unsigned int CSimpleXMLParser::s_cNodesAllocated = 0;
unsigned int CSimpleXMLParser::m_cLinenum = 0;

bool CSimpleXMLParser::m_bFoundBeginTag = false;
bool CSimpleXMLParser::m_bFatalError = false;



/** *****************************************************************
 *
 * CSimpleXMLParser::cleanUp
 *
 * Delete any memory currently in use for a node hierarchy or 
 * error messages.
 *
 **/
void CSimpleXMLParser::cleanUp()
{
	unsigned int i;

	CSimpleXMLParser::m_cLinenum = 0;
	CSimpleXMLParser::m_bFoundBeginTag = false;
	CSimpleXMLParser::m_bFatalError = false;

	if ( 0 != m_Root )
	{
		delete m_Root;
	}

	m_Root = 0;

	for ( i=0; i<g_cMaxErrors; i++ )
	{
		if ( 0 != m_pszErrors[i] ) 
		{
			delete [] m_pszErrors[i];
		}

		m_pszErrors[i] = 0;
	}

	m_cErrCount = 0;

}



/** *****************************************************************
 *
 * CSimpleXMLParser::moveRoot
 *
 * Accessor for the root node which moves the root node to the 
 * client that accessesd it such that the parser no longer knows 
 * about it and is therefore not responsible for deleting the memory 
 * associated with the instance hierarchy.
 *
 * Error information associated with the parse is also removed. 
 *
 ***/
CXMLNode * CSimpleXMLParser::moveRoot()
{
	CXMLNode * p = 0;

	p = m_Root;
	m_Root = 0;
	cleanUp();

	return (p);
}



/** *****************************************************************
 *
 * CSimpleXMLParser::parse
 *
 * Returns true on no error,  false on error.
 *
 * TODO: What happens when parse is called multiple times in a row?
 *
 **/
bool CSimpleXMLParser::parse( const char * pszDir, const char * pszFile )
{
	char szFilename[300] = {'\0'};
	char szNewLogFilename[300] = {'\0'};
	char szNewErrFilename[300] = {'\0'};
	unsigned int i;
	bool bErr = false;
	unsigned int cLen;


	if ( 0==pszFile || 0==pszDir )
		return false;

	/** Clean up previous memory associated with the parser */
	cleanUp();



	/** Generate new log filename */
	cLen = strlen(pszDir) + strlen(g_pszFileDelim) + strlen( pszFile ) + 1;

	if ( cLen < 290 )
	{
		sprintf( szFilename, "%s%s%s", pszDir, g_pszFileDelim, pszFile );
		sprintf( szNewLogFilename, "%s%s%s.log", pszDir, g_pszFileDelim, pszFile );
		sprintf( szNewErrFilename, "%s%s%s.err", pszDir, g_pszFileDelim, pszFile );
	}
	else
	{
		this->addError( "Input filename plus directory too long..." );
	}



	/** Open input and output file */
	fstream fin( szFilename, ios::in | ios::nocreate );

	#ifdef __OS_WINNT__
	 if ( fin.is_open() )
	#elif defined __OS_SOLARIS__
	 if ( fin )
	#endif
	{
		fstream fout( szNewLogFilename, ios::out );

		/** Parse the input file writing logs to output file */
		#ifdef __OS_WINNT__
		 if ( fout.is_open() )
		#elif defined __OS_SOLARIS__
		 if ( fout )
		#endif
		{
			fout << "Parse log: " << endl;
			recursiveParse( fin, fout, (&m_Root), 0 );

			fin.close();
			fout.close();


			/** Only open the err file if there is something to report */
			if ( m_cErrCount > 0 )
			{
				fstream ferrout( szNewErrFilename, ios::out );
		

				#ifdef __OS_WINNT__
				 if ( ferrout.is_open() )
				#elif defined __OS_SOLARIS__
				 if ( ferrout )
				#endif
				{
					for ( i=0; i<m_cErrCount; i++, bErr=true )
					{	 
						if ( 0==i ) 
						{ ferrout << endl << "Parsing errors:" << endl; }

						ferrout << m_pszErrors[i] << endl; 
					}
				ferrout.close();
				}
			}

			return (!bErr);

		}
		/** There was an error opening the output file */
		else
		{
			fin.close();

			fstream ferrout( szNewErrFilename, ios::out );
		

			#ifdef __OS_WINNT__
			 if ( ferrout.is_open() )
			#elif defined __OS_SOLARIS__
			 if ( ferrout )
			#endif
			{
				ferrout << "Error opening output file: [" << szNewLogFilename << "]" << endl << endl;
				ferrout.close();
			}	

			return false;
		}
	}
	else
	{
		/** There was an error opening the input file */
		fstream ferrout( szNewErrFilename, ios::out );
	

		#ifdef __OS_WINNT__
		 if ( ferrout.is_open() )
		#elif defined __OS_SOLARIS__
		 if ( ferrout )
		#endif
		{
			ferrout << "Error opening input file: [" << szFilename << "]" << endl << endl;
			ferrout.close();
		}	
	
		return false;
	}


}



/** *****************************************************************
 *
 * CSimpleXMLParser::recursiveParse
 *
 * Recursively parse the xml input file.  If a "Begin" tag is 
 * found,  allocate a new CXMLNode instance,  add it to the tree,  
 * and recursively continue to parse.  This means adding any name/value 
 * tags found to the current node as one of the node's properties.
 *
 * If an "End" tag is found,  back out of the recursion. 
 *
 * If a "Name/value" tag is found,  add this to the current CXMLNode.
 *
 * Returns true if no errors,  false if any.
 *
 ***/
bool CSimpleXMLParser::recursiveParse( fstream & fin, fstream & fout, PCXMLNode * root, int cLevel )
{
	char szBuffer[1025] = {'\0'};
	char szErr[1025] = {'\0'};
	char szOpenTag[256] = {'\0'};
	unsigned int cLen = 0;
	unsigned int cOpenTagLinenum = 0;
	PCXMLNode n = 0;

	
	/** A fatal error has occurred... */
	if ( true == CSimpleXMLParser::m_bFatalError )	
		return false;


	/** There is nothing left in the input file... */
	if ( !fin.getline( szBuffer, 1024 ) )
	{
		if ( 0 != strlen( szOpenTag ) )
		{
			sprintf( szErr, "Line %04ld: Begin tag [%s] is missing an End tag", cOpenTagLinenum, szOpenTag );
			this->addError( szErr );  
		}

		if ( false == CSimpleXMLParser::m_bFoundBeginTag )
		{
			sprintf( szErr, "Found no valid nodes." );
			this->addError( szErr );  
			return false;
		}

		return true;
	}


	CSimpleXMLParser::m_cLinenum++;

	
	while ( true )
	{
		this->trim( szBuffer );
		cLen = strlen( szBuffer );

		/** Tag is an "end tag" - the node is done being parsed */
		if ( true == this->isEndTag( szBuffer, cLen) )
		{	
			fout << "recursiveParse> " << cLevel << "  Found end tag...   [" << szBuffer << "]" << endl;
		
			sprintf( szErr, "%s", szBuffer+2 );
			szErr[ cLen-3] = '\0';

			if ( 0 != strcmp( szErr, (*root)->getName() ) )
			{
				sprintf( szBuffer, "%s", szErr );
				sprintf( szErr, "Line %04ld: End tag [%s] does not match it's begin tag [%s].", CSimpleXMLParser::m_cLinenum, szBuffer, (*root)->getName() );
				this->addError( szErr );  
			}
			else
			{
				memset( szOpenTag, '\0', 256 );
			} 	

			return true;
		}
		/** Tag is a begin tag - create a new child node */
		else
		if ( true == this->isBeginTag( szBuffer, cLen) )
		{	
			fout << "recursiveParse> " << cLevel << "  Found begin tag... [" << szBuffer << "]" << endl;
		
			cOpenTagLinenum = CSimpleXMLParser::m_cLinenum;
			CSimpleXMLParser::m_bFoundBeginTag = true;

			if ( 0 == cLevel )
			{
				(*root) = new CXMLNode( szBuffer, cLevel );
				CSimpleXMLParser::s_cNodesAllocated++;
				this->recursiveParse( fin, fout, root, (cLevel+1) );
			}
			else
			{
				sprintf( szOpenTag, "%s", szBuffer );
	
				n = new CXMLNode( szBuffer, cLevel );
				CSimpleXMLParser::s_cNodesAllocated++;
				n->m_Parent = (*root);
				
				/** If the node is overflow,  nuke it... */
				if ( false == (*root)->addChild( n ) )
				{
					sprintf( szErr, "Line %04ld: Overflow of children for node [%s], ", CSimpleXMLParser::m_cLinenum, (*root)->getName() );
					this->addError( szErr );
					delete n;
					this->recursiveParse( fin, fout, root, (cLevel+1) );
				}
				/** If the node was added,  it's the new root */
				else
				{
					this->recursiveParse( fin, fout, &n, (cLevel+1) );
				}
			}	

		}
		/** Tag is a value tag - add value to current node */
		else
		if ( true == this->isValueTag( szBuffer, cLen) )
		{

			if ( false == CSimpleXMLParser::m_bFoundBeginTag )
			{
				CSimpleXMLParser::m_bFatalError = true;
				sprintf( szErr, "Line %04ld: Fatal parsing error. Found value tag before Begin tag", CSimpleXMLParser::m_cLinenum );
				this->addError( szErr );
			}
			else
			{
				if ( *root==0 )
				{
					fout << "recursiveParse> " << cLevel << "  No current node to add property [" << szBuffer << "] to." << endl;			
					sprintf( szErr, "Line %04ld: No currently valid node to add property [%s] to.", CSimpleXMLParser::m_cLinenum, szBuffer );
					this->addError( szErr );
				}
				else
				{
					fout << "recursiveParse> " << cLevel << "  Adding Property... [" << szBuffer << "]" << endl;			
					
					if ( false == (*root)->addProperty( szBuffer ) )
					{
						sprintf( szErr, "Line %04ld: Overflow of properties for node [%s], ", CSimpleXMLParser::m_cLinenum, (*root)->getName() );
						this->addError( szErr );
					}
				}
			}
		}
		/** Tag is errant if it contains any characters other then whitespace */
		else 
		if ( cLen > 0 )
		{
				sprintf( szErr, "Line %04ld: Discarding invalid tag: [%s],", CSimpleXMLParser::m_cLinenum, szBuffer );
				this->addError( szErr );

				fout << "recursiveParse> Discarding invalid tag: [" << szBuffer << "]" << endl;
		}
	


		/** A fatal error has occurred... */
		if ( true == CSimpleXMLParser::m_bFatalError )
			return false;


		/** Retrieve the next line */
		memset( szBuffer, '\0', 1025 );
		if ( !fin.getline( szBuffer, 1024 ) )
		{

			if ( 0 != strlen( szOpenTag ) )
			{
				sprintf( szErr, "Line %04ld: Begin tag [%s] is missing an End tag", cOpenTagLinenum, szOpenTag );
				this->addError( szErr );  
			}

			if ( false == CSimpleXMLParser::m_bFoundBeginTag )
			{
				sprintf( szErr, "Found no valid nodes." );
				this->addError( szErr );  
				return false;
			}
			
			return true;
		}

		CSimpleXMLParser::m_cLinenum++;
	}
	

	return true;
}



/** *****************************************************************
 *
 * CSimpleXMLParser::CSimpleXMLParser
 *
 **/
CSimpleXMLParser::CSimpleXMLParser()
{
	m_Root = 0;
	m_cErrCount = 0;

	for ( unsigned int i=0; i<g_cMaxErrors; i++ )
	{
		m_pszErrors[i] = 0;
	}

	CSimpleXMLParser::m_cLinenum = 0;
	CSimpleXMLParser::m_bFoundBeginTag = false;
	CSimpleXMLParser::m_bFatalError = false;

	return;
}



/** *****************************************************************
 *
 * CSimpleXMLParser::~CSimpleXMLParser
 *
 **/
CSimpleXMLParser::~CSimpleXMLParser()
{
	cleanUp();
	return;
}




/** *****************************************************************
 *
 * CSimpleXMLParser::addError
 *
 * Add an error to the buffer of errors.
 *
 **/
bool CSimpleXMLParser::addError( const char * pszErr )
{
	if ( m_cErrCount+1 >= g_cMaxErrors ) return false;
	if ( 0==pszErr ) return false;

	char * psz = 0;
	int cLen = strlen( pszErr ) + 1;

	psz = new char[cLen];

	if ( 0 == psz ) return false;

	memset( psz, '\0', cLen );

	sprintf( psz, "%s", pszErr );

	m_pszErrors[m_cErrCount++] = psz;
	
	return true;
}




/** *****************************************************************
 *
 * CSimpleXMLParser::trim
 *
 * Left/right whitepace trim [ '\n', '\r', '\t', ' ' ]
 *
 **/
bool CSimpleXMLParser::trim( char * psz ) const
{
	static const char szWhite[6] = { '\n', '\r', '\t', ' ', (char)160, '\0' };	

	if ( 0==psz ) return false;
	int cLen = strlen( psz );
	int i, cCount;

	/** Count left */
	for ( i=0, cCount=0; i<cLen; i++, cCount++ )
		{ if ( NULL == strchr( szWhite, psz[i] ) ) break; }

	/** Trim left */
	for ( i=0; (cCount>0) && (i<cLen-cCount); i++ )
		{ psz[i] = psz[i+cCount]; }

	psz[ cLen - cCount ] ='\0';
	
	cLen = strlen( psz );
	
	/** Count right */
	for ( i=cLen-1, cCount=0; i>=0; i--, cCount++ )
		{ if ( NULL == strchr( szWhite, psz[i] ) ) break; }

	/** Trim right */
	for ( i=cLen-1; (cCount>0) && (i>=cLen-cCount); i-- )
		{ psz[i] = '\0'; }

	return true;
}



 
/** *****************************************************************
 *
 * CSimpleXMLParser::isBeginTag
 *
 * Begins with '<',  ends with '>',  does not begin with '</'.
 *
 **/
bool CSimpleXMLParser::isBeginTag( const char * psz, unsigned int cSize ) const
{
	if ( 0 == psz ) return false;
	if ( cSize < 3 ) return false;	/* <a> */

	if ( (psz[0]!='<') || (psz[1]=='/') ) return false;
	
	char * pch = strchr( psz, '>' );

	if ( 0 == pch ) return false;

	if ( strlen(pch) > 1 ) return false;
	
	return true;
}



/** *****************************************************************
 *
 * CSimpleXMLParser::isEndTag
 *
 * Begins with '</',  ends with '>'.
 *
 **/
bool CSimpleXMLParser::isEndTag( const char * psz, unsigned int cSize ) const
{
	if ( 0 == psz ) return false;
	if ( cSize < 3 ) return false;	/* </a> */

	if ( (psz[0]!='<') || (psz[1]!='/') ) return false;
	
	if ( psz[cSize-1] != '>' ) return false;
	
	return true;
}




/** *****************************************************************
 *
 * CSimpleXMLParser::isValueTag
 *
 * Begins with '<NAME>', ends with </NAME>,  has a value in the middle. 
 *
 * eg:  <FirstName>Fred</FirstName>
 *
 **/
bool CSimpleXMLParser::isValueTag( const char * psz, unsigned int cSize ) const
{
	static const char * pszDelim = "<>";
	char * pszBegin   = 0;
	char * pszMiddle  = 0;
	char * pszEnd     = 0;
	char * p          = 0;

	if ( 0==psz ) return false;
	int cLen = cSize + 1;

	p = new char[ cLen ];

	memset(  p, '\0', cLen );
	sprintf( p, "%s", psz );

	pszBegin  = strtok( p, pszDelim );	
	pszMiddle = strtok( 0, pszDelim );	
	pszEnd    = strtok( 0, pszDelim );	

	if ( 0==pszBegin || 0==pszMiddle || 0==pszEnd ) 
	{
		if ( 0 != p ) { delete [] p; }
		return false;
	}

	if ( strlen(pszEnd) < 2 )
	{
		if ( 0 != p ) { delete [] p; }
		return false;
	}

	if ( pszEnd[0] != '/' )
	{
		if ( 0 != p ) { delete [] p; }
		return false;
	}

	if ( 0 != strcmp( pszBegin, pszEnd+1 ) ) 
	{
		if ( 0 != p ) { delete [] p; }
		return false;
	}

	if ( 0 != p ) { delete [] p; }
	return true;
}
