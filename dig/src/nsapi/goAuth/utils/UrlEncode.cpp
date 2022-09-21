//--file: UrlEncode.cpp----------------------------------------------
//



/****************************************************
 * URLEncode.cpp
 *
 * Provides static functions to encode and decode ascii strings
 * into a UTF-8 format. 
 *
 * Declared in SWUtil.h
 *
 * @version 1.x
 * $Revision: 2 $
 * $Modtime: 6/29/99 2:50p $
 * @author John Mollman
 *
 *****************************************************/
#include <string.h>
#include <stdio.h>
#include <ctype.h>


// If it isn't in this array, encode it
// we're being conservative here. 
// NOTE: We encode '+'; and assume everyone else does too.
// When decoding we change all '+' to ' '.  
static const char SafeCharSet[] = 
	{'$', '-', '_', '.', '!', '*', '\'', '(', ')', ',', '\"', '%',
	 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', 'A', 'B',
	 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '1', '2', '3', '4', 
	 '5', '6', '7', '8', '9', '0' };
	


/** *****************************************************************
 *
 * x2c
 *
 * Given a two-digit hex value, return the ascii character of 
 * that value
 *
 ***/
char x2c(char* pszOctet) 
{
    char digit;

    digit = ((pszOctet[0] >= 'A') ? 
		((pszOctet[0] & 0xdf) - 'A')+10 : (pszOctet[0] - '0'));

    digit *= 16;

    digit += (pszOctet[1] >= 'A' ? 
		((pszOctet[1] & 0xdf) - 'A')+10 : (pszOctet[1] - '0'));

    return(digit);
}
			



/** *****************************************************************
 *
 * URLEncodeString
 *
 * URL encode source string and copy into destination buffer.
 *
 * NOTE: Destination buffer needs to be 33% larger that source 
 * to accomodate growth of encoded characters!
 *
 * Returns length of destination buffer when encoding is complete, or
 * -1 if the destination buffer provided is too small (or other error). 
 *
 **/
int URLEncodeString( const char* pszSrc, char* pszDest, unsigned int iDestLen )
{
	// Caller needs to provide a destination buffer that's 33% larger than 
	// the buffer to be encoded (max growth of string is 33% if all characters 
	// are encoded)

  if( 0==pszSrc || 0==pszDest) {
	return -1;
  }

  //printf("URLEncodeString -> encoding %s\n", pszSrc);
	

  unsigned int iSrcLen = strlen(pszSrc);
  unsigned int idx = 0;
  unsigned int iLen = 0;
  unsigned int iRetLen = 0;
  char szEncoded[4] = {'\0'};

	
  
  // We're allowing truncation and returning -1 to indicate it happened 
  /*
  if( iDestLen < (iSrcLen * 1.33) ) {
	  printf("URLEncodeString -> destination too small, failing\n");
	  return -1;
  }
  */
  
  memset( pszDest, '\0', iDestLen );


	
  //printf("URLEncode -> starting scan\n");
  while((iRetLen < iDestLen) && (idx < iSrcLen)) {
	// index of next char not in SafeCharSet 
	iLen = strspn(&pszSrc[idx], SafeCharSet);


	if(iLen > 0) {
	  // strcat all the safe chars
	  strncat(pszDest, &pszSrc[idx], iLen);
	  iRetLen += iLen;
	}
	
	// if you didn't find null terminator, encode and strcat offending char
	if( '\0' != pszSrc[iLen+idx]) {

	  // hex encode the offending char at idx 
	  sprintf(szEncoded, "%%%02x", pszSrc[iLen+idx]);
		
	  // add it to destination string 
	  // Magic Number: always 3 chars encoded 
	  strncat(pszDest, szEncoded, 3);	
	  iRetLen += 3;
	  idx += (iLen + 1);

	} else {
	  idx += iLen;
	}
  }

  //printf("URLEncode finished, %s encoded as %s\n", pszSrc, pszDest);
  return strlen(pszDest);


  // If the dest buffer was big enough return new length
  if (idx >= iSrcLen) {
	return iRetLen;
  } else {
	// return failure code (though we encoded all we had room for)
	return -1;
  }
}



/** *****************************************************************
 *
 * URLDecodeString
 *
 * Unescapes URL encoded characters in the passed-in string.
 * Does replacements in-situ, since there's no chance the string 
 * will grow.  We replace all '+' chars with ' ', which assumes all 
 * encoded strings we get encode '+'. 
 *
 * Returns the length of the decoded string
 *
 **/
int URLDecodeString( char * pszSrc )
{
	unsigned int iLen = strlen(pszSrc);
	unsigned int x=0, y=0;

	/** x is index of Destination  */
	/** y is index of Source       */
	for ( x=0, y=0; pszSrc[y]; ++x, ++y ) 
	{
		if( pszSrc[y] == '+') 
		{
			/** Convert all '+' to spaces             */
			/** (assumes that '+' has been encoded!)  */
			pszSrc[x] = ' ';
		}
        else if((pszSrc[x] = pszSrc[y]) == '%') 
		{
            pszSrc[x] = x2c(&pszSrc[y+1]);
            y+=2;
        }
	}

    pszSrc[x] = '\0';
    return x;
}




/** *****************************************************************
 *
 * verifyFormat
 *
 * Convert all backslashes '\' to forward slashes '/'.
 *
 * Remove multiple instances of forward slashes...  
 *   (eg:  'suck.com///index.html' becomes 'suck.com/index.html')
 *
 * Perform this operation in-situ.
 *
 **/
int URLVerifyFormat( char * psz )
{
	const char * pszHttp = "http";
	char szTemp[5] = {'\0'};
	unsigned int i=0, j=0;
	unsigned int cLen;

	if ( 0 == psz )
		return -1;

	cLen = strlen( psz );

	/** Convert backslashes to slashes */
	for ( unsigned int m=0; m<cLen; m++ )
	{
		if ( psz[m]=='\\' ) 
			psz[m] = '/';
	}

	/**
	 *
	 * Look for multiple forward slashes.  Remove instances of 
	 * more than 1 forward slash coupled together: 
	 *
	 * (eg: "///story////index.html" becomes "/story/index.html")
	 *
	 * Watch out for: http://www.fubar.com.  The 2 forward 
	 * slashes near the beginning are ok.
	 *
	 **/

   // See if we need to skip 'http://' or 'https://'

	for (i = 0; i < 4; i++)
	{
		szTemp[i] = tolower(psz[i]);
	}

	if (strcmp(szTemp, pszHttp) == 0)
    {
       // Go to the "//" after "http" or "https"

       for (i = 0; i < cLen; i++)
       {
          if (psz[i] == '/')
          {
             // Advance to immediately after the two forward slashes

             i += 2;
             break;
          }
       }

       // Continue advancing to the next slash to skip the domain name or
       // IP address and port number

       for (; psz[i] != '/'; i++);

       // Update the length

       cLen -= i;

       // Now, skip the entire "http://www.fubar.com"

       for (j = 0; j < cLen; i++, j++)
       {
          psz[j] = psz[i];
       }

       psz[j] = '\0';
    }

	/** Early out */
    if (strstr(psz, "//" ) == 0)
		return cLen;


	/** Collapse multiple forward slashes... */
	for (i = 0; i<cLen; i++)
	{
		if ( psz[i] == '/' )
		{
			while (psz[ (i+1) ] == '/' )
			{
				for ( j=i; j<(cLen); j++ )
					psz[j] = psz[j+1];

				cLen -= 1;
				psz[j] = '\0';
			}
		}
	}

	
	return cLen;
}

