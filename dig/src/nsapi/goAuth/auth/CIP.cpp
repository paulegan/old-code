//--file: CIP.cpp----------------------------------------------------
//
#include "CIP.h"
#include "CPremiumArea.h"
#include <stdio.h>
#include <memory.h>
//#include <fstream.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#include "./../utils/CLog.h"
#include "./../utils/QuickRegEx.h"


/** '\\' for WINNT,  '/' otherwise... */
extern const char * g_pszFileDelim;

/** "HTTP/1.1 302 Moved Temporarily\r\n" */
extern const char * g_pszHTTPMoved;





/** *****************************************************************
 *
 * CIP::formatRedirectUrl
 *
 * Format the Url to redirect the user to.
 *
 * Both the 'Location' and 'appRedrect' Urls must be absolute, 
 * like so:
 *
 *  HTTP/1.1 302 Moved Temporarily\r\n
 *  Location: https://majestic.vanguard.com?appRedirect=http://www.go.com/premium/index.html\r\n\r\n
 *
 * TOOD: This function could overflow a buffer...(szBuf)
 *
 * 6/28/99 Mollman modified this function to re-URLEncode the "appRedirect"
 * portion of the Location: header, and replaced the hard-coded '&' delimiter
 * between the appRedirect URL and it's QueryString with an escaped '?'
 * delimiter. 
 *
 * This is a bug-fix for the problem that prevented users from being able to
 * log in via GlobalReg when the redirect URL contained a QueryString. 
 * (this bug manifested itself in redirects to the "playrbn" CGI in a premium
 * area.)	-jam
 **/

bool CIP::formatRedirectUrl( char * szBuf, unsigned int cSize, const char * pszQry, const char * pszUrl, const char * pszDefaultUrl, const char * pszOrigRqstUrl ) const
{
	char szOrigUrl[512] = {'\0'};
	char * pUseThisLocationUrl = 0;
	unsigned int cQryLen = 0;
	unsigned int cLen = 0;
	unsigned int cTotalLen = 0;


	

	// Error 
	if ( 0==szBuf ) return false;
	memset( szBuf, '\0', cSize );
	cSize-=6;


	// Which 'Location:' Url are we using? 
	if ( 0==pszUrl )
	{
		pUseThisLocationUrl = (char*)pszDefaultUrl;
	}
	else
	{
		pUseThisLocationUrl = (char*)pszUrl;
	}

 	// Obtain an absolute 'appRedirect' Url
	this->reformatUrl( pszOrigRqstUrl, szOrigUrl, 511 );

	// Format the 302, do we add a '?' before appRedirect or a '&'... 
	
	// Need to re-endcode the "appRedirect" portion of the "Location:" 
	// header before sending it out, else
	// goRegister will be unable to properly redirect for URLs containg
	// QueryStrings 
	// This is a bug-fix for SZ RealAudio authentication (-jam 6/21/99)
		
	// figure out query string delimiters (not sure what the logic of
	// the choice is -jam)
	// and put them back together with properly escaped delimiters
	if ( NULL != strchr( pUseThisLocationUrl, '?' ) ) {
		sprintf( szBuf, "%sLocation: %s&appRedirect=", g_pszHTTPMoved, pUseThisLocationUrl );
	} else {
		sprintf( szBuf, "%sLocation: %s?appRedirect=", g_pszHTTPMoved, pUseThisLocationUrl );
	}

   	cTotalLen = strlen( szBuf );

   	// Add 'appRedirect'Url to this 302
	// (after URLEncoding it again -jam)
	int iOrigUrlLen = strlen(szOrigUrl);
	iOrigUrlLen = (iOrigUrlLen * 2);

	if ( MAX_URL_LENGTH < iOrigUrlLen ) {
	  CLog::log("CIP::formatRedirectUrl> Panic..URL buffer too small to encode");
	  return false;
	}

	char szEncodedOrigUrl[MAX_URL_LENGTH];
	memset(szEncodedOrigUrl, 0, MAX_URL_LENGTH);
	
	// Encode the original URL (returns length of encoded string)
	cLen = URLEncodeString(szOrigUrl, szEncodedOrigUrl, MAX_URL_LENGTH); 
	// cLen = strlen( szOrigUrl );
	
	if ( cLen > 0 ) {
		// encoding succeded
   		if ((cLen+cTotalLen) < cSize ) {
			cTotalLen += cLen;
			strcat( szBuf, szEncodedOrigUrl );
		} else {
			CLog::log(" CIP::formatRedirectUrl> Panic..buffer is not large enough for 302" );
			return false;
		}

    } else {
		// Encoding failed, use unencoded URL and log error
		char szLogErr2[1024];
		memset(szLogErr2, 0, 1024);

		sprintf(szLogErr2, "CIP::formatRedirectUrl encoding original URL (%.472s) failed, using unencoded URL (%.472s)\n", szEncodedOrigUrl, szOrigUrl);
		CLog::log(szLogErr2);

		// reset length to keep char count correct
		cLen = strlen(szOrigUrl);

		// try to use original (unencoded) URL
   		if ((cLen+cTotalLen) < cSize ) {
			cTotalLen += cLen;
			strcat( szBuf, szOrigUrl );
		} else {

			CLog::log(" CIP::formatRedirectUrl> Panic..buffer is not large enough for 302" );
			return false;
		}
	}
 
	// Add the (re-URLEncoded) query string to this 302
	if ( (0!=pszQry) && ('\0'!=pszQry[0]) )
	{

		// make a copy of the const char* pszQuery (with room to expand it)
		int iQueryLen = strlen(pszQry);
		iQueryLen = (iQueryLen * 2);
		
		if ( MAX_URL_LENGTH < iQueryLen ) {
			CLog::log("CIP::formatRedirectUrl> Panic..QueryString buffer too small to encode");
			return false;
		}
		
		
		char szEncodedQuery[MAX_URL_LENGTH];
		memset(szEncodedQuery, 0, MAX_URL_LENGTH);

		// and encode the query string (returns new length of encoded string)
		cLen = URLEncodeString(pszQry, szEncodedQuery, iQueryLen);
		if(cLen > 0) {
			// encoding succeded. 

		  	// +2 since we will hand-encode the delimiter next
			if ( (cLen+cTotalLen+2) < cSize ) {
				
				// this delimiter needs to be encoded! -jam
				//  strcat( szBuf, "&" );
				strcat( szBuf, "%3f");	// changed from '&' to '?' (-jam)
				strcat( szBuf, szEncodedQuery );
				strcat( szBuf, "\r\n\r\n\0" ); // We sub'd 5 from cSize at the beginning 
	
				return true;
			} else {
				printf("CIP buffer too small for %s, failing\n", pszUrl);
				CLog::log(" CIP::formatRedirectUrl> Panic..buffer is not large enough for 302" );
				return false;
			}

		} else {
			// encoding failed, use unencoded query string and log error

			// +2 since we will hand-encode the delimiter next
			if ( (cLen+cTotalLen+2) < cSize ) {
	
				// this delimiter needs to be encoded! -jam
				//strcat( szBuf, "&" );
				strcat( szBuf, "%3f");	// changed from '&' to '?' (-jam)
				strcat( szBuf, pszQry );
				strcat( szBuf, "\r\n\r\n\0" ); // We sub'd 5 from cSize at the beginning 
				char szLogErr3[1024];
				memset(szLogErr3, 0, 1024);
				sprintf(szLogErr3, "CIP::formatRedirectUrl encoding original URL (%.472s) failed, using unencoded URL (%.472s)\n", szEncodedOrigUrl, szOrigUrl);
				CLog::log(szLogErr3);

				return true;
			}  else {
				CLog::log(" CIP::formatRedirectUrl> Panic..buffer is not large enough for 302" );
				return false;
			}
		}
	}
	
	
	strcat( szBuf, "\r\n\r\n\0" );  // We sub'd 5 from cSize at the beginning 
	return true;
}




/** *****************************************************************
 *
 * CIP::isFreeAreaUrl
 *
 * Is the Url request a Free Area?
 *
 **/
bool CIP::isFreeAreaUrl( const char * pszUrl ) const
{	
	try
	{
		if ( 0==pszUrl ) return false;

		for ( unsigned int k=0; k<m_cFreeAreas; k++ )
		{
			if ( 0 != m_pszFreeAreas[k]  )
			{
				/** New code -- regular expressions */
				if ( true == matchQuickRegEx( pszUrl, m_pszFreeAreas[k] ) )
				{
					return true;
				}

			}
		}
	
		return false;

	}
	catch( ... )
	{
		CLog::log("CIP::isFreeAreaUrl> Exception!");
		return false;
	}
}



/** *****************************************************************
 *
 * CIP::isAuthRedirectPage
 *
 * Is the Url request a Login Page,  an Upsell Page,  or an Access 
 * Denied Page?
 *
 * This is mostly for the case when a premium area has a 
 * Request Url of "/".
 *
 **/
bool CIP::isAuthRedirectPage( const char * pszUrl ) const
{
	CPremiumArea * pa = 0;
	unsigned int cLen = 0;
	char * psz = 0;

	try
	{

		if ( 0==pszUrl ) return false;


		/** It's one of the 4 default pages for this IP address */
		if ( (0==strncmp(m_pszDefaultLoginPageUrl,        pszUrl, m_cDefaultLoginPageUrlSize        )) ||
			 (0==strncmp(m_pszDefaultUpsellPageUrl,       pszUrl, m_cDefaultUpsellPageUrlSize       )) ||
			 (0==strncmp(m_pszDefaultAccessDeniedPageUrl, pszUrl, m_cDefaultAccessDeniedPageUrlSize )) ||
			 (0==strncmp(m_pszDefaultSignupPageUrl,       pszUrl, m_cDefaultSignupPageUrlSize )) )
		{
			return true;
		}


		/** It's one of the 4 default pages for one of the Premium Areas */
		for ( unsigned int i=0; i<m_cAreas; i++ )
		{
			pa = m_Areas[i];
		
			if ( pa->hasLoginPageUrl() )	/** Login page Url? */
			{
				psz = (char*)pa->getLoginPageUrl( &cLen );
				if ( 0==strncmp( psz, pszUrl, cLen ) ) return true;
			}

			if ( pa->hasUpsellPageUrl() )	/** Upsell Page Url? */
			{
				psz = (char*)pa->getUpsellPageUrl( &cLen );
				if ( 0==strncmp( psz, pszUrl, cLen ) ) return true;
			}

			if ( pa->hasAccessDeniedPageUrl() ) /** AccessDenied Page Url? */
			{
				psz = (char*)pa->getAccessDeniedPageUrl( &cLen );
				if ( 0==strncmp( psz, pszUrl, cLen ) ) return true;
			}

			if ( pa->hasSignupPageUrl() ) /** Signup Page Url? */
			{
				psz = (char*)pa->getSignupPageUrl( &cLen );
				if ( 0==strncmp( psz, pszUrl, cLen ) ) return true;
			}

		}

		return false;


	}
	catch( ... )
	{
		CLog::log("CIP::isAuthRedirectPage> Exception!");
		return false;
	}
}


/** *****************************************************************
 *
 * CIP::findBestPremiumAreaMatch
 *
 * Find the premium area that best matches the given Url.
 * Once found,  return a pointer to that premium area.
 *
 * Return true if a premium area was found,  false if not.
 *
 **/
bool CIP::findBestPremiumAreaMatch( const char * pszUrl, unsigned int cLen, CPremiumArea ** ppPa ) const
{
	bool bFound = false;
	unsigned int cMatch = 0;
	unsigned int cBestMatch = 0;

	try
	{

		/** Look at each premium area in the collection */
		for ( unsigned int i=0; i<m_cAreas; i++ )
		{
			/** See if the Url matches at all... */
			if ( true == m_Areas[i]->matchesPremiumUrl( pszUrl, cLen, &cMatch ) )
			{
				/** If it's the best match so far... */
				if ( cMatch > cBestMatch )
				{
					cBestMatch = cMatch;
					*ppPa = m_Areas[i];
					bFound = true;
				}
			}
		}
		

		return bFound;
	}
	catch( ... )
	{
		CLog::log("CIP::findBestPremiumAreaMatch> Exception!");
		return false;
	}
}





/** *****************************************************************
 *
 * CIP::CIP
 *
 **/
CIP::CIP()
{
	for ( unsigned int i=0; i<g_cMaxPremiumAreas; i++ )
	{ 
		m_Areas[i] = 0; 
	}

	for ( unsigned int j=0; j<g_cMaxFreeAreas; j++ )
	{ 
		m_pszFreeAreas[j] = 0; 
		m_cFreeAreaSize[j] = 0;
	}

	m_cAreas                        = 0;
	m_cFreeAreas                    = 0;

	m_pszHostName                   = 0;
	m_pszServiceName                = 0;
	m_pszLameBrowserUrl             = 0;
	m_pszPort                       = 0;
	m_pszDomainName                 = 0;

	m_pszDefaultLoginPageUrl        = 0;
	m_pszDefaultUpsellPageUrl       = 0;
	m_pszDefaultAccessDeniedPageUrl = 0;
	m_pszDefaultSignupPageUrl       = 0;
	
	m_cDefaultLoginPageUrlSize        = 0;
	m_cDefaultUpsellPageUrlSize       = 0;
	m_cDefaultAccessDeniedPageUrlSize = 0;
	m_cDefaultSignupPageUrlSize       = 0;

	m_pszDefaultAbsoluteLoginPageUrl = 0;
	m_pszDefaultAbsoluteUpsellPageUrl = 0;
	m_pszDefaultAbsoluteAccessDeniedPageUrl = 0;
	m_pszDefaultAbsoluteSignupPageUrl = 0;
	m_pszAbsoluteLameBrowserUrl = 0;

	m_bDefaultActiveLoginRequired = true;
	m_bDefaultCaching = false;

	m_pszDefaultRequiredDataList = 0;
    m_cDefaultRequiredDataMask   = 0;

	return;
}



/** *****************************************************************
 *
 * CIP::addFreeArea
 *
 * Add a free area to the collection.
 *
 * Returns true if successful,  false if not.
 *
 **/
bool CIP::addFreeArea( const char * pszUrl ) 
{
	if ( (m_cFreeAreas+1) >= g_cMaxFreeAreas )
		return false;

	char * psz = 0;
	unsigned int cLen = 0;

	try
	{

		cLen = strlen( pszUrl ) + 1;

		psz = new char[ cLen ];

		if ( 0 == psz ) return 0;
	
		memset( psz, '\0', cLen );

		/*
		sprintf( psz, "%s", pszUrl );
		*/

		strncpy(psz, pszUrl, cLen);

		m_pszFreeAreas[ m_cFreeAreas ] = psz;
		m_cFreeAreaSize[ m_cFreeAreas ] = cLen - 1;
		m_cFreeAreas++;

		return true;
	}
	catch( ... )
	{
		CLog::log("CIP::addFreeArea> Exception!");
		return false;
	}
}



/** *****************************************************************
 *
 * CIP::addPremiumArea
 *
 * Add a premium area to the collection.
 *
 * pszN = Area name
 * pszR = Area Urls
 * pszT = Area Tokens Required (the logical expression)
 * pszO = original area tokens required (the expression)
 * pszL = Area Login Url
 * pszU = Area Upsell Url
 * pszA = Area AccessDenied Url
 * pszS = Area Signup Url
 * pszAL= Active login flag
 * pszC = Caching flag
 * pszDL= Required Data List
 * cDM  = Required Data Mask
 *
 * Returns true if successful,  false if not.
 *
 **/
bool CIP::addPremiumArea
( 
	const char * pszN, 
	const char * pszR,
	const char * pszT, 
	const char * pszO,
	const char * pszL, 
	const char * pszU, 
	const char * pszA,
	const char * pszS,
	const char * pszAL,
	const char * pszC,
	const char * pszDL,
	int          cDM
)
{
	/** Data error */
	if ( 0==pszN || 0==pszR || 0==pszT || 0==pszO || 0==pszL || 0==pszU || 0==pszA || 0==pszS || 0==pszAL || 0==pszC ||
		 0==pszDL || cDM<0)
		return false;

	try
	{

		/** No room */
		if ( (m_cAreas+1) >= g_cMaxPremiumAreas )
		{
			return false;
		}


		/** Add it */
		m_Areas[ m_cAreas ] = new CPremiumArea( pszN, pszR, pszT, pszO, pszL, pszU, pszA, pszS, pszAL, pszC, pszDL, cDM );
		
		/** Format the absolue URLs */
		m_Areas[ m_cAreas ]->m_pszAbsoluteLoginPageUrl = reformatUrl( pszL );
		m_Areas[ m_cAreas ]->m_pszAbsoluteUpsellPageUrl = reformatUrl( pszU );
		m_Areas[ m_cAreas ]->m_pszAbsoluteAccessDeniedPageUrl = reformatUrl( pszA );
		m_Areas[ m_cAreas ]->m_pszAbsoluteSignupPageUrl = reformatUrl( pszS );

		m_cAreas++;
	
		return true;

	}
	catch( ... )
	{
		CLog::log("CIP::addPremiumArea> Exception!");
		return false;
	}
}




/** *****************************************************************
 *
 * CIP::init
 *
 * Initialize the IP with the given parameters... no premium areas 
 * though. Set:
 *
 *  m_pszServiceName                   m_pszHostName 
 *  m_pszLameBrowserUrl				   m_pszAuthServerUrl 
 *  m_pszAuthRealm					   m_pszAuthServerName
 *  m_pszDefaultLoginPageUrl           m_pszDefaultUpsellPageUrl
 *  m_pszDefaultAccessDeniedPageUrl    m_pszDefaultSignupPageUrl
 *  m_pszDomainName                    m_bDefaultActiveLoginRequired
 *  m_bDefaultCaching                  m_pszDefaultRequiredDataList
 *  m_cDefaultRequiredDataMask
 *
 **/
bool CIP::init
( 
	const char * pszN, 
	const char * pszH, 
	const char * pszP, 
	const char * pszLa,
	const char * pszLo,
	const char * pszU,
	const char * pszA,
	const char * pszS,
	const char * pszD,
	const char * pszAL,
	const char * pszC,
	const char * pszDL,
	int          cDM
)
{
	if ( 0==pszN || 0==pszH || 0==pszP || 0==pszLa || 0==pszLo ||
		 0==pszU || 0==pszA || 0==pszS || 0==pszD || 0==pszAL ||
		 0==pszC || 0==pszDL || cDM < 0 )
		return false;

	unsigned int cN=0, cH=0, cP=0, cLa=0, cLo=0, cU=0, cA=0, cS=0, cD=0, cDL=0;
	char *pN=0, *pH=0, *pP=0, *pLa=0, *pLo=0, *pU=0, *pA=0, *pS=0, *pD=0, *pDL=0;

	cN   = strlen(pszN)   + 1;		pN   = new char[ cN   ];
	cH   = strlen(pszH)   + 1;		pH   = new char[ cH   ];
	cP   = strlen(pszP)   + 1;		pP   = new char[ cP   ];
	cLa  = strlen(pszLa)  + 1;		pLa  = new char[ cLa  ];
	cLo  = strlen(pszLo)  + 1;		pLo  = new char[ cLo  ];
	cU   = strlen(pszU)   + 1;		pU   = new char[ cU   ];
	cA   = strlen(pszA)   + 1;		pA   = new char[ cA   ];
	cS   = strlen(pszS)   + 1;		pS   = new char[ cS   ];
	cD   = strlen(pszD)   + 1;		pD   = new char[ cD   ];
	cDL  = strlen(pszDL)  + 1;		pDL  = new char[ cDL  ];


	if ( 0!=pN && 0!=pH && 0!=pP && 0!=pLa && 0!=pLo && 0!=pU && 0!=pA && 0!=pS && 0!=pD && 0!=pDL )
	{
		memset( pN,   '\0', cN   );
		memset( pH,   '\0', cH   );
		memset( pP,   '\0', cP   );
		memset( pLa,  '\0', cLa  );
		memset( pLo,  '\0', cLo  );
		memset( pU,   '\0', cU   );
		memset( pA,   '\0', cA   );
		memset( pS,   '\0', cS   );
		memset( pD,   '\0', cD   );
		memset( pDL,  '\0', cDL  );
		
		/*
		sprintf( pN,   "%s", pszN   );
		sprintf( pH,   "%s", pszH   );
		sprintf( pP,   "%s", pszP   );
		sprintf( pLa,  "%s", pszLa  );
		sprintf( pLo,  "%s", pszLo  );
		sprintf( pU,   "%s", pszU   );
		sprintf( pA,   "%s", pszA   );
		sprintf( pS,   "%s", pszS   );
		sprintf( pD,   "%s", pszD   );
		sprintf( pDL,  "%s", pszDL  );
		*/

		strncpy( pN,   pszN,  cN );
		strncpy( pH,   pszH,  cH );
		strncpy( pP,   pszP,  cP );
		strncpy( pLa,  pszLa, cLa );
		strncpy( pLo,  pszLo, cLo );
		strncpy( pU,   pszU,  cU );
		strncpy( pA,   pszA,  cA );
		strncpy( pS,   pszS,  cS );
		strncpy( pD,   pszD,  cD );
		strncpy( pDL,  pszDL, cDL );
		
		m_pszServiceName                = pN;
		m_pszHostName                   = pH;
		m_pszPort                       = pP;
		m_pszLameBrowserUrl             = pLa;
		m_pszDomainName                 = pD;

		m_pszDefaultLoginPageUrl        = pLo;
		m_pszDefaultUpsellPageUrl       = pU;
		m_pszDefaultAccessDeniedPageUrl = pA;
		m_pszDefaultSignupPageUrl       = pS;

		m_cDefaultLoginPageUrlSize        = cLo-1;
		m_cDefaultUpsellPageUrlSize       = cU-1;
		m_cDefaultAccessDeniedPageUrlSize = cA-1;
		m_cDefaultSignupPageUrlSize       = cS-1;

		m_pszDefaultAbsoluteLoginPageUrl        = reformatUrl( m_pszDefaultLoginPageUrl );
		m_pszDefaultAbsoluteUpsellPageUrl       = reformatUrl( m_pszDefaultUpsellPageUrl );
		m_pszDefaultAbsoluteAccessDeniedPageUrl = reformatUrl( m_pszDefaultAccessDeniedPageUrl );
		m_pszDefaultAbsoluteSignupPageUrl       = reformatUrl( m_pszDefaultSignupPageUrl );
		m_pszAbsoluteLameBrowserUrl             = reformatUrl( m_pszLameBrowserUrl );

		m_bDefaultActiveLoginRequired = true;

		if ( ( (pszAL[0] != '\0') && (toupper(pszAL[0]) == 'F') ) &&
			 ( (pszAL[1] != '\0') && (toupper(pszAL[1]) == 'A') ) &&
			 ( (pszAL[2] != '\0') && (toupper(pszAL[2]) == 'L') ) &&
			 ( (pszAL[3] != '\0') && (toupper(pszAL[3]) == 'S') ) &&
			 ( (pszAL[4] != '\0') && (toupper(pszAL[4]) == 'E') ) )
		{
			m_bDefaultActiveLoginRequired = false;
		}

		m_bDefaultCaching = false;

		if ( ( (pszC[0] != '\0') && (toupper(pszC[0]) == 'T') ) &&
			 ( (pszC[1] != '\0') && (toupper(pszC[1]) == 'R') ) &&
			 ( (pszC[2] != '\0') && (toupper(pszC[2]) == 'U') ) &&
			 ( (pszC[3] != '\0') && (toupper(pszC[3]) == 'E') ) )
		{
			m_bDefaultCaching = true;
		}

        m_pszDefaultRequiredDataList = pDL;
		m_cDefaultRequiredDataMask   = cDM;

		return true;
	}
	else
	{
		if ( 0 !=pN   ) { delete [] pN;   }
		if ( 0 !=pH   ) { delete [] pH;   }
		if ( 0 !=pP   ) { delete [] pP;   }
		if ( 0 !=pLa  ) { delete [] pLa;  }
		if ( 0 !=pLo  ) { delete [] pLo;  }
		if ( 0 !=pU   ) { delete [] pU;   }
		if ( 0 !=pA   ) { delete [] pA;   }
		if ( 0 !=pS   ) { delete [] pS;   }
		if ( 0 !=pD   ) { delete [] pD;   }
		if ( 0 !=pDL  ) { delete [] pDL;  }

		return false;
	}

}




/** *****************************************************************
 *
 * CIP::reformatUrl
 *
 * given either "/login.html", "http://www.go.com/login.html", 
 * or "https://www.go.com/login.html",  determine if the Url 
 * needs to be reformatted to be absolute.  If so,  return 
 * the reformatted Url allocated off of the heap.
 *
 * If pszBuffer is non-NULL,  put the redirect there.
 *
 * Otherwise,  new it off of the heap.
 *
 **/
char * CIP::reformatUrl( const char * psz, char * pszBuffer, unsigned int cSize ) const
{
	unsigned int cLen=0, cNewLen=0;
	char sz[12] = {'\0'};
	char * pszRet = 0;

	
	// If there is no URL at all - this is an error...
	if ( 0==psz ) return 0;
	
	// If the Url is all whitespace,  then don't make a default Url out of it...
	if ( psz[0] == '\0' ) return 0;
	

	cLen = strlen( psz );

	
	
	/** Look at the beginning of the Url */
	for ( unsigned i=0; i<cLen && i<7; i++ )
	{
		sz[i] = tolower( psz[i] ); 
	}


	if ( 0!=pszBuffer )
	{
		memset( pszBuffer, '\0', cSize );
		cSize--;
	}
	
		
	/** Reformatting required... */
	if ( (0 != strncmp( sz, "http://", 7 )) && (0 != strncmp( sz, "https:/", 7 )) )
	{

		//
		// If the host does not listen on PORT 80,  the new URL is:
		//      "http://" + m_pszHostName + ":" + PORT + m_pszDefaultLoginPageUrl 
		//
		if ( 0 != strcmp( m_pszPort, "80" ))
		{
			cNewLen = 7 + strlen(m_pszHostName) + 1 + strlen(m_pszPort) + cLen + 1;
			
			if ( 0!=pszBuffer )
			{
				if ( cNewLen>=cSize )
				{
					return pszRet;
				}
				else
				{
					sprintf(pszBuffer, "http://%s:%s%s", m_pszHostName, m_pszPort, psz );
					return pszRet;
				}
			}
			else
			{
				pszRet = new char[ cNewLen ];
				memset( pszRet, '\0', cNewLen );
				sprintf(pszRet, "http://%s:%s%s", m_pszHostName, m_pszPort, psz );
				return pszRet;
			}

		}
		//
		// If the host listens on PORT 80,  the new URL is:
		//      "http://" + m_pszHostName + m_pszDefaultLoginPageUrl 
		//
		else
		{
			cNewLen = 7 + strlen(m_pszHostName) + cLen + 1;
			
			if ( 0!=pszBuffer )
			{
				if ( cNewLen>=cSize )
				{
					return pszRet;
				}
				else
				{
					sprintf(pszBuffer, "http://%s%s", m_pszHostName, psz );
					return pszRet;
				}
			}
			else
			{
				pszRet = new char[ cNewLen ];
				memset( pszRet, '\0', cNewLen );
				sprintf(pszRet, "http://%s%s", m_pszHostName, psz );
				return pszRet;
			}
		}
	
	}
	/** No reformatting required...just make a copy of the incoming Url */
	else
	{
		cNewLen = cLen+1;
		
		if ( 0!=pszBuffer )
		{
			if ( cNewLen>=cSize )
			{
				return pszRet;
			}
			else
			{
				sprintf(pszBuffer, "%s", psz );
				return pszRet;
			}
		}
		else
		{
			pszRet = new char[cNewLen];
			memset( pszRet, '\0', cNewLen );
			sprintf( pszRet, "%s", psz );
			return pszRet;
		}
	}

//	return pszRet;
}



/** *****************************************************************
 *
 * CIP::destroy
 *
 * Nuke everything - reset all counters/member variable 
 * pointers to zero.
 *
 **/
void CIP::destroy() 
{

	try
	{

		for ( unsigned int i=0; i<g_cMaxPremiumAreas; i++ )
		{
			if ( 0 != m_Areas[i] )
			{	
				delete m_Areas[i];
			}

			m_Areas[i] = 0;
		}

		m_cAreas = 0;

		for ( unsigned int j=0; j<g_cMaxFreeAreas; j++ )
		{
			if ( 0 != m_pszFreeAreas[j] )
			{	
				delete [] m_pszFreeAreas[j];
			}

			m_pszFreeAreas[j] = 0;
			m_cFreeAreaSize[j] = 0;
		}

		m_cFreeAreas = 0;
	
		if ( 0 != m_pszHostName )                   { delete [] m_pszHostName;    }
		if ( 0 != m_pszServiceName )                { delete [] m_pszServiceName;    }
		if ( 0 != m_pszLameBrowserUrl )             { delete [] m_pszLameBrowserUrl;  }
		if ( 0 != m_pszPort )                       { delete [] m_pszPort;             }
		if ( 0 != m_pszDomainName )                 { delete [] m_pszDomainName;           }
		if ( 0 != m_pszDefaultLoginPageUrl )        { delete [] m_pszDefaultLoginPageUrl;  }
		if ( 0 != m_pszDefaultUpsellPageUrl )       { delete [] m_pszDefaultUpsellPageUrl;  }
		if ( 0 != m_pszDefaultAccessDeniedPageUrl ) { delete [] m_pszDefaultAccessDeniedPageUrl; }
		if ( 0 != m_pszDefaultSignupPageUrl )       { delete [] m_pszDefaultSignupPageUrl; }
	
		if ( 0 != m_pszDefaultAbsoluteLoginPageUrl )        { delete [] m_pszDefaultAbsoluteLoginPageUrl; } 
		if ( 0 != m_pszDefaultAbsoluteUpsellPageUrl )       { delete [] m_pszDefaultAbsoluteUpsellPageUrl; } 
		if ( 0 != m_pszDefaultAbsoluteAccessDeniedPageUrl ) { delete [] m_pszDefaultAbsoluteAccessDeniedPageUrl; }
		if ( 0 != m_pszDefaultAbsoluteSignupPageUrl )       { delete [] m_pszDefaultAbsoluteSignupPageUrl; }
		if ( 0 != m_pszAbsoluteLameBrowserUrl )             { delete [] m_pszAbsoluteLameBrowserUrl; }

        if ( 0 != m_pszDefaultRequiredDataList ) { delete [] m_pszDefaultRequiredDataList; }

		m_pszHostName                   = 0;
		m_pszPort                       = 0;
		m_pszServiceName                = 0;
		m_pszLameBrowserUrl             = 0;
		m_pszDomainName                 = 0;
		m_pszDefaultLoginPageUrl        = 0;
		m_pszDefaultUpsellPageUrl       = 0;
		m_pszDefaultAccessDeniedPageUrl = 0;
		m_pszDefaultSignupPageUrl       = 0;

		m_cDefaultLoginPageUrlSize        = 0;
		m_cDefaultUpsellPageUrlSize       = 0;
		m_cDefaultAccessDeniedPageUrlSize = 0;
		m_cDefaultSignupPageUrlSize       = 0;

		m_pszDefaultAbsoluteLoginPageUrl = 0;
		m_pszDefaultAbsoluteUpsellPageUrl = 0;
		m_pszDefaultAbsoluteAccessDeniedPageUrl = 0;
		m_pszDefaultAbsoluteSignupPageUrl = 0;
		m_pszAbsoluteLameBrowserUrl = 0;

		m_pszDefaultRequiredDataList = 0;

		return;
	
	}
	catch( ... )
	{
		CLog::log("CIP::destroy> Exception!");
		return;
	}
}



/** *****************************************************************
 *
 * CIP::~CIP
 *
 **/
CIP::~CIP()
{
	this->destroy();
	return;
}


