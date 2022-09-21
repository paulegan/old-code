//--file: CGreen.cpp-----------------------------------------
//
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "CGreen.h"
#include "GreenUtils.h"



/** *****************************************************************
 *
 * CGreen::CGreen( const CGreen & copy )
 *
 * Copy constructor
 *
 **/
CGreen::CGreen( const CGreen & copy )
{
 	unsigned int cLen = 0;
	unsigned int i = 0;

	this->init();

	this->m_ulWarnings     = copy.m_ulWarnings;
	this->m_ulErrors       = copy.m_ulErrors;
	this->m_cCryptKeyIndex = copy.m_cCryptKeyIndex;


	/** Get name/pair value data */
	for ( i=0; i<g_cMaxKnownNameValuePairs; i++ )
	{
		/** Do a direct copy... */
		if ( 0 != copy.m_pszValues[i] )
		{
			this->m_pszValues[i] = new char[ copy.m_cValueSize[i] + 1 ];
	
			if ( 0 != this->m_pszValues[i] )
			{
				memset( this->m_pszValues[i], '\0', (copy.m_cValueSize[i] + 1) );
				
				// TODO: Is there a bug here with using "sprintf"?
				/*
				sprintf( this->m_pszValues[i], "%s", copy.m_pszValues[i] );
				*/
				strncpy(this->m_pszValues[i], copy.m_pszValues[i], copy.m_cValueSize[i] + 1);
				this->m_cValueSize[i] = copy.m_cValueSize[i];	
			}
		}
	}
	
	return;
}




/** *****************************************************************
 *
 * CGreen::CGreen
 *
 * Constructs an empty Green, to be used by services 
 * that need to SET the GREEN. 
 *
 **/
CGreen::CGreen()
{
	this->init();
	return;
} 


		
/** *****************************************************************
 *
 * CGreen::~CGreen
 *
 **/
CGreen::~CGreen()
{
	unsigned int i;
	
	for ( i=0; i<g_cMaxKnownNameValuePairs; i++ )
	{
		if ( 0 != m_pszValues[i] )
		{
			delete m_pszValues[i];
		}
	}

	return;
} 



/** *****************************************************************
 *
 * CGreen::CGreen
 *
 * Initializes the Green with the GREEN string from the 
 * actual browser cookie, to be used by services that need to GET 
 * the GREEN.
 *
 * Takes data of any the following forms:
 *
 *      SWID={213452563567245214}; GREEN=d,34FD14...3443; app=ESPN 
 *      GREEN=d,34FD14...3443; SWID={213452563567245214}; app=ESPN 
 *      GREEN=d,34FD14...3443;
 *      d,34FD14...3443
 * 
 **/
CGreen::CGreen( char * pszCookie )
{
	char * pszToken = 0;
	char * pszEnd = 0;

	
	this->init();


	/** Errant data */
	if ( 0==pszCookie ) 
	{
		m_ulErrors |= g_cErrorMissingOrNullCookieData;
		return;
	
	}
	
	
	/** It's the 2nd, 3rd, or later cookie... */
	pszToken = strstr( pszCookie, " GREEN=" );
	
	if ( 0 != pszToken )
	{
		pszEnd = strchr( pszToken, ';' );

		if ( 0 != pszEnd )
		{
			*pszEnd = '\0';
		}

		pszToken += 7;
		
		this->parseCookieData( pszToken );

		if ( 0 != pszEnd )
		{
			*pszEnd = ';';
		}

		return;
	}


	/** It's the 1st cookie... */
	pszToken = strstr( pszCookie, "GREEN=" );
	
	if ( 0!=pszToken && pszToken==pszCookie )
	{
		pszEnd = strchr( pszToken, ';' );

		if ( 0 != pszEnd )
		{
			*pszEnd = '\0';
		}

		pszToken += 6;

		this->parseCookieData( pszToken );

		if ( 0 != pszEnd )
		{
			*pszEnd = ';';
		}

		return;
	}



	/** It's the raw data */
	this->parseCookieData( pszCookie );

	return;
}



/** *****************************************************************
 *
 * CGreen::init
 *
 * Initialize data members common between all constructors.
 *
 **/
void CGreen::init()
{
	unsigned int i;
	
	for ( i=0; i<g_cMaxKnownNameValuePairs; i++ )
	{
		memset( m_szNames[i], '\0', 3 );
		m_pszValues[i] = 0;
	}
	
	m_ulWarnings = 0;
	m_ulErrors = 0;

	m_cCryptKeyIndex = -1;

	/*
	sprintf( m_szNames[0],  "ID" );	m_ulMask[0]  = 1;
	sprintf( m_szNames[1],  "LG" );	m_ulMask[1]  = 2;
	sprintf( m_szNames[2],  "SN" );	m_ulMask[2]  = 4;
	sprintf( m_szNames[3],  "TM" );	m_ulMask[3]  = 8;
	sprintf( m_szNames[4],  "UN" );	m_ulMask[4]  = 16;
	sprintf( m_szNames[5],  "EM" );	m_ulMask[5]  = 32;
	sprintf( m_szNames[6],  "SD" );	m_ulMask[6]  = 64;
	sprintf( m_szNames[7],  "DA" );	m_ulMask[7]  = 128;
	sprintf( m_szNames[8],  "TE" );	m_ulMask[8]  = 256;
	sprintf( m_szNames[9],  "DL" );	m_ulMask[9]  = 512;
	sprintf( m_szNames[10], "LM" );	m_ulMask[10] = 1024;
	sprintf( m_szNames[11], "FN" );	m_ulMask[11] = 2048;
	sprintf( m_szNames[12], "LN" );	m_ulMask[12] = 4096;
	sprintf( m_szNames[13], "DM" );	m_ulMask[13] = 8192;
	sprintf( m_szNames[14], "OU" );	m_ulMask[14] = 16384;
	sprintf( m_szNames[15], "OA" );	m_ulMask[15] = 32768;
	sprintf( m_szNames[16], "BD" );	m_ulMask[16] = 65536;
	sprintf( m_szNames[17], "GC" );	m_ulMask[17] = 131072;
	sprintf( m_szNames[18], "GE" );	m_ulMask[18] = 262144;
	sprintf( m_szNames[19], "FM" );	m_ulMask[19] = 524288;
	*/

	strncpy( m_szNames[0],  "ID", 3 );	m_ulMask[0]  = 1;
	strncpy( m_szNames[1],  "LG", 3 );	m_ulMask[1]  = 2;
	strncpy( m_szNames[2],  "SN", 3 );	m_ulMask[2]  = 4;
	strncpy( m_szNames[3],  "TM", 3 );	m_ulMask[3]  = 8;
	strncpy( m_szNames[4],  "UN", 3 );	m_ulMask[4]  = 16;
	strncpy( m_szNames[5],  "EM", 3 );	m_ulMask[5]  = 32;
	strncpy( m_szNames[6],  "SD", 3 );	m_ulMask[6]  = 64;
	strncpy( m_szNames[7],  "DA", 3 );	m_ulMask[7]  = 128;
	strncpy( m_szNames[8],  "TE", 3 );	m_ulMask[8]  = 256;
	strncpy( m_szNames[9],  "DL", 3 );	m_ulMask[9]  = 512;
	strncpy( m_szNames[10], "LM", 3 );	m_ulMask[10] = 1024;
	strncpy( m_szNames[11], "FN", 3 );	m_ulMask[11] = 2048;
	strncpy( m_szNames[12], "LN", 3 );	m_ulMask[12] = 4096;
	strncpy( m_szNames[13], "DM", 3 );	m_ulMask[13] = 8192;
	strncpy( m_szNames[14], "OU", 3 );	m_ulMask[14] = 16384;
	strncpy( m_szNames[15], "OA", 3 );	m_ulMask[15] = 32768;
	strncpy( m_szNames[16], "BD", 3 );	m_ulMask[16] = 65536;
	strncpy( m_szNames[17], "GC", 3 );	m_ulMask[17] = 131072;
	strncpy( m_szNames[18], "GE", 3 );	m_ulMask[18] = 262144;
	strncpy( m_szNames[19], "FM", 3 );	m_ulMask[19] = 524288;

	return;
}


/** *****************************************************************
 *
 * CGreen::parseCookieData
 *
 * Parse out the cookie data, eg:
 *
 *      d,34FD14FD356748BBAC...etc...34FBBC56443
 * 
 **/
bool CGreen::parseCookieData( char * pszData )
{	
	char * pszFrontToken = 0;
	char * pszBackToken  = 0;
	char * pszNullToken  = 0;
	char szFront[10]  = {'\0'};
	char szBack[1025] = {'\0'};
	char szData[513]  = {'\0'};
	unsigned int i = 0;
	unsigned int cLen = 0;
	unsigned long cCryptKeyIndex = 0;
	char szCryptKeyIndex[2] = {'\0'};
	bool bStatus = true;
	unsigned char * pszDecrypt = 0;
	 

	/** Bad input data */
	if ( 0==pszData ) 
	{
		m_ulErrors |= g_cErrorMissingOrNullCookieData;
		return false;
	}

	pszFrontToken = strtok( pszData, "," );
	pszBackToken = strtok( 0, "," );
	pszNullToken = strtok( 0, "," );


   	/** Bad GREEN data format - i need 2 tokens here */
	if ( 0==pszFrontToken || 0==pszBackToken || 0!=pszNullToken )
	{
		m_ulErrors |= g_cErrorInvalidCookieDataFormat;
		return false;
	}

	
	/** Need between 1 and 8 characters here - get a copy of them */
	cLen = strlen( pszFrontToken );
	
	if ( (cLen<1) || (cLen>8) ) 
	{
		m_ulErrors |= g_cErrorInvalidCookieDataFormat;
		return false;
	}

	/*
	sprintf( szFront, "%s", pszFrontToken );
	*/

	memset(szFront, '\0', 10);
	strncpy(szFront, pszFrontToken, cLen + 1);


	/** Need between 1 and 1024 characters here. - get a copy of them  */
	cLen = strlen( pszBackToken );
	
	if ( (cLen<1) || (cLen>1024) ) 
	{
		m_ulErrors |= g_cErrorInvalidCookieDataFormat;
		return false;
	}

	/*
	sprintf( szBack, "%s", pszBackToken );
	*/
	
	memset(szBack, '\0', 1025);
	strncpy(szBack, pszBackToken, cLen + 1);


	/** Get a decryption key index - return if it's a bad one... */
	szCryptKeyIndex[0] = szFront[0];
	cCryptKeyIndex =  strtoul( szCryptKeyIndex, 0, 16 );
		
	if ( cCryptKeyIndex>15 )
	{
		m_ulErrors |= g_cErrorInvalidCookieDataFormat;
		return false;
	}

	
	/** Store the key for possible later use... */
	m_cCryptKeyIndex = (long)cCryptKeyIndex;


 	/** Get a decryption key  */
	pszDecrypt = (unsigned char *)getCryptionKey( cCryptKeyIndex );



 	/** HEX unescape the data part of the GREEN */
	memset( szData, '\0', 513 );
	bStatus = hexUnescape( (unsigned char*)szBack, cLen, (unsigned char*)szData, 512 );
	
	if ( false == bStatus )
	{ 
		m_ulErrors |= g_cErrorHexUnescapeError;
		return false; 
	}



	/** Decrypt the data part of the green */
	cLen /= 2;
	memset( szBack, '\0', 1025 );
	bStatus = cryptXOR( (unsigned char *)szData, cLen, (unsigned char *)szBack, 512, (unsigned char *)pszDecrypt );

	if ( false == bStatus )
	{ 
		m_ulErrors |= g_cErrorDecryptionError;
		return false; 
	}


	/** Extract the info into this instance... */
	this->parseGreenData( szBack );
	
	return true;	
}





/** *****************************************************************
 *
 * CGreen::parseGreenData
 *
 * Parse out the information found within a GREEN.  This consists 
 * of known NAME/VALUE pairs,  arbitrary NAME/VALUE pairs,  and 
 * complex NAME/VALUE pairs, and arbitrary complex NAME/VALUE pairs./
 cookie data, eg:
 *
 *      d,34FD14FD356748BBAC...etc...34FBBC56443
 * 
 **/
bool CGreen::parseGreenData( char * pszInfo )
{
	char * pszNameValue = 0;
	unsigned int cLen = 0;
	unsigned long ulSeen = 0;
	short cSeen = 0;


	if ( 0==pszInfo ) 
	{
		m_ulErrors |= g_cErrorMissingOrNullGreenInfo;
		return false;
	}



	/*
		Sets of NAME/VALUE pairs  are delimited by ';'
	
		Maximum length of a name is 2 

		Maximum length of a VALUE is 256 
	*/


	pszNameValue = strtok( pszInfo, ";" );
	
	while( 0 != pszNameValue )
	{
		cSeen = 1;
		
		cLen = strlen( pszNameValue );
		
		if ( cLen < g_cMaxNameValuePairSize )
		{
			this->parseNameValuePair( pszNameValue, cLen, &ulSeen );
		}

		pszNameValue = strtok( 0, ";" );
	}
	
	
	/** Found no tokens */
	if ( 0==cSeen )
	{
		m_ulErrors |= g_cErrorMissingOrNullGreenInfo;
		return false;
	}
	
	return true;
}




/** *****************************************************************
 *
 * CGreen::parseNameValuePair
 *
 * Parse out a NAME/VALUE or complex NAME/VALUE pair 
 *
 * 'psz' is guaranteed not to be longer than 
 * 'g_cMaxNameValuePairSize' (272 = 256 + 16) bytes big. 
 *
 **/
bool CGreen::parseNameValuePair( const char * psz, unsigned int cLen, unsigned long * pulSeen )
{
	char szData[273] = {'\0'};
	char szName[18] = {'\0'};
	char szValue[258] = {'\0'};

	char * pszName = 0;
	char * pszData = 0;
	char * pszEnd = 0;

	unsigned int cNameLen = 0;
	unsigned int cDataLen = 0;
	

	/** Make a copy of the data */
	/*
	sprintf( szData, "%s", psz );
	*/

	memset(szData, '\0', 273);
	strncpy(szData, psz, strlen(psz) + 1);

	/** Find first occurance of '=' to get value */
	pszData = strchr( szData, '=' );

	if ( 0==pszData ) 
	{
		m_ulWarnings |= g_cWarnInvalidNameValuePair;
		return false;
	}

	pszEnd = pszData;
	*pszEnd = '\0';

	pszData++;
	
	/** Get name - make sure it has no illegal characters in it... */
	pszName = szData;

	if( 0!=strchr(pszName, '%') || 0!=strchr(pszName,',') || 0!=strchr(pszName,';') )
	{
		m_ulWarnings |= g_cWarnInvalidNameToken;
		return false;
	}


	cNameLen = strlen( pszName );
	cDataLen = strlen( pszData );
	
	/*
	   Maximum length of a NAME is 16 characters.  We're only 
	   setting known NAME/VALUE pairs here though - so,  it 
	   must be 2 characters big.

	   Maximum length of a VALUE or set of complex VALUEs 
	   is 256 characters
	*/
	
	if ( cNameLen>2 || cDataLen>256 ) 
	{
		m_ulWarnings |= g_cWarnOverflowNameValuePair;
		return false;
	}
	

	
	/** Compare all known Commerce Token known NAME values */
	for ( int i=0; i<g_cMaxKnownNameValuePairs; i++ )
	{
		/** Seen the token already... */
		if ( ((*pulSeen)&m_ulMask[i])==m_ulMask[i]) continue;

		/** Not seen it - found a match... */
		if ( 0 == strncmp( pszName, m_szNames[i], 2) )
		{	
			(*pulSeen) |= m_ulMask[i];
			this->addNameValuePair( pszName, cNameLen, pszData, cDataLen, i );
			return true;
		}
	}


	/** Name was not found... */
	return false;
}




/** *****************************************************************
 *
 * CGreen::addNameValuePair
 *
 * Parse out a NAME/VALUE or complex NAME/VALUE pair 
 *
 * 'psz' is guaranteed not to be longer than 
 * 'g_cMaxNameValuePairSize' (272 = 256 + 16) bytes big. 
 *
 **/
bool CGreen::addNameValuePair( char * pszName, unsigned int cNameLen, char * pszVal, unsigned int cValLen, int idx )
{
	int index = -1;
	char szSetValue[258] = {'\0'};
	unsigned int cSetValueLen = 0;	


	/** Error conditions */
    if( 0==pszName || 0==pszVal )
	{
		m_ulWarnings |= g_cWarnInvalidNameValuePair;
		return false;
	}
		
	if ( (cNameLen>2) || (cNameLen==0) )
	{
		m_ulWarnings |= g_cWarnInvalidNameValuePair;
		return false;
	}

    
	if ( (cValLen>256) || (cValLen==0) )
	{
		m_ulWarnings |= g_cWarnInvalidNameValuePair;
		return false;
	}

	
	/** If idx == -1 then we don't know the name of the NAME/VALUE pair yet */
	if ( -1 == idx )
	{
		for ( int i=0; i<g_cMaxKnownNameValuePairs; i++ )
		{
			/** Not seen it - found a match... */
			if ( 0 == strncmp(pszName, m_szNames[i], 2) )
			{	
				index = i;
				break;
			}
		}
	}
	else
	{
		index = idx;
	}

	
	/** Don't have a valid name */
	if ( index<0 || index>=g_cMaxKnownNameValuePairs )
	{
		m_ulWarnings |= g_cWarnUnknownNameToken;
		return false;
	}

	
	/** Is there data here already?  Don't leak */
	if ( 0 != m_pszValues[index] )
	{
		delete [] m_pszValues[index];
	}

	
	
	/** Escape the value.. we may be setting something like: DM=male%5'9"%150 */
	escapeInfo( pszVal, szSetValue, 257 );
	cSetValueLen = strlen( szSetValue );
	


	/** Reset the value */
	m_pszValues[index] = 0;
	m_pszValues[index] = new char[ cSetValueLen + 1 ];


	if ( 0 != m_pszValues[index] )
	{
		memset( m_pszValues[index], '\0', (cSetValueLen+1) );
		
		/** Don't use sprintf to do this...it ruins the format of the escaping... */
		/** sprintf( m_pszValues[index], szSetValue ); */

		for ( unsigned int i=0; i<cSetValueLen; i++ )
		{
			m_pszValues[index][i] = szSetValue[i];
		}
		
		m_cValueSize[index] = cSetValueLen;
		return true;
	}

	return false;
}







/** *****************************************************************
 *
 * CGreen::getRawInformation
 *
 * Show raw name/value data.
 *
 **/
bool CGreen::getRawInformation( char * psz, unsigned int cSize ) const
{
	unsigned int cLen = 0;
	unsigned int cTotalLen = 0;
	unsigned int i;
	char     szTemp[ g_cMaxNameValuePairSize + 32 ] = {'\0'};

	
	if ( 0==psz ) 
		return false;
	
	if ( cSize<20 )
		return false;


	memset( psz, '\0', cSize );
	cSize--;

	
	/** Enumerate the known name/value pairs */
	for ( i=0; i<g_cMaxKnownNameValuePairs; i++ )
	{
		if ( 0 != m_pszValues[i] )
		{
			/** There is no problem with using sprintf here because	 */
			/** it's not used on the "value" part of the NAME/VALUE	 */
			sprintf( szTemp, "[%02d] %s=", m_cValueSize[i], m_szNames[i] );
			
			/** HEX unescape the info contained within */
			char * pszBuffer = &szTemp[8];
			unescapeInfo( m_pszValues[i], pszBuffer, g_cMaxNameValuePairSize );

			strcat( szTemp, "\n" );
			cLen = strlen( szTemp );
		}
		else
		{
			sprintf( szTemp, "[00] %s=(null)\n", m_szNames[i] );
			cLen = 10;
		}
		
		/** Room to add it to outgoing buffer? */
		if ( (cLen+cTotalLen) < cSize )
		{
			strcat( psz, szTemp );
			cTotalLen += cLen;
		}
	}
	
	return true;
}





/** *****************************************************************
 *
 * CGreen::getWarningInfo
 *
 * Return this hard-coded warning information.  Also return the 
 * codes themselves.
 *
 **/
bool CGreen::getWarningInfo( char * psz, unsigned int cSize ) const
{
	unsigned int cTotalLen = 0;
	unsigned int cLen = 0;
	unsigned int i;


	char * pszWarn[] = { \
		"An invalid NAME/VALUE pair was found\n", \
		"A NAME token was found that contained illegal chars\n", \
		"A NAME/VALUE pair was found in which at least 1 token was too long\n", \
		"An unknown NAME/VALUE pair was found\n", \
		"There was a buffer overflow while trying to retrieve a value\n", \
		"The TimeToExpire value was invalid (see the GREEN spec)", \
		"A Date value was found to be invalid (see the GREEN spec)", \
		"A Time value was found to be invalid (see the GREEN spec)", \
		"A Date/Time value was found to be invalid (see the GREEN spec)", \
		"The token value was too large (>1024)"
	};
	
	if ( 0==psz )
		return false;

	memset( psz, '\0', cSize );
	cSize--;

	/** Add appropriate warning info to the outgoing buffer */
	for ( i=0; i<g_cMaxKnownWarnings; i++ )
	{
		if ( (m_ulWarnings&m_ulMask[i]) == m_ulMask[i] )
		{
			cLen = strlen( pszWarn[i] );

			if ( (cLen+cTotalLen) < cSize )
			{
				strcat( psz, pszWarn[i] );
				cTotalLen += cLen;
			} 
		}
	}

	return true;
}

/** *****************************************************************
 *
 * CGreen::getErrorInfo
 *
 **/
bool CGreen::getErrorInfo( char * psz, unsigned int cSize ) const
{
	unsigned int cTotalLen = 0;
	unsigned int cLen = 0;
	unsigned int i;
	
	char * pszErr[] = { \
		"The cookie data was missing or zero-length\n", \
		"The format of the cookie data was invalid\n", \
		"There was an error HEX unescaping the cookie value\n", \
		"There was an error decrypting the cookie value\n", \
		"The escaped/decrypted cookie info was missing or zero-length\n", \
		"The GREEN is missing one or more of it's required fields\n", \
		"There was an error while decrypting the GREEN\n", \
		"There was an error while HEX unescaping the GREEN\n", \
		"There was a buffer overflow while trying to write the GREEN\n" \
	};
	

	if ( 0==psz )
		return false;

	memset( psz, '\0', cSize );
	cSize--;

	/** Add appropriate warning info to the outgoing buffer */
	for ( i=0; i<g_cMaxKnownErrors; i++ )
	{
		if ( (m_ulErrors&m_ulMask[i]) == m_ulMask[i] )
		{
			cLen = strlen( pszErr[i] );

			if ( (cLen+cTotalLen) < cSize )
			{
				strcat( psz, pszErr[i] );
				cTotalLen += cLen;
			} 
		}
	}

	return true;
}







/** *****************************************************************
 *
 * CGreen::isValid
 *
 * Accessors for specific name/value pairs                           
 * 
 * true if Green contains all required name/value pairs, 
 * false otherwise  
 *
 **/
bool CGreen::isValid()
{
	if ( (0!=m_pszValues[0] ) &&	// USER_ID  (r)           ID 
		 (0!=m_pszValues[1] ) &&	// LOGID  (r)             LG  
		 (0!=m_pszValues[2] ) &&	// SESSION (r)            SN
		 (0!=m_pszValues[3] ) &&	// TOKEN_MASK (r)         TM
		 (0!=m_pszValues[4] ) &&	// USERNAME (r)           UN
		 (0!=m_pszValues[5] ) &&	// EMAIL (r)              EM
		 (0!=m_pszValues[6] ) &&	// SWID (r)		          SD
		 (0!=m_pszValues[7] ) &&	// DATE_ASSIGNED (r)      DA
		 (0!=m_pszValues[9] ) &&	// DATE_LAST_LOGIN (r)    DL
		 (0!=m_pszValues[10] ) )	// DATE_LAST_MODIFIED (r) LM
	{
		return true;
	}

	
	m_ulErrors |= g_cErrorGreenInfoIsIncomplete;
	return false;
}


	

/** *****************************************************************
 *
 * Accessors for specific name/value pairs                           
 * 
 * These all call "getValue" with the appropriate index.
 *
 **/

bool CGreen::hasFieldMask( unsigned int iMask ) const
{
    char  s1[10] = {'\0'};
    if( this->getValue( 19, s1, 10 ) == true ) {
	int fieldMask = atoi( s1 );
	if( ( fieldMask & iMask ) == iMask ) {
	    return true;
	} else {
	    return false;
	}
    }
    return true;
}

bool CGreen::isUserComplete() const
{
    return this->hasFieldMask( g_cCompleteRegRequiredFieldsMask );
}

bool CGreen::getUserId( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 0, psz, cSize ) );
}

bool CGreen::getLogId( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 1, psz, cSize ) );
}

bool CGreen::getSession( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 2, psz, cSize ) );
}

bool CGreen::getTokenMask( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 3, psz, cSize ) );
}

bool CGreen::getUsername( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 4, psz, cSize ) );
}

bool CGreen::getEmail( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 5, psz, cSize ) );
}

bool CGreen::getSWID( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 6, psz, cSize ) );
}

bool CGreen::getDateAssigned( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 7, psz, cSize ) );
}

bool CGreen::getTimeToExpire( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 8, psz, cSize ) );
}

bool CGreen::getDateLastLogin( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 9, psz, cSize ) );
}

bool CGreen::getDateLastModified( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 10, psz, cSize ) );
}

bool CGreen::getFirstname( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 11, psz, cSize ) );
}

bool CGreen::getLastname( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 12, psz, cSize ) );
}

bool CGreen::getDemoMask( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 13, psz, cSize ) );
}

bool CGreen::getOldUsername( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 14, psz, cSize ) );
}

bool CGreen::getOldAffiliate( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 15, psz, cSize ) );
}

bool CGreen::getBirthday( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 16, psz, cSize ) );
}

bool CGreen::getGeoCode( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 17, psz, cSize ) );
}

bool CGreen::getGender( char * psz, unsigned int cSize ) const
{
	return ( this->getValue( 18, psz, cSize ) );
}

/** *****************************************************************
 *
 * Accessors for specific name/value pairs                           
 * 
 * These all call "getValue" with the appropriate index.
 *
 **/
bool CGreen::getValue( unsigned int idx, char * psz, unsigned int cSize ) const
{
	unsigned int cLen = 0;								 

	if ( 0 == psz )
	{
		return false;
	}

	if ( idx > g_cMaxKnownNameValuePairs )
	{
		return false;
	}

	if ( 0 == m_pszValues[idx] )
	{
		return false;
	}

	memset( psz, '\0', cSize );

	/** HEX unescape the info. Turn "DM=male%2528 yrs old" into "DM=male%28yrs old" */
	unescapeInfo( m_pszValues[idx], psz, cSize ); 	

	return true;
}







/** *****************************************************************
 *
 * CGreen::setValue
 *
 * Private method for setting data memeber values.
 *
 * If 'psz' is null set an integer value data member.
 * If 'psz' is not null it's character string data. 
 *
 **/
bool CGreen::setValue( unsigned int idx, const char * psz, unsigned long l )
{
	char szNumber[32] = {'\0'};

	
	/** It's an integer value */
	if ( 0==psz )
	{
		sprintf( szNumber, "%u", (unsigned long)l );
		return ( this->setNameValuePair( idx, szNumber ) );
	}

	/** It's string value */
	return ( this->setNameValuePair( idx, psz ) );
}



/** *****************************************************************
 *
 * CGreen::setNameValuePair
 *
 * Private method for setting data memeber values.
 *
 * Takes in a string and an index...it can be any valid index
 *
 **/
bool CGreen::setNameValuePair( unsigned int idx, const char * psz )
{
	unsigned int cLen;
	char szTemp[ 2*g_cMaxNameValuePairSize+1 ] = {'\0'};

 	
	if ( idx>g_cMaxKnownNameValuePairs )
	{
		return false;
	}

	if ( 0==psz )
	{
		return false;
	}

	/** Make sure the value doesn't exist already - no leaks... */
	if ( 0 != m_pszValues[idx] )
	{
		delete [] m_pszValues[idx];
	}

	m_pszValues[idx] = 0;

	

	/** Is the new value okay to set? */
	cLen = strlen( psz );

	if ( cLen>g_cMaxGreenValueSize )
	{
		return false;
	}

	
	/** HEX escape the infor. We may be setting something like: DM=male%5'9"%150 */
	escapeInfo( psz, szTemp, (2*g_cMaxNameValuePairSize) );
	cLen = strlen( szTemp );


	/** Set the new value */
	m_pszValues[idx] = new char[ cLen + 1 ];
	
	if ( 0 != m_pszValues[idx] )
	{
		memset( m_pszValues[idx], '\0', (cLen+1) );
		
		/** Don't use 'sprintf' to set the value... it causes problems with the HEX escaping */
		/** sprintf( m_pszValues[idx], "%s", szTemp ); */

		for ( unsigned int i=0; i<cLen; i++ )
		{
			m_pszValues[idx][i] = szTemp[i];
		}

		m_cValueSize[idx] = cLen;	
		return true;
	}

	return false;
}








/** *****************************************************************
 *
 * CGreen::userHasToken
 *
 * See if a user has a particular token 
 *
 **/ 
bool CGreen::userHasToken( unsigned int tokenId ) const
{
	bool bErr;
	return ( decodeHexMask( tokenId, m_pszValues[3], &bErr ) );
}


/** *****************************************************************
 *
 * CGreen::getUserTokens
 *
 * Get the users tokens in a comma delimited list 
 *
 **/
bool CGreen::getUserTokens( char * psz, unsigned int cSize ) const
{
	unsigned int cLen = 0;
	unsigned int cTotalLen = 0;
	unsigned int cMaxId = 0;
	char szNum[12] = {'\0'};

	
	/** Errant parameters */
	if ( 0==psz )
	{
		return false;
	}


	memset( psz, '\0', cSize );
	cSize--;

	
	/** No tokens */
	if ( 0==m_pszValues[3] )
	{
		return true;
	}


	/** Determine the max ID that could be encoded */
	cMaxId = ( strlen(m_pszValues[3]) * 4 ) + 1;	

	for ( unsigned int i=1; i<cMaxId; i++ )
	{
		if ( true == userHasToken(i) )
		{
			sprintf( szNum, "%u,", i );
	
			cLen = strlen(szNum);
			
			/** Will it fit into return buffer? */
			if ( (cLen+cTotalLen) < cSize )
			{
				strcat( psz, szNum );
				cTotalLen += cLen;
			}
		}
	}

	return true;
}


