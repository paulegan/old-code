//--file: CGoAuthIPCollection.cpp------------------------------------
//
#include <stdio.h>
#include <memory.h>
#include <fstream.h>
#include <string.h>
#include <ctype.h>

#include "CGoAuthIPCollection.h"
#include "CGoAuthResponse.h"
#include "CPremiumArea.h"
#include "CIP.h"
#include "./../green/CGreen.h"
#include "./../green/GreenUtils.h"
#include "./../utils/StrUtils.h"
#include "./../utils/UrlEncode.h"
#include "./../utils/CLog.h"


/** XML tag names */
static const char * g_pszXMLTag_Service                     = "Service";
static const char * g_pszXMLTag_Name                        = "Name";
static const char * g_pszXMLTag_Host                        = "Host";
static const char * g_pszXMLTag_Port                        = "Port";
static const char * g_pszXMLTag_LameBrowserUrl              = "Lame-Browser-Url";
static const char * g_pszXMLTag_DomainName                  = "Domain-Name";
static const char * g_pszXMLTag_DefaultLoginPageUrl         = "Default-Login-Page-Url";
static const char * g_pszXMLTag_DefaultUpsellPageUrl        = "Default-Upsell-Page-Url";
static const char * g_pszXMLTag_DefaultAccessDeniedPageUrl  = "Default-Access-Denied-Page-Url";
static const char * g_pszXMLTag_DefaultSignupPageUrl        = "Default-Signup-Page-Url";
static const char * g_pszXMLTag_DefaultActiveLoginRequired  = "Default-Active-Login-Required";
static const char * g_pszXMLTag_DefaultCaching              = "Default-Caching";
static const char * g_pszXMLTag_DefaultRequiredData         = "Default-Required-Data";
static const char * g_pszXMLTag_PremiumArea                 = "Premium-Area";
static const char * g_pszXMLTag_FreeArea                    = "Free-Area";
static const char * g_pszXMLTag_RequestUrls                 = "Request-Urls";
static const char * g_pszXMLTag_TokensRequired              = "Tokens-Required";
static const char * g_pszXMLTag_LoginPageUrl                = "Login-Page-Url";
static const char * g_pszXMLTag_UpsellPageUrl               = "Upsell-Page-Url";
static const char * g_pszXMLTag_AccessDeniedPageUrl         = "Access-Denied-Page-Url";
static const char * g_pszXMLTag_SignupPageUrl               = "Signup-Page-Url";
static const char * g_pszXMLTag_ActiveLoginRequired         = "Active-Login-Required";
static const char * g_pszXMLTag_Caching                     = "Caching";
static const char * g_pszXMLTag_RequiredData                = "Required-Data";


/** '\\' for WINNT,  '/' otherwise... */
extern const char * g_pszFileDelim;

/** HTTP 1.1 html response header */
extern const char * g_pszHTTPHeader;

/** HTTP 1.1 text/html response header */
extern const char * g_pszContentTypeHeader;

/** HTTP 1.1 cache control response header */
extern const char * g_pszCacheControlHeader;


/** *****************************************************************
 *
 * CGoAuthIPCollection::handleGreenRequest
 *
 * Set a Green in the appropriate domain
 *
 * To be valid - we need a minimum of:
 *
 *		USER_ID  (r)           ID 
 *		LOGID  (r)             LG  
 *		SESSION (r)            SN
 *		TOKEN_MASK (r)         TM
 *		USERNAME (r)           UN
 *		EMAIL (r)              EM
 *		SWID (r)	           SD
 *		DATE_ASSIGNED (r)      DA
 *		DATE_LAST_LOGIN (r)    DL
 *		DATE_LAST_MODIFIED (r) LM
 *
 **/
bool CGoAuthIPCollection::setGreen( int cIndex, CGoAuthResponse & r ) const
{
	static unsigned long cKeyIndex = 15;
	const char * psz1 = "ID=1;LG=1;SN=1;TM=FFFFFF;UN=go;EM=go@go.com;SD={01234567890123456789};DA=19101998095436;DL=19101998095436;LM=19101998095436";
	unsigned char szBuffer[256] = {'\0'};
	unsigned char szGreen[512]  = {'\0'};
	char szCookie[1025]         = {'\0'};
	char szMsg[1025]            = {'\0'};
	char * pszDomain            = "";
	
	unsigned int cLen;
	CGreen g;
	CIP * pIp = 0;


	try
	{
		/** Get an encryption key index... */
		cKeyIndex += 1;
		
		if ( cKeyIndex >=16 )
		{
			cKeyIndex = 0;
		}

		/** Determine the contents of the GREEN */
		cLen = strlen( psz1 );
		cryptXOR( (const unsigned char *)psz1, cLen, szBuffer, 255, getCryptionKey( cKeyIndex ) );
		hexEscape( szBuffer, cLen, szGreen, 511 );
	
		
		/** Format the message */
		memset( szMsg, '\0', 1025 );
		sprintf( szMsg, "<html><body><font size=2>Received cookie: <br><ul><li>ID=1<li>LG=1<li>SN=1<li>TM=FFFFFF<li>UN=go<li>EM=go@go.com<li>SD={01234567890123456789}<li>DA=19101998095436<li>DL=19101998095436<li>LM=19101998095436</ul>\n</font></body></html>\r\n" );
		const char * psz1 = "ID=1;LG=1;SN=1;TM=FFFFFF;UN=go;EM=go@go.com;SD={01234567890123456789};DA=19101998095436;DL=19101998095436;LM=19101998095436";
	
		
		/** Need a domain name asssociated with this IP address */
		pIp = m_IP[ cIndex ];
		
		if ( 0 != pIp )
		{
			pszDomain = (char*)pIp->getDomainName();
		}


		/** Format the cookie header */
		memset( szCookie, '\0', 1025 );
		
		
		/** Only assign it a domain if the DOMAIN value is not "N/A" ) */
		if ( 0 == strcmp( pszDomain, "N/A" ) )
		{
			sprintf( szCookie, "Set-Cookie: GREEN=%X,%s; path=/\r\n\r\n", cKeyIndex, (char*)szGreen );
		}
		else
		{
			sprintf( szCookie, "Set-Cookie: GREEN=%X,%s; path=/; domain=%s\r\n\r\n", cKeyIndex, (char*)szGreen, pszDomain );
		}


		/**  "HTTP/1.1 200 OK\r\n */
		r.addResponseHeader( g_pszHTTPHeader );

		/** Content-Type: text/html\r\n"  */
		r.addResponseHeader( g_pszContentTypeHeader );

		/** "Set-Cookie: GREEN=%X,%s; path=/; domain=%s;\r\n\r\n"  */
		r.addResponseHeader( szCookie );
	
		/** "<html><body> ... </body></html>"  */
		r.addMessageBody( szMsg );
	
		r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
		return true;

	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::setGreen> Exception!");
		return false;
	}
}




/** *****************************************************************
 *
 * CGoAuthIPCollection::isAuthRedirectPage
 *
 * Determine if the requested Url is one of this IP's:
 *
 *   o DefaultLoginPageUrl
 *   o DefaultUpsellPageUrl
 *   o DefaultAccessDeniedPageUrl
 *   o DefaultSignupPageUrl
 *
 * Or one of these pages in a Premium Area contained within 
 * the IP address.
 *
 * The cIdx argument is the index of the IP addres with the 
 * collection -- this was already looked up and does not need 
 * to be looked up again.
 *
 * Returns true if the Url is a "special" url,  false if not.
 *
 **/
bool CGoAuthIPCollection::isAuthRedirectPage( unsigned int cIdx, const char * pszUrl ) const
{
	CIP * pIp = 0;
	bool bRet;

	/** This call occurs between LockReinit()/UnlockReinit() */
	
	try
	{
		pIp = m_IP[ cIdx ];
		
		if ( 0 == pIp )
		{
			return false;
		}
		
		bRet = pIp->isAuthRedirectPage(pszUrl);
		return (bRet);
	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::isAuthRedirectPage> Exception!");
		return false;
	}
}



/** *****************************************************************
 *
 * CGoAuthIPCollection::authenticate
 *
 * Perform authentication.
 *
 * A couple of things are known at this point in processing:
 *
 *   1) cIdx is the index of the IP address requiring the authentication.
 *   2) No input parameters are NULL
 *
 *
 * TODO: Make Interpreter local to this function for thread-safety..?
 *
 **/
bool CGoAuthIPCollection::authenticate( unsigned long cRequest, unsigned int cIdx, const char * pszIP, const char * pszUrl, const char * pszQry, char * pszCookieData, CGoAuthResponse & r, bool bVerbose ) 
{
	CPremiumArea * pPa = 0;
	CPremiumArea * pPaBest = 0;
	CIP * pIp = 0;

	char szBuffer[4097] = {'\0'};
	char szUrl[257]     = {'\0'};
	char szLog[1025]    = {'\0'};
	

	unsigned int cUrl;
	unsigned int cCookie;
	

	bool bEvaluateTokens;


	try
	{

		if ( true == bVerbose )
		{
			sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) Status: 01", cRequest );
			CLog::log( szLog );
		}



		/** Get a copy of the Url */
		cUrl = strlen( pszUrl );
		memset(szUrl, '\0', 257);
	
		if ( cUrl <= 256 )
			//sprintf( szUrl, "%s", pszUrl );
			strncpy(szUrl, pszUrl, cUrl + 1);
		else
			memcpy( szUrl, pszUrl, 256 );    

	
		/** Decode it and verify it's format */
		URLDecodeString( szUrl );
		cUrl = URLVerifyFormat( szUrl );
	
	
		/** Look at the correct IP information */
		pIp = m_IP[ cIdx ];
		
		if ( 0 == pIp )
		{
			if ( true == bVerbose )
			{
				sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) Null pIp pointer.", cRequest );
				CLog::log( szLog );
			}

			r.m_cStatus = GO_AUTH_REQUEST_CONTINUE;
			return false;
		}


		/** Try to find the premium area with best match Url */
		if ( false == pIp->findBestPremiumAreaMatch( szUrl, cUrl, &pPa ) )
		{
			
			if ( true == bVerbose )
			{
				sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) End request (not premium).", cRequest );
				CLog::log( szLog );
			}

			/** No match - it's not a premium area request */
		
			r.m_cUserStatus |= GO_USER_NOT_PREMIUM_REQUEST; 
			r.m_cStatus = GO_AUTH_REQUEST_CONTINUE;
			return true;
		}

	
		/** Make sure it's not a Free Area request */
		if ( true == pIp->isFreeAreaUrl( szUrl ) )
		{

			if ( true == bVerbose )
			{
				sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) End request (is free area).", cRequest );
				CLog::log( szLog );
			}

			r.m_cUserStatus |= GO_USER_FREE_REQUEST; 
			r.m_cStatus = GO_AUTH_REQUEST_CONTINUE;
			return true;
		}


		if ( true == bVerbose )
		{
			sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) Status: 02", cRequest );
			CLog::log( szLog );
		}


		/** A premium area was found...see if the user has a GREEN */
		r.m_cUserStatus |= GO_USER_PREMIUM_REQUEST; 


		cCookie = strlen( pszCookieData );
		memset( szBuffer, '\0', 4097 );

		if ( cCookie<4096 ) 
			strcat( szBuffer, pszCookieData );
		else
			memcpy( szBuffer, pszCookieData, 4096 );


		// Look for the SWID

		bool registeredUser = parseForSwid(szBuffer);

		// The user doesn't have a SWID or SWID with curly brace

		if (registeredUser == false)
		{
			memset( szLog, '\0', 1025 );
			if ( true == bVerbose )
			{
			   sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) Unregistered user. ", cRequest );
			   CLog::log(szLog);
			}

			memset( szLog, '\0', 1025 );
			returnSignupUser( cRequest, cIdx, szLog, pPa, pszUrl, pszQry, r, bVerbose );
			return true;
		}

		CGreen green( szBuffer );
		if ( true == bVerbose )
		{
		   sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) Cookie data: %s", cRequest, szBuffer );
		   CLog::log(szLog);
		}
	
		/** The user doesn't have a GREEN or it is not formatted correctly */
		/** Valid GREEN is not determined by email availability */
		//if ( false == green.isValid() )
		if ( green.hasEmail() == false )
		{
			memset( szLog, '\0', 1025 );

			if ( true == green.hasWarnings() )
			{
				green.getWarningInfo( szLog, 128 );
			}
		
			if ( true == green.hasErrors() )
			{
				green.getErrorInfo( szUrl, 128 );
			}
			
			strcat( szLog, szUrl );

			this->returnUserHasNoGreen( cRequest, cIdx, szLog, pPa, pszUrl, pszQry, r, bVerbose );
			return true;
		}

		/** Check if the user has the required data in the GREEN */

		unsigned int cDataMask = 0;

		if (pPa->hasRequiredData())
			cDataMask = pPa->getRequiredDataMask();
		else
			cDataMask = pIp->getDefaultRequiredDataMask();

        if (green.hasFieldMask(cDataMask) == false)
		{
			memset( szLog, '\0', 1025 );
			memset( szUrl, '\0', 257 );

			this->returnUserAccessDenied( cRequest, cIdx, szLog, pPa, pszUrl, pszQry, r, bVerbose );
			return true;
		}

		/** Retrieve a comma-delimited list of user tokens */
		memset( szBuffer, '\0', 4097 );
		green.getUserTokens( szBuffer, 512 );
	
		if ( true == bVerbose )
		{
			sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) Status: 03", cRequest );
			CLog::log( szLog );
		}

		/** Set the converted expression... */
		m_Interpreter.setExpr( pPa->getTokenExpr() );
		bEvaluateTokens = m_Interpreter.evaluate( szBuffer, cRequest );

		if ( true == m_Interpreter.hasError() )
		{
			if ( true == bVerbose )
			{
				sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) Interpreter error. ", cRequest );
				if ( m_Interpreter.hasInvalidExprError()   ) strcat( szLog, "{invalid expr}"   );
				if ( m_Interpreter.hasMissingExprError()   ) strcat( szLog, "{missing expr}"   );
				if ( m_Interpreter.hasMissingTokensError() ) strcat( szLog, "{missing tokens}" );
				if ( m_Interpreter.hasIllegalCharError()   ) strcat( szLog, "{illegal char}"   );
				if ( m_Interpreter.hasOverflowError()      ) strcat( szLog, "{overflow}"       );
				CLog::log( szLog );
			}
		}


		if ( true == bVerbose )
		{
			sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) Status: 04", cRequest );
			CLog::log( szLog );
		}


		/** The user does not have the correct tokens for the area... upsell them. */
		if ( false == bEvaluateTokens )
		{
			memset( szLog, '\0', 1025 );

			if ( true == m_Interpreter.failedOnNOT() )
			{
				this->returnUserAccessDenied( cRequest, cIdx, szLog, pPa, pszUrl, pszQry, r, bVerbose );
				return true;
			}
			else
			{
				this->returnUpsellTheUser( cRequest, cIdx, szLog, pPa, pszUrl, pszQry, r, bVerbose );
				return true;
			}
		
		}


		if ( true == bVerbose )
		{
			sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) Status: 05", cRequest );
			CLog::log( szLog );
		}

		/** The user has the correct tokens to gain acces to the area.. */
		/** check if the LG bit is set correctly */

		/** Get the LG bit from the user's GREEN */
		memset( szBuffer, '\0', 4097 );
		green.getLogId( szBuffer, 2 );

		/** Get the active login requirement */

		bool bActiveLoginRequired = true;

		if (pPa->hasActiveLoginRequired())
		{
			bActiveLoginRequired = pPa->getActiveLoginRequired();
		}
		else
		{
			bActiveLoginRequired = pIp->getDefaultActiveLoginRequired();
		}

		/** Check if the user has LG bit set correctly */

		bool bEvaluateLGBit = false;

		if (bActiveLoginRequired == true)
		{
			if (szBuffer[0] == '1')
				bEvaluateLGBit = true;
		}
		else
		{
			/** Always grant access if active login is not required */
			bEvaluateLGBit = true;
		}

		/** The user has LG bit set correctly */
		if (bEvaluateLGBit == true)
		{
			if ( true == bVerbose )
			{
				sprintf( szLog, "CGoAuthIPColl::authen> (%06ld) End request. User allowed to enter premium area", cRequest );
				CLog::log( szLog );
			}

			// Check for caching flag and set the correct HTTP header
			// before letting the user in

			bool bCacheEnabled = false;

			if (pPa->hasCachingTag())
			{
				bCacheEnabled = pPa->getCaching();
			}
			else
			{
				bCacheEnabled = pIp->getDefaultCaching();
			}

			if (bCacheEnabled == false)
			{
				r.addResponseHeader(g_pszCacheControlHeader);
			}

			// Set the status in the response

			r.m_cUserStatus |= GO_USER_GRANT_ACCESS;
			r.m_cStatus = GO_AUTH_REQUEST_CONTINUE;

			return true;
		}
		/** The user does not have the LG bit set correctly.. */
		/** send them to login page */
		else
		{
			memset( szLog, '\0', 1025 );

			this->returnUserHasNoGreen( cRequest, cIdx, szLog, pPa, pszUrl, pszQry, r, bVerbose );
			return true;
		}

	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::authenticate> Exception!");
		return false;
	}
}





/** *****************************************************************
 *
 * CGoAuthIPCollection::returnUserHasNoGreen
 *
 * Log an error message and send the user an HTTP 302 to the login 
 * page.
 *
 * Return true
 *
 **/
bool CGoAuthIPCollection::returnUserHasNoGreen( unsigned long cRequest, unsigned int cIdx, char * pszErr, CPremiumArea * pPa, const char * pszUrl, const char * pszQry, CGoAuthResponse & r, bool bVerbose ) const
{
	char szLog[1601] = {'\0'};
	char szMsg[1501] = {'\0'};
	CIP * pIp = 0;

	
	try
	{
		/** Look at the correct IP information */
		pIp = m_IP[ cIdx ];

		if ( true == bVerbose )
		{
			memset( szMsg, '\0', 1501 );
			sprintf( szMsg, "CGoAuthIPColl::return> (%06ld) End request. (Log In). Reason [%s]", cRequest, pszErr );
			CLog::log( szMsg );
		}

		/** Override the PAs login Url if necessary by sending the IPs default... */
		memset( szMsg, '\0', 1501 );
		pIp->formatRedirectUrl( szMsg, 1500, pszQry, pPa->getAbsoluteLoginPageUrl(), pIp->getDefaultAbsoluteLoginPageUrl(), pszUrl );
	
		if ( true == bVerbose )
		{
#ifdef __OS_WINNT__
			// NT sprintf gets terribly confused by '%' chars, so unescape log message
			char szTempMsgBuf[1501];
			memset(szTempMsgBuf, 0, 1501);
			strncpy(szTempMsgBuf, szMsg, 1500);
			URLDecodeString(szTempMsgBuf);
			sprintf( szLog, "CGoAuthIPColl::return> (%06ld)> Redirect Url (unencoded) [%s]", cRequest, szTempMsgBuf );


#else // ! __OS_WINNT_
			sprintf( szLog, "CGoAuthIPColl::return> (%06ld)> Redirect Url [%s]", cRequest, szMsg );
			
#endif // __OS_WINNT__
			CLog::log( szLog );
		}

		r.addResponseHeader( szMsg );
		r.m_cUserStatus |= GO_USER_MUST_LOGIN; 
		r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;

		return true;
	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::returnUserHasNoGreen> Exception!");
		return false;
	}
}





/** ***************************************************************** 
 *
 *
 * Redirect a user that needs to be upsold...
 *
 **/
bool CGoAuthIPCollection::returnUpsellTheUser( unsigned long cRequest, unsigned int cIdx, char * pszErr, CPremiumArea * pPa, const char * pszUrl, const char * pszQry, CGoAuthResponse & r, bool bVerbose ) const
{
	char szMsg[1501] = {'\0'};
	char szLog[1601] = {'\0'};
	CIP * pIp = 0;


	try
	{
		/** Look at the correct IP information */
		pIp = m_IP[ cIdx ];

		if ( true == bVerbose )
		{
			memset( szMsg, '\0', 1501 );
			sprintf( szMsg, "CGoAuthIPColl::return> (%06ld) End request. (Upsold). User rebuffed from premium area. Reason [%s]", cRequest, pszErr );
			CLog::log( szMsg );
		}

		/** Override the PAs login Url if necessary by sending the IPs default... */
		memset( szMsg, '\0', 1501 );
		pIp->formatRedirectUrl( szMsg, 1500, pszQry, pPa->getAbsoluteUpsellPageUrl(), pIp->getDefaultAbsoluteUpsellPageUrl(), pszUrl );
	
		if ( true == bVerbose )
		{
#ifdef __OS_WINNT__
			// NT sprintf gets terribly confused by '%' chars, so unescape log message
			char szTempMsgBuf[1501];
			memset(szTempMsgBuf, 0, 1501);
			strncpy(szTempMsgBuf, szMsg, 1500);
			URLDecodeString(szTempMsgBuf);
			sprintf( szLog, "CGoAuthIPColl::return> (%06ld)> Redirect Url (unencoded) [%s]", cRequest, szTempMsgBuf );

#else // ! __OS_WINNT_
			sprintf( szLog, "CGoAuthIPColl::return> (%06ld)> Redirect Url [%s]", cRequest, szMsg );
			
#endif // __OS_WINNT__

			CLog::log( szLog );
		}

		r.addResponseHeader( szMsg );
		r.m_cUserStatus |= GO_USER_UPSOLD;
		r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
	
		return true;
	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::returnUpsellTheUser> Exception!");
		return false;
	}
}





/** *****************************************************************
 *
 * CGoAuthIPCollection::returnUserAccessDenied
 *
 **/
bool CGoAuthIPCollection::returnUserAccessDenied(unsigned long cRequest, unsigned int cIdx, char * pszErr, CPremiumArea * pPa, const char * pszUrl, const char * pszQry, CGoAuthResponse & r, bool bVerbose ) const
{
	char szMsg[1501] = {'\0'};
	char szLog[1601] = {'\0'};
	CIP * pIp = 0;


	try
	{
		/** Look at the correct IP information */
		pIp = m_IP[ cIdx ];

		if ( true == bVerbose )
		{
			memset( szMsg, '\0', 1501 );
			sprintf( szMsg, "CGoAuthIPColl::return> (%06ld) End request. (AccessDenied). User rebuffed from premium area. Reason [%s]", cRequest, pszErr );
			CLog::log( szMsg );
		}

		/** Override the PAs login Url if necessary by sending the IPs default... */
		memset( szMsg, '\0', 1501 );
		pIp->formatRedirectUrl( szMsg, 1500, pszQry, pPa->getAbsoluteAccessDeniedPageUrl(), pIp->getDefaultAbsoluteAccessDeniedPageUrl(), pszUrl );
	
		if ( true == bVerbose )
		{
#ifdef __OS_WINNT__
			// NT sprintf gets terribly confused by '%' chars, so unescape log message
			char szTempMsgBuf[1501];
			memset(szTempMsgBuf, 0, 1501);
			strncpy(szTempMsgBuf, szMsg, 1500);
			URLDecodeString(szTempMsgBuf);
			sprintf( szLog, "CGoAuthIPColl::return> (%06ld)> Redirect Url (unencoded) [%s]", cRequest, szTempMsgBuf );

#else // ! __OS_WINNT_
			sprintf( szLog, "CGoAuthIPColl::return> (%06ld)> Redirect Url [%s]", cRequest, szMsg );
			
#endif // __OS_WINNT__

			CLog::log( szLog );
		}

		r.addResponseHeader( szMsg );
		r.m_cUserStatus |= GO_USER_ACCESS_DENIED;
		r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
	
		return true;
	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::returnUserAccessDenied> Exception!");
		return false;
	}
}


/** *****************************************************************
 *
 * CGoAuthIPCollection::returnSignupUser
 *
 **/
bool CGoAuthIPCollection::returnSignupUser(unsigned long cRequest, unsigned int cIdx, char * pszErr, CPremiumArea * pPa, const char * pszUrl, const char * pszQry, CGoAuthResponse & r, bool bVerbose ) const
{
	char szMsg[1501] = {'\0'};
	char szLog[1601] = {'\0'};
	CIP * pIp = 0;


	try
	{
		/** Look at the correct IP information */
		pIp = m_IP[ cIdx ];

		if ( true == bVerbose )
		{
			memset( szMsg, '\0', 1501 );
			sprintf( szMsg, "CGoAuthIPColl::return> (%06ld) End request. (Signup). User rebuffed from premium area. Reason [%s]", cRequest, pszErr );
			CLog::log( szMsg );
		}

		/** Override the PAs signup Url if necessary by sending the IPs default... */
		memset( szMsg, '\0', 1501 );
		pIp->formatRedirectUrl( szMsg, 1500, pszQry, pPa->getAbsoluteSignupPageUrl(), pIp->getDefaultAbsoluteSignupPageUrl(), pszUrl );
	
		if ( true == bVerbose )
		{
#ifdef __OS_WINNT__
			// NT sprintf gets terribly confused by '%' chars, so unescape log message
			char szTempMsgBuf[1501];
			memset(szTempMsgBuf, 0, 1501);
			strncpy(szTempMsgBuf, szMsg, 1500);
			URLDecodeString(szTempMsgBuf);
			sprintf( szLog, "CGoAuthIPColl::return> (%06ld)> Redirect Url (unencoded) [%s]", cRequest, szTempMsgBuf );

#else // ! __OS_WINNT_
			sprintf( szLog, "CGoAuthIPColl::return> (%06ld)> Redirect Url [%s]", cRequest, szMsg );
			
#endif // __OS_WINNT__

			CLog::log( szLog );
		}

		r.addResponseHeader( szMsg );
		r.m_cUserStatus |= GO_USER_MUST_SIGNUP;
		r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
	
		return true;
	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::returnSignupUser> Exception!");
		return false;
	}
}


/** *****************************************************************
 *
 * CGoAuthIPCollection::containsRequestIP
 *
 **/
bool CGoAuthIPCollection::containsRequestIP( const char * pszIP, int * pIdx ) const
{
	if ( 0==pszIP ) return false;
	if ( 0 != pIdx ) { (*pIdx) = -1; }
	
	try
	{
	
		for ( unsigned int i=0; i<m_cIP; i++ )
		{
			if ( 0 == strcmp( pszIP, m_pszIP[i] ) )
			{
				if ( 0 != pIdx ) { (*pIdx) = (int)i; }
				return true;
			}
		}

		return false;

	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::containsRequestIP> Exception!");
		return false;
	}

}




/** *****************************************************************
 *
 * CGoAuthIPCollection::CGoAuthIPCollection
 *
 **/
CGoAuthIPCollection::CGoAuthIPCollection()
{
	m_bInitialized = false;
	m_cIP = 0;

	for ( int i=0; i<g_cMaxIPs; i++ )
	{
		m_IP[i] = 0;
		m_pszIP[i] = 0;
	}

	return;
}



/** *****************************************************************
 *
 * CGoAuthIPCollection::~CGoAuthIPCollection
 *
 **/
CGoAuthIPCollection::~CGoAuthIPCollection()
{
	this->destroy();
	return;
}



/** *****************************************************************
 *
 * CGoAuthIPCollection::destroy
 *
 **/
bool CGoAuthIPCollection::destroy()
{
	try
	{
		m_bInitialized = false;

		for ( unsigned int i=0; i<m_cIP; i++ )
		{
			if ( 0 != m_IP[i] )
			{
				delete m_IP[i];
			}

			if ( 0 != m_pszIP[i] )
			{
				delete [] m_pszIP[i];
			}

			m_pszIP[i] = 0;
			m_IP[i] = 0;
		}

		m_cIP = 0;
		return true;

	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::destroy> Exception!");
		return false;
	}
}




/** *****************************************************************
 *
 * CGoAuthIPCollection::init
 *
 * Given an .ini filename,  open it up and read out all of the 
 * IP addresses in it.  For each IP Address,  instantiate an XML 
 * parser and parse out the input file.  If the parse is successful, 
 * convert the XML to an instance of CIP and add it, along with 
 * the IP address,  to the array of IP addresses.
 *
 * Then destroy the XML parse hierarchy.
 *
 * Returns true if even 1 IP address was found to authenticate 
 * against, false if not.
 *
 **/
bool CGoAuthIPCollection::init( const char * pszDir,  const char * pszIni, bool * bVerbose )
{
	bool bSuccess = false;
	unsigned int cLen;

	char szFile[300] = {'\0'};
	char szErr[500]  = {'\0'};
	char szLine[350] = {'\0'};
	char szRunMode[33] = {'\0'};


	if ( 0==pszDir || 0==pszIni )
		return false;

	try
	{

		/** Generate the .ini filename */
		cLen = strlen( pszDir) + strlen(g_pszFileDelim) + strlen(pszIni) + 1;

		if ( cLen<290 )
		{
			sprintf( szFile, "%s%s%s", pszDir, g_pszFileDelim, pszIni );
		}
		else
		{
			sprintf ( szErr, "Could not open .ini file [%s] (too long)", pszIni );
			CLog::log( szErr );
			return false;
		}

		/** Open the input file stream */
		fstream fin( szFile, ios::in | ios::nocreate );


		/** Could not open .ini file */
		#ifdef __OS_WINNT__
		if ( !fin.is_open() )
		#elif defined __OS_SOLARIS__
		if ( !fin )
		#endif
		{
			sprintf ( szErr, "Could not open .ini file [%s]", szFile );
			CLog::log( szErr );
			return false;
		}

		CLog::log(" ");
		CLog::log("         <Begin initialization> ");
		CLog::log(" ");

	
		/** Read in a line at a time */
		while ( fin.getline( szLine, 349 ) )
		{
			
			trim( szLine );

			cLen = strlen( szLine );
			
			if ( 0 == cLen ) continue;
			
			
			if ( cLen<33 )
			{
				for ( unsigned int i=0; i<cLen; i++ )
				{
					szRunMode[i] = toupper( szLine[i] );
				}
				
				/** Check for run mode first */
				if ( 0==strncmp( szRunMode, "RUNMODE=VERBOSE", 15 ) )
				{
					(*bVerbose) = true;
				}
				/** ..then check it again... */
				else
				if ( 0==strncmp( szRunMode, "RUNMODE=SILENT", 14 ) )
				{
					(*bVerbose) = false;
				}
				/** Then check if it's a parseable IP address */
				else
				if( true == parseIPAddress( pszDir, szLine, szErr ) )
				{
					/** One successful IP indicates a success... */
					bSuccess = true;
				}
			

			}		
		}

		fin.close();


		if ( 0 == m_cIP )
		{
			CLog::log( "No valid IP addresses/XML files were found for initialization" );
			CLog::log( "No authentication will occur for any IP addresses..." );
		}


		CLog::log(" ");
		CLog::log("          <End initialization> ");
		CLog::log(" ");
	
		return bSuccess;
	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::init> Exception!");
		return false;
	}
}





/** *****************************************************************
 * 
 * CGoAuthIPCollection::parseIPAddress
 *
 * Given an IP Address,  open it's .ini file and parse out the XML.
 *
 * If successful,  convert the XML hierarchy to an instance of CIP 
 * with a set of instances of CPremiumArea added to it.  Save the 
 * IP address and use it as a key to determine if an incoming Url 
 * Request is for a given IP address.
 *
 * Returns true the XML plus the conversion to an instance of CIP 
 * were both successful,  false if not.
 *
 **/
bool CGoAuthIPCollection::parseIPAddress( const char * pszDir, const char * pszIP, char * pszErr )
{
	char szIP[129] = {'\0'};
	char szFile[300] = {'\0'};
	unsigned int cLen;
	CXMLNode * p = 0;
	bool bConvert = false;

	try
	{
	
		cLen = strlen(pszIP );
		memset(szIP, '\0', 129);

		/** Sometimes,  in UNIX,  we're stuck with an extra char at the end.. */
		if ( cLen < 128 )
		{
			//sprintf( szIP, "%s", pszIP );
			strncpy(szIP, pszIP, cLen + 1);
	
			if ( szIP[ cLen-1 ] == 10 )
				szIP[cLen-1] = '\0';
			else if ( szIP[ cLen-1 ] == 13 )
				szIP[cLen-1] = '\0';
			else if ( szIP[ cLen-1 ] == 15 )
				szIP[cLen-1] = '\0';
		}
		else
		{
			memcpy( szIP, pszIP, 128 );
			memset( pszErr, '\0', 500 );
			sprintf (pszErr, "Error opening .ini file for IP address [%s] (filename too long)", szIP );
			CLog::log( pszErr );
			CLog::log(" ");
			return false;
		}


		sprintf( szFile, "%s.ini.xml", szIP );


		/** Success! */
		if ( true == m_Parser.parse( pszDir, szFile ) )
		{
			sprintf ( pszErr, "Successful XML parse of .ini file [%s] in dir [%s]", szFile, pszDir );
			CLog::log( pszErr );

			p = m_Parser.copyRoot();

			bConvert = xml2IP( szIP, p );
		
			m_Parser.cleanUp();

			if ( true == bConvert )
			{
				memset( pszErr, '\0', 500 );
				sprintf ( pszErr, "Successful conversion of .ini file [%s] for authentication", szFile );
				CLog::log( pszErr );
				memset( pszErr, '\0', 500 );
				//sprintf ( pszErr, "This IP address will have authentication");
				strcpy( pszErr, "This IP address will have authentication");
				CLog::log( pszErr );
				CLog::log(" ");
				return true;
			}
			else
			{
				memset( pszErr, '\0', 500 );
				sprintf ( pszErr, "Unsuccessful conversion of .ini file [%s] for authentication", szFile );
				CLog::log( pszErr );
				memset( pszErr, '\0', 500 );
				//sprintf ( pszErr, "This IP address WILL NOT have authentication");
				strcpy( pszErr, "This IP address WILL NOT have authentication");
				CLog::log( pszErr );
				CLog::log(" ");
				return true;
			}

		}
		/** Failed! */
		else
		{
			memset( pszErr, '\0', 500 );
			sprintf ( pszErr, "Unsuccessful XML parse of .ini file [%s] in dir [%s]", szFile, pszDir );
			CLog::log( pszErr );
			memset( pszErr, '\0', 500 );
			//sprintf ( pszErr, "This IP address WILL NOT have authentication");
			strcpy( pszErr, "This IP address WILL NOT have authentication");
			CLog::log( pszErr );

			/** Log the errors */
			cLen = m_Parser.getErrorCount();
			for ( unsigned int j=0; j<cLen; j++ )
			{
				sprintf( pszErr, "XML Error: [%s]", m_Parser.getError(j) );
				CLog::log( pszErr );
			}

			CLog::log(" ");
			return false;
		} 

	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::parseIPAddress> Exception!");
		return false;

	}
}


/** *****************************************************************
 * 
 * CGoAuthIPCollection::parseForSwid
 *
 * Given the cookie string, look for a registered SWID (SWID with
 * curly brace).
 *
 * Returns true if registered SWID is found, false if not. 
 *
 **/
 bool CGoAuthIPCollection::parseForSwid( const char * pszCookie )
{
	 if (pszCookie == 0)
		 return false;

	 const char *pszStart = 0;
	 const char *pszEnd = 0;

	 char *pszTemp = strstr(pszCookie, "SWID=");
	 if (pszTemp == 0)
		 return false;

	 pszStart = strchr(pszTemp, '=');
     pszStart += 1;

	 pszEnd = strchr(pszStart, ';');
	 if (pszEnd == 0)
	 {
		 // End of the cookie string

		 pszEnd = pszCookie + strlen(pszCookie) - 1;
	 }
	 else
	 {
		 // Not end of the cookie string

	     pszEnd -= 1;
	 }

	 if ((*pszStart != '{') || (*pszEnd != '}'))
		 return false;

	 return true;
}


/** *****************************************************************
 *
 * CGoAuthIPCollection::xml2IP
 *
 * Given an XML parse tree,  convert this to an IP with it's 
 * corresponding Premium Areas.
 *
 * Return true if all of the information is present to instantiate 
 * an IP and at least 1 premium area -- false if not.
 *
 **/
bool CGoAuthIPCollection::xml2IP( const char * pszIP, const CXMLNode * p )
{
	char   szName[129] = {'\0'};
	char   szLame[129] = {'\0'};
	char   szHost[129] = {'\0'};
	char   szPort[129] = {'\0'};
	char szDomain[129] = {'\0'};
	char   szLUrl[128] = {'\0'};
	char   szUUrl[128] = {'\0'};
	char   szAUrl[128] = {'\0'};
	char   szSUrl[128] = {'\0'};
	char   szActiveLoginRequired[128] = {'\0'};
	char   szCaching[128] = {'\0'};
	char   szRequiredData[128] = {'\0'};

	char  szFreeArea[129] = {'\0'};

	char  szPAName[129] = {'\0'};
	char szPARUrls[513] = {'\0'};
	char  szPATReq[513] = {'\0'};
	char  szPATOrigReq[513] = {'\0'};
	char  szPALUrl[129] = {'\0'};
	char  szPAUUrl[129] = {'\0'};
	char  szPAAUrl[129] = {'\0'};
	char  szPASUrl[129] = {'\0'};
	char  szPAActiveLoginRequired[128] = {'\0'};
    char  szPACaching[128] = {'\0'};
	char  szPARequiredData[128] = {'\0'};

	int cDataMask = 0;

	CXMLNode * child = 0;
	CIP * pIp = 0;

	char  szTemp[128] = {'\0'};
	char * psz = 0;
	unsigned int cLen;
	unsigned int cPremArea = 0;
	unsigned int cFreeArea = 0;
	unsigned int cValidPremArea = 0;

	char  szErr1[256] = {'\0'};
	char  szErr[4096] = {'\0'};
	char szWarn[4096] = {'\0'};
	
	bool bErrOverflow  = false;
	bool bWarnOverflow = false;


	try
	{
	
		/** Find the root tag - <Service>...if it is missing it is a fatal error */
		if ( 0 != strcmp( g_pszXMLTag_Service, p->getName() ) )
		{
			sprintf( szErr1, "\tMissing root key <%s>\r\n", g_pszXMLTag_Service  );
			strcat( szErr, szErr1 );
		}


		/** Get the name - if it is missing it is only a warning */
		if ( false == p->hasProperty( g_pszXMLTag_Name, szName, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>\r\n", g_pszXMLTag_Name );
			strcat( szWarn, szErr1 );
		}


		/** Get the Lame Browser Url...if it is missing it is a fatal error */
		if ( false == p->hasProperty( g_pszXMLTag_LameBrowserUrl, szLame, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>\r\n", g_pszXMLTag_LameBrowserUrl );
			strcat( szErr, szErr1 );
		}


		/** Get the Host...if it is missing it is a fatal error */
		if ( false == p->hasProperty( g_pszXMLTag_Host, szHost, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>\r\n", g_pszXMLTag_Host );
			strcat( szErr, szErr1 );
		}

		/** Get the PORT...if it is missing it is a fatal error */
		if ( false == p->hasProperty( g_pszXMLTag_Port, szPort, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>\r\n", g_pszXMLTag_Port);
			strcat( szErr, szErr1 );
		}

		/** Get the Domain Name...if it is missing it is a fatal error */
		if ( false == p->hasProperty( g_pszXMLTag_DomainName, szDomain, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>\r\n", g_pszXMLTag_DomainName );
			strcat( szErr, szErr1 );
		}

	
		/** Get the DefaultLoginPageUrl...if it is missing it is a fatal error */
		if ( false == p->hasProperty( g_pszXMLTag_DefaultLoginPageUrl, szLUrl, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>\r\n", g_pszXMLTag_DefaultLoginPageUrl );
			strcat( szErr, szErr1 );
		}


		/** Get the DefaultUpsellPageUrl...if it is missing it is a fatal error */
		if ( false == p->hasProperty( g_pszXMLTag_DefaultUpsellPageUrl, szUUrl, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>\r\n", g_pszXMLTag_DefaultUpsellPageUrl );
			strcat( szErr, szErr1 );
		}


		/** Get the DefaultAccessDeniedPageUrl...if it is missing it is a fatal error */
		if ( false == p->hasProperty( g_pszXMLTag_DefaultAccessDeniedPageUrl, szAUrl, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>\r\n", g_pszXMLTag_DefaultAccessDeniedPageUrl );
			strcat( szErr, szErr1 );
		}

		/** Get the DefaultSignupPageUrl...if it is missing it is only a warning */
		if ( false == p->hasProperty( g_pszXMLTag_DefaultSignupPageUrl, szSUrl, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>, default to <%s>\r\n", g_pszXMLTag_DefaultSignupPageUrl, g_pszXMLTag_DefaultLoginPageUrl );
			strcat( szWarn, szErr1 );

			// Default to login URL

			memset(szSUrl, '\0', 128);
            strncpy(szSUrl, szLUrl, strlen(szLUrl));
		}

		/** Get the DefaultActiveLogin - if it is missing it is only a warning */
		if ( false == p->hasProperty( g_pszXMLTag_DefaultActiveLoginRequired, szTemp, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>, default to TRUE\r\n", g_pszXMLTag_DefaultActiveLoginRequired );
			strcat( szWarn, szErr1 );
		}
		else
		{
			// Trim leading and trailing spaces for the DefaultActiveLogin value

			strtrim(szTemp, szActiveLoginRequired);

			// Check if the value is valid

			for (unsigned int idx = 0; idx < strlen(szActiveLoginRequired); idx++)
			{
				szActiveLoginRequired[idx] = toupper(szActiveLoginRequired[idx]);
			}

			if ( (strncmp(szActiveLoginRequired, "FALSE", 5) != 0) &&
				 (strncmp(szActiveLoginRequired, "TRUE", 4) != 0) )
			{
				sprintf(szErr1, "\tInvalid value for <%s> (%s), default to TRUE\r\n",
					    g_pszXMLTag_DefaultActiveLoginRequired, szActiveLoginRequired);
				strcat(szWarn, szErr1);
			}
		}
		memset(szTemp, '\0', 128);

		/** Get the DefaultCaching - if it is missing it is only a warning */
		if ( false == p->hasProperty( g_pszXMLTag_DefaultCaching, szTemp, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>, default to FALSE\r\n", g_pszXMLTag_DefaultCaching );
			strcat( szWarn, szErr1 );
		}
		else
		{
			// Trim leading and trailing spaces for the DefaultCaching value

			strtrim(szTemp, szCaching);

			// Check if the value is valid

			for (unsigned int idx = 0; idx < strlen(szCaching); idx++)
			{
				szCaching[idx] = toupper(szCaching[idx]);
			}

			if ( (strncmp(szCaching, "FALSE", 5) != 0) &&
				 (strncmp(szCaching, "TRUE", 4) != 0) )
			{
				sprintf(szErr1, "\tInvalid value for <%s> (%s), default to FALSE\r\n",
					    g_pszXMLTag_DefaultCaching, szCaching);
				strcat(szWarn, szErr1);
			}
		}
		memset(szTemp, '\0', 128);

		/** Get the DefaultRequiredData - if it is missing it is only a warning */
		if ( false == p->hasProperty( g_pszXMLTag_DefaultRequiredData, szTemp, 128 ) )
		{
			sprintf( szErr1, "\tMissing name/value key <%s>, default to no data is required\r\n", g_pszXMLTag_DefaultRequiredData );
			strcat( szWarn, szErr1 );
			cDataMask = 0;
		}
		else
		{
            // Trim leading and trailing spaces

			strtrim(szTemp, szRequiredData);

			// Check if the value is valid, and get the correct mask

			if ((cDataMask = checkRequiredData(szRequiredData)) == -1)
			{
				// Error

				sprintf(szErr1, "\tInvalid value for <%s> (%s), default to no data is required\r\n",
					    g_pszXMLTag_DefaultRequiredData, szRequiredData);
				strcat(szWarn, szErr1);

				memset(szRequiredData, '\0', 128);
				cDataMask = 0;
			}
		}
		memset(szTemp, '\0', 128);

		/** Get a count of existing premium areas */
		if ( false == p->childExists(g_pszXMLTag_PremiumArea, &cPremArea ) )
		{
			sprintf( szErr1, "\tNo premium areas found...\r\n" );

			if ( (strlen(szErr) + strlen(szErr1)) < 4096 )
				strcat( szErr, szErr1 );
			else
				bErrOverflow = true;
		}
		else
		{

			/** Get a count of the valid Premium Areas */
			for ( unsigned int j=0; j<cPremArea; j++ )
			{
				p->getChild( g_pszXMLTag_PremiumArea, j, &child );
		
				if ( true == child->hasProperty( g_pszXMLTag_RequestUrls, szPARUrls, 512 ) )
				{
					if ( true == child->hasProperty( g_pszXMLTag_TokensRequired, szPATReq, 512 ) )
					{
						cValidPremArea++;
					}
				}
		
				memset( szPARUrls, '\0', 513 );
				memset( szPATReq,  '\0', 513 );
			}
		}


		/** If there are no errors,  and there is at least 1 premium area, continue */
		if ( 0!=strlen(szErr) || (cValidPremArea==0 ) )
		{
			sprintf( szPARUrls, "The following FATAL errors were found in parsing IP address [%s]", pszIP );
			CLog::log( szPARUrls );

			psz = 0;
			psz = strtok( szErr, "\r\n" );
			while ( 0 != psz )
			{
				CLog::log( psz );
				psz = strtok( 0, "\r\n" );
			}

			if ( 0 != strlen(szWarn) ) 
			{
				sprintf( szPARUrls, "The following warnings were found in parsing IP address [%s]", pszIP );
				CLog::log( szPARUrls );

				psz = 0;
				psz = strtok( szWarn, "\r\n" );
				while ( 0 != psz )
				{
					CLog::log( psz );
					psz = strtok( 0, "\r\n" );
				}
			}

			return false;
		}

		/** Check the values of the IP before creating it */
		this->checkVals( szHost, szLame, szLUrl, szUUrl, szAUrl, szSUrl, szDomain );

		/** Add a new IP address to the collection */
		pIp = new CIP();
		if ( 0 == pIp ) return false;
		pIp->init( szName, szHost, szPort, szLame, szLUrl, szUUrl, szAUrl, szSUrl, szDomain, szActiveLoginRequired,
			       szCaching, szRequiredData, cDataMask );

		/** Look for the Free Areas */
		if ( true == p->hasProperty( g_pszXMLTag_FreeArea, &cFreeArea ) )
		{
			for ( unsigned int x=0; x<cFreeArea; x++ )
			{
				if ( p->hasProperty( g_pszXMLTag_FreeArea, x, szFreeArea, 128 ) )
				{
					pIp->addFreeArea( szFreeArea );	
					memset( szFreeArea, '\0', 129 );
				}
			}
		}

		/** Enumerate over the premium areas */
		for ( unsigned int i=0; i<cPremArea; i++ )
		{
            cDataMask = 0;

			/** Get the name - if it is missing it is only a warning */
			p->getChild( g_pszXMLTag_PremiumArea, i, &child );

			if ( false == child->hasProperty( g_pszXMLTag_Name,szPAName, 128 ) )
			{
				sprintf( szErr1, "\tPremium area %d missing name\r\n", cPremArea-i );
				
				if ( (strlen(szWarn) + strlen(szErr1)) < 4096 )
					strcat( szWarn, szErr1 );
				else
					bErrOverflow = true;
			}
			
			
			/** Get the Request Urls - if it is missing this is fatal */
			if ( false == child->hasProperty( g_pszXMLTag_RequestUrls, szPARUrls, 512 ) )
			{
				sprintf( szErr1, "\tPremium area %d missing <%s>\r\n", cPremArea-i, g_pszXMLTag_RequestUrls );
				
				if ( (strlen(szErr) + strlen(szErr1)) < 4096 )
					strcat( szErr, szErr1 );
				else
					bErrOverflow = true;
			}
			

			/** Get the Tokens Required Expression... this is required. */
			if ( false == child->hasProperty( g_pszXMLTag_TokensRequired, szPATReq, 512 ) )
			{
				sprintf( szErr1, "\tPremium area %d missing <%s>\r\n", cPremArea-i, g_pszXMLTag_TokensRequired );
				
				if ( (strlen(szErr) + strlen(szErr1)) < 4096 )
					strcat( szErr, szErr1 );
				else
					bErrOverflow = true;
			
			}
			
			/** Preserve the original token expression */
			sprintf( szPATOrigReq, "%s", szPATReq);


			/** See if the Tokens Required Expression is valid */
			m_Interpreter.convertExpression( szPATReq, strlen(szPATReq ) );


			if ( m_Interpreter.hasError() )
			{
				if ( m_Interpreter.hasFatalError() )
				{
					sprintf( szErr1, "\tThe token logic for this area has the following FATAL errors: " );
					
					if ( m_Interpreter.hasInvalidExprError()   )  strcat( szErr1, "{invalid expr}"   );
					if ( m_Interpreter.hasMissingExprError()   )  strcat( szErr1, "{missing expr}"   );
					if ( m_Interpreter.hasMissingTokensError() )  strcat( szErr1, "{missing tokens}" );
					if ( m_Interpreter.hasIllegalCharError()   )  strcat( szErr1, "{illegal char}"   );
					if ( m_Interpreter.hasOverflowError()      )  strcat( szErr1, "{overflow}"       );

					strcat( szErr1, "\r\n" );
					
					if ( (strlen(szErr) + strlen(szErr1)) < 4096 )
						strcat( szErr, szErr1 );
					else
						bWarnOverflow = true;
				}
				else
				{
					sprintf( szErr1, "\tThe token logic for this area has the following warnings: ");
					
					if ( m_Interpreter.hasInvalidExprError()   )  strcat( szErr1, "{invalid expr}"   );
					if ( m_Interpreter.hasMissingExprError()   )  strcat( szErr1, "{missing expr}"   );
					if ( m_Interpreter.hasMissingTokensError() )  strcat( szErr1, "{missing tokens}" );
					if ( m_Interpreter.hasIllegalCharError()   )  strcat( szErr1, "{illegal char}"   );
					if ( m_Interpreter.hasOverflowError()      )  strcat( szErr1, "{overflow}"       );

					strcat( szErr1, "\r\n" );
					
					if ( (strlen(szWarn) + strlen(szErr1)) < 4096 )
						strcat( szWarn, szErr1 );
					else
						bWarnOverflow = true;
				}
			}

			/** Initialize Premium Area with Converted expression */
			sprintf( szPATReq, "%s", m_Interpreter.getExpr() );
			m_Interpreter.clear();


			/** Get the Login Page Url - if it is missing it is only a warning */
			if ( false == child->hasProperty( g_pszXMLTag_LoginPageUrl, szPALUrl, 128 ) )
			{
				sprintf( szErr1, "\tPremium area %d missing <%s>\r\n", cPremArea-i, g_pszXMLTag_LoginPageUrl );
				
				if ( (strlen(szWarn) + strlen(szErr1)) < 4096 )
					strcat( szWarn, szErr1 );
				else
					bWarnOverflow = true;
			}


			/** Get the Upsell Page Url - if it is missing it is only a warning */
			if ( false == child->hasProperty( g_pszXMLTag_UpsellPageUrl, szPAUUrl, 128 ) )
			{
				sprintf( szErr1, "\tPremium area %d missing <%s>\r\n", cPremArea-i, g_pszXMLTag_UpsellPageUrl );
				
				if ( (strlen(szWarn) + strlen(szErr1)) < 4096 )
					strcat( szWarn, szErr1 );
				else
					bWarnOverflow = true;
			}
			

			/** Get the Access Denied Page Url - if it is missing it is only a warning */
			if ( false == child->hasProperty( g_pszXMLTag_AccessDeniedPageUrl, szPAAUrl, 128 ) )
			{
				sprintf( szErr1, "\tPremium area %d missing <%s>\r\n", cPremArea-i, g_pszXMLTag_AccessDeniedPageUrl );

				if ( (strlen(szWarn) + strlen(szErr1)) < 4096 )
					strcat( szWarn, szErr1 );
				else
					bWarnOverflow = true;
			}

			/** Get the Signup Page Url - if it is missing it is only a warning */
			if ( false == child->hasProperty( g_pszXMLTag_SignupPageUrl, szPASUrl, 128 ) )
			{
				sprintf( szErr1, "\tPremium area %d missing <%s>\r\n", cPremArea-i, g_pszXMLTag_SignupPageUrl );

				if ( (strlen(szWarn) + strlen(szErr1)) < 4096 )
					strcat( szWarn, szErr1 );
				else
					bWarnOverflow = true;
			}

			/** Get the Active Login flag - if it is missing it is only a warning */
			if ( false == child->hasProperty( g_pszXMLTag_ActiveLoginRequired, szTemp, 128 ) )
			{
				sprintf( szErr1,
					     "\tPremium area %d missing <%s>, default to the value set in <%s>\r\n",
						 cPremArea-i, g_pszXMLTag_ActiveLoginRequired, g_pszXMLTag_DefaultActiveLoginRequired );

				if ( (strlen(szWarn) + strlen(szErr1)) < 4096 )
					strcat( szWarn, szErr1 );
				else
					bWarnOverflow = true;
			}
			else
			{
				// Trim leading and trailing spaces for the ActiveLogin value

				strtrim(szTemp, szPAActiveLoginRequired);

				// Check if the value is valid

				for (unsigned int idx = 0; idx < strlen(szPAActiveLoginRequired); idx++)
				{
					szPAActiveLoginRequired[idx] = toupper(szPAActiveLoginRequired[idx]);
				}

				if ( (strncmp(szPAActiveLoginRequired, "FALSE", 5) != 0) &&
					 (strncmp(szPAActiveLoginRequired, "TRUE", 4) != 0) )
				{
					sprintf(szErr1, "\tPremium area %d invalid value for <%s> (%s), default to TRUE\r\n",
						    cPremArea-i, g_pszXMLTag_ActiveLoginRequired, szPAActiveLoginRequired);
					strcat(szWarn, szErr1);
				}
			}

			/** Get the Caching flag - if it is missing it is only a warning */
			if ( false == child->hasProperty( g_pszXMLTag_Caching, szTemp, 128 ) )
			{
				sprintf( szErr1,
					     "\tPremium area %d missing <%s>, default to the value set in <%s>\r\n",
						 cPremArea-i, g_pszXMLTag_Caching, g_pszXMLTag_DefaultCaching );

				if ( (strlen(szWarn) + strlen(szErr1)) < 4096 )
					strcat( szWarn, szErr1 );
				else
					bWarnOverflow = true;
			}
			else
			{
				// Trim leading and trailing spaces for the Caching value

				strtrim(szTemp, szPACaching);

				// Check if the value is valid

				for (unsigned int idx = 0; idx < strlen(szPACaching); idx++)
				{
					szPACaching[idx] = toupper(szPACaching[idx]);
				}

				if ( (strncmp(szPACaching, "FALSE", 5) != 0) &&
					 (strncmp(szPACaching, "TRUE", 4) != 0) )
				{
					sprintf(szErr1, "\tPremium area %d invalid value for <%s> (%s), default to FALSE\r\n",
						    cPremArea-i, g_pszXMLTag_Caching, szPACaching);
					strcat(szWarn, szErr1);
				}
			}
			memset(szTemp, '\0', 128);

  		    /** Get the RequiredData - if it is missing it is only a warning */
		    if ( false == child->hasProperty( g_pszXMLTag_RequiredData, szTemp, 128 ) )
			{
			    sprintf( szErr1, "\tPremium area %d missing <%s>, default to the value set in <%s>\r\n",
					     cPremArea-i, g_pszXMLTag_RequiredData, g_pszXMLTag_DefaultRequiredData );
			    strcat( szWarn, szErr1 );
			    cDataMask = 0;
			}
		    else
			{
                // Trim leading and trailing spaces

			    strtrim(szTemp, szPARequiredData);

			    // Check if the value is valid, and get the correct mask

			    if ((cDataMask = checkRequiredData(szPARequiredData)) == -1)
				{ 
				    // Error

				    sprintf(szErr1, "\tPremium area %d invalid value for <%s> (%s), default to no data is required\r\n",
					        cPremArea-i, g_pszXMLTag_RequiredData, szPARequiredData);
				    strcat(szWarn, szErr1);

				    memset(szPARequiredData, '\0', 128);
				    cDataMask = 0;
				} 
			}

			/** If the Premium Area has the 2 required fields,  add it to this IP address */
			/** Add a new Premium Area to this IP address */
			if ( (strlen(szPARUrls)>0) && (strlen(szPATReq)>0) )
			{
				if ( false == pIp->addPremiumArea( szPAName, szPARUrls, szPATReq, szPATOrigReq, szPALUrl, szPAUUrl, szPAAUrl, szPASUrl, szPAActiveLoginRequired, szPACaching,
					                               szPARequiredData, cDataMask ) )
				{
					sprintf( szErr1, "\tPremium area %d overflowing Premium Areas\r\n",  cPremArea-i );
					
					if ( (strlen(szWarn) + strlen(szErr1)) < 4096 )
						strcat( szWarn, szErr1 );
					else
						bWarnOverflow = true;
				}
			
			}
	
			memset( szPAName,  '\0', 129 );
			memset( szPARUrls, '\0', 513 );
			memset( szPATReq,  '\0', 513 );
			memset( szPALUrl,  '\0', 129 );
			memset( szPAUUrl,  '\0', 129 );
			memset( szPAAUrl,  '\0', 129 );
			memset( szPASUrl,  '\0', 129 );
			memset( szPAActiveLoginRequired, '\0', 128 );
			memset( szPACaching, '\0', 128 );
			memset( szPARequiredData, '\0', 128 );
			memset( szTemp,    '\0', 128 );
		}

		/** Log errors */
		if ( 0!=strlen(szErr) )
		{
			sprintf( szPARUrls, "The following errors were found in parsing IP address [%s]", pszIP );
			CLog::log( szPARUrls );

			psz = 0;
			psz = strtok( szErr, "\r\n" );
			while ( 0 != psz )
			{
				CLog::log( psz );
				psz = strtok( 0, "\r\n" );
			}
		}

	
		/** Log warnings.... */
		if ( 0!=strlen(szWarn) )
		{
			sprintf( szPARUrls, "The following warnings were found in parsing IP address [%s]", pszIP );
			CLog::log( szPARUrls );

			psz = 0;
			psz = strtok( szWarn, "\r\n" );
			while ( 0 != psz )
			{
				CLog::log( psz );
				psz = strtok( 0, "\r\n" );
			}
		}

	
		/** Log overflow problems in displaying errors/warnings */
		if ( true == bErrOverflow )
		{
			sprintf( szErr, "Warning: Not all errors could be displayed for IP address [%s]", pszIP );
			CLog::log( szErr );
		}

		if ( true == bWarnOverflow )
		{
			sprintf( szWarn, "Warning: Not all warnings could be displayed for IP address [%s]", pszIP );
			CLog::log( szWarn );
		}
	

		/** Use the IP as the key to look up an instance of CIP() */
		cLen = strlen( pszIP ) + 1;
		psz = new char[ cLen ];
		memset( psz, '\0', cLen );
		sprintf( psz, "%s", pszIP );

		/** Add the IP adddress to the collection of IP addresses */
		m_pszIP[m_cIP] = psz;
	    m_IP[m_cIP] = pIp;
		m_cIP++;

		return true;

	}
	catch( ... )
	{
		CLog::log("CGoAuthIPCollection::xml2IP> Exception!");
		return false;
	}
}





/** *****************************************************************
 *
 * CGoAuthIPCollection::checkVals
 *
 * See if the values of this IP Address are valid.
 * We know here that all of these pointers are valid and all point 
 * to something.
 *
 *  NAME         GOOD                    BAD
 * ----------------------------------------------------------------
 *  pszHost      www.infoseek.com        http://www.infoseek.com
 *  pszLame      www.infoseek.com        http://www.infoseek.com
 *  pszLUrl      must begin with '/'
 *  pszUUrl      must begin with '/'
 *  pszAUrl      must begin with '/'
 *  pszSUrl      must begin with '/'
 *  pszDomain    <anything>
 *
 **/
bool CGoAuthIPCollection::checkVals( 
	char * pszHost, char * pszLame, char * pszLUrl, char * pszUUrl, char * pszAUrl, char * pszSUrl, char * pszDomain ) const
{
	char szTemp[8] = {'\0'};
	unsigned int cLen = 0;
	unsigned int i = 0;


	/** HOST - Make sure this value does not begin with "http://" */
	cLen = strlen( pszHost );

	for ( i=0; i<8 && i<cLen; i++ )
		{ szTemp[i] = tolower( pszHost[i] ); }

	if ( 0 == strncmp( szTemp, "http://", 7 ) )
	{
		for ( i=7; i<cLen; i++ )
			{ pszHost[i-7] = pszHost[i]; }

		pszHost[cLen-7] = '\0';			
	}

	return true;
}




/** *****************************************************************
 *
 * CGoAuthIPCollection::getConfigInfo
 *
 * Retrieve the config information for this IP collection as long 
 * as it fits within the buffer supplied.
 *
 **/
bool CGoAuthIPCollection::getConfigInfo( char * pszBuffer, unsigned int cSize ) const
{
	if ( 0==pszBuffer || cSize<=128 ) return false;

	CIP * pIp = 0;
	
	char szBuf[1025] = {'\0'};
	memset( pszBuffer, '\0', cSize );
	unsigned int cLen = 0;
	int cOpenList = 0;
	unsigned int cAreas = 0;
	unsigned int cUrls = 0;

	try
	{

		/** For trailer */
		cSize-=25;

		memset(szBuf, '\0', 1025);

		/** Header... 59 */
		sprintf( szBuf, "<html><body bgcolor=\"#ffffff\"><font size=\"3\">\nActive IP addresses: %d<hr>", m_cIP );
		cLen = strlen( szBuf );
		if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; }
 	

		/** List of active IP addresses */
		memset(szBuf, '\0', 1025);
		//sprintf( szBuf, "<ul>" );
		strcpy( szBuf, "<ul>" );
		cLen = strlen( szBuf );
		if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; cOpenList++; };
		


		/** Iterate over the IPs */
		for ( unsigned int i=0; i<m_cIP; i++ )
		{
			pIp = m_IP[ i ];
		
			/** Area ip address */
			sprintf( szBuf, "<li><h3>%s</h3>", m_pszIP[i] );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;


			/** Default values */
			sprintf( szBuf, "<b>Host</b>: <tt>%s &nbsp;</tt>", pIp->getHostName() );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;

			sprintf( szBuf, "<b>Port</b>: <tt>%s &nbsp;</tt>", pIp->getPort() );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;

			sprintf( szBuf, "<b>Domain name</b>: <tt>%s &nbsp;</tt> <p>", pIp->getDomainName() );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;


			if (pIp->getDefaultCaching() == true)
			{
				sprintf( szBuf, "<b>Default Caching</b>: <tt>TRUE </tt><p>");
			}
			else
			{
				sprintf( szBuf, "<b>Default Caching</b>: <tt>FALSE </tt><p>");
			}
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;


			if (pIp->getDefaultActiveLoginRequired() == true)
			{
				sprintf( szBuf, "<b>Default Active Login Required</b>: <tt>TRUE </tt><p>");
			}
			else
			{
				sprintf( szBuf, "<b>Default Active Login Required</b>: <tt>FALSE </tt><p>");
			}
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;


			sprintf( szBuf, "<b>Default Required Data</b>: <tt>%s, %d </tt><p>", pIp->getDefaultRequiredDataList(), pIp->getDefaultRequiredDataMask() );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;

			
			sprintf( szBuf, "<b>Default login Url (relative | absolute)</b>: <tt>%s | %s</tt><p>", 
				pIp->getDefaultLoginPageUrl(), pIp->getDefaultAbsoluteLoginPageUrl() );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;

	
			sprintf( szBuf, "<b>Default upsell Url (relative | absolute)</b>: <tt>%s | %s</tt><p>", 
				pIp->getDefaultUpsellPageUrl(), pIp->getDefaultAbsoluteUpsellPageUrl() );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
	
		
			sprintf( szBuf, "<b>Default access denied Url (relative | absolute)</b>: <tt>%s | %s</tt><p>\n", 
				pIp->getDefaultAccessDeniedPageUrl(), pIp->getDefaultAbsoluteAccessDeniedPageUrl()  );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
	

			if (strncmp(pIp->getDefaultAbsoluteSignupPageUrl(), pIp->getDefaultAbsoluteLoginPageUrl(),
				        strlen(pIp->getDefaultAbsoluteSignupPageUrl())) == 0)
			   sprintf(szBuf,
			           "<b>Default signup Url (relative | absolute)</b> (not defined, default to Default Login Page URL): <tt>%s | %s</tt><p>\n",
					   pIp->getDefaultSignupPageUrl(), pIp->getDefaultAbsoluteSignupPageUrl());
			else
			   sprintf(szBuf, "<b>Default signup Url (relative | absolute)</b>: <tt>%s | %s</tt><p>\n",
				       pIp->getDefaultSignupPageUrl(), pIp->getDefaultAbsoluteSignupPageUrl());
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;


			sprintf( szBuf, "<b>Premium Areas</b> (%d)\n", pIp->getPremiumAreaCount() );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
		
		
			/** Premium Area count */
			sprintf( szBuf, "<ul>" );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; cOpenList++; } else break;

		
			/** Enumerate over premium areas.. */
			cAreas = pIp->getPremiumAreaCount();
			
			for ( unsigned int j=0; j<cAreas; j++ )
			{
				const CPremiumArea * pPa = pIp->getPremiumArea(j);
			
				if ( pPa != 0 )
				{
					/** Area name */
					sprintf( szBuf, "<li><b>Area name</b>: %s<br>\n", pPa->getName() );
					cLen = strlen( szBuf );
					if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
			

					/** Get tokens needed */
					sprintf( szBuf, "<b>Tokens-Required</b>: <tt>%s</tt><br>\n", pPa->getOriginalTokenExpr() );
					cLen = strlen( szBuf );
					if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;


					/** Has required data list? */
					if ( true == pPa->hasRequiredData() )
					{
  					    sprintf( szBuf, "<b>Required-Data</b>: <tt>%s, %d</tt><br>\n", pPa->getRequiredDataList(), pPa->getRequiredDataMask() );
						cLen = strlen( szBuf );
						if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
					}


					/** Has a caching flag? */
					if ( true == pPa->hasCachingTag() )
					{
						if (pPa->getCaching() == true)
						{
							sprintf( szBuf, "<b>Caching</b>: <tt>TRUE </tt><br>" );
						}
						else
						{
							sprintf( szBuf, "<b>Caching</b>: <tt>FALSE </tt><br>" );
						}
						cLen = strlen( szBuf );
						if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
					}

					
					/** Has a active login required flag? */
					if ( true == pPa->hasActiveLoginRequired() )
					{
						if (pPa->getActiveLoginRequired() == true)
						{
							sprintf( szBuf, "<b>Active Login Required</b>: <tt>TRUE </tt><br>" );
						}
						else
						{
							sprintf( szBuf, "<b>Active Login Required</b>: <tt>FALSE </tt><br>" );
						}
						cLen = strlen( szBuf );
						if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
					}


					/** Has a login Url? */
					if ( true == pPa->hasLoginPageUrl() )
					{
						sprintf( szBuf, "<b>Login Url (relative | absolute)</b>: <tt>%s | %s</tt><br>", 
							pPa->getLoginPageUrl(), pPa->getAbsoluteLoginPageUrl() );
						cLen = strlen( szBuf );
						if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
					}

					
					/** Has an upsell page Url? */
					if ( true == pPa->hasUpsellPageUrl() )
					{
						sprintf( szBuf, "<b>Upsell Url (relative | absolute)</b>: <tt>%s | %s</tt><br>", 
							pPa->getUpsellPageUrl(), pPa->getAbsoluteUpsellPageUrl() );
						cLen = strlen( szBuf );
						if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
					}

					
					/** Has an access denied Url? */
					if ( true == pPa->hasAccessDeniedPageUrl() )
					{
						sprintf( szBuf, "<b>AccessDenied Url (relative | absolute)</b>: <tt>%s | %s</tt><br>", 
							pPa->getAccessDeniedPageUrl(), pPa->getAbsoluteAccessDeniedPageUrl() );
						cLen = strlen( szBuf );
						if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
					}

					/** Has a signup Url? */
					if ( true == pPa->hasSignupPageUrl() )
					{
						sprintf( szBuf, "<b>Signup Url (relative | absolute)</b>: <tt>%s | %s</tt><br>", 
							pPa->getSignupPageUrl(), pPa->getAbsoluteSignupPageUrl() );
						cLen = strlen( szBuf );
						if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
					}


					/** Get a count of the number of request Urls... */
					cUrls = pPa->getPremiumUrlCount();
					sprintf( szBuf, "<b>Request Urls</b> (%d)\n", cUrls ); 
					cLen = strlen( szBuf );
					if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;


					/** Enumerate over request Urls */
					sprintf( szBuf, "<ul>" );
					cLen = strlen( szBuf );
					if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; cOpenList++; } else break;

					for ( unsigned int k=0; k<cUrls; k++ )
					{
						sprintf( szBuf, "<li> <tt>%s</tt>", pPa->getPremiumUrl(k) ); 
						cLen = strlen( szBuf );
						if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
					}

					sprintf( szBuf, "</ul>\n" );
					cLen = strlen( szBuf );
					if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; cOpenList--; } else break;

					sprintf( szBuf, "<p>\n" );
					cLen = strlen( szBuf );
					if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;

				}
			}

			sprintf( szBuf, "</ul>\n" );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; cOpenList--; } else break;

		
			/** Free Area count*/
			cAreas = pIp->getFreeAreaCount();
			
			sprintf( szBuf, "<b>Free Areas</b> (%d)\n", cAreas );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;


			/** Enumerate over free areas.. */
			if ( cAreas>0 )
			{
				sprintf( szBuf, "<ul>" );
				cLen = strlen( szBuf );
				if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; cOpenList++; } else break;

				for ( unsigned int k=0; k<cAreas; k++ )
				{
					sprintf( szBuf, "<li><tt>%s</tt>", pIp->getFreeAreaUrl(k) );
					cLen = strlen( szBuf );
					if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; } else break;
				}

				sprintf( szBuf, "</ul>\n" );
				cLen = strlen( szBuf );
				if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; cOpenList--; } else break;
			}


			sprintf( szBuf, "<hr>\n" );
			cLen = strlen( szBuf );
			if ( cLen<cSize ) { strcat( pszBuffer, szBuf ); cSize-=cLen; }
		}


	
		if ( cOpenList>10 ) { cOpenList=10; }
		
		while ( cOpenList>0 )
		{
			strcat( pszBuffer, "</ul>" );
			cOpenList--;
		}

		/** We should have at least 25 bytes left here... */
		strcat( pszBuffer, "</font></body></html>\n");
	
		return true;

	}
	catch( ... ) 
	{
		CLog::log("CGoAuthIPCollection::getConfigInfo> Exception!");
		return false;
	}
}

/** *****************************************************************
 *
 * CGoAuthIPCollection::checkRequiredData
 *
 * Given the required data list, check if it is valid. Return the
 * correct data mask if it's valid, return -1 otherwise.
 *
 **/

int CGoAuthIPCollection::checkRequiredData(const char *psz) const
{
	if (psz == 0 || strlen(psz) == 0)
	{
		CLog::log("CGoAuthIPCollection::checkRequiredData > Null input string");
		return -1;
	}

	int cDataMask = 0;
	char *pszRequiredData = new char[strlen(psz) + 1];
	if (pszRequiredData == 0)
	{
		CLog::log("CGoAuthIPCollection::checkRequiredData > Out of memory");
		return -1;
	}

	memset(pszRequiredData, '\0', strlen(psz) + 1);

	static const char * const username  = "username";
	static const char * const firstName = "firstname";
	static const char * const lastName  = "lastname";
	static const char * const email     = "email";
	static const char * const birthday  = "birthday";
	static const char * const geocode   = "geocode";
	static const char * const demoMask  = "demomask";
	static const char * const gender    = "gender";
	static const char * const all       = "all";

    // Convert the data list to lower case

	for (unsigned int i = 0, j = 0; i < strlen(psz); ++i)
	{
		// Skip whitespace

		if (isspace(psz[i]))
			continue;

		// Copy ','

		if (psz[i] == ',')
			pszRequiredData[j++] = psz[i];
		else if (isalpha(psz[i]))
		    pszRequiredData[j++] = tolower(psz[i]);
		else
		{
			CLog::log("CGoAuthIPCollection::checkRequiredData > Invalid required data list");
			return -1;
		}
	}

	char *pszData = strtok(pszRequiredData, ",");
	char  szErr[128] = {'\0'};

	while (pszData != 0)
	{
       if (strcmp(pszData, username) == 0)
		cDataMask |= g_cUsernameBit;
       else if (strcmp(pszData, firstName) == 0)
		cDataMask |= g_cFirstNameBit;
       else if (strcmp(pszData, lastName) == 0)
		cDataMask |= g_cLastNameBit;
       else if (strcmp(pszData, email) == 0)
		cDataMask |= g_cEmailAddressBit;
       else if (strcmp(pszData, birthday) == 0)
		cDataMask |= g_cBirthdayBit;
       else if (strcmp(pszData, geocode) == 0)
		cDataMask |= g_cGeoCodeBit;
       else if (strcmp(pszData, demoMask) == 0)
		cDataMask |= g_cDemoMaskBit;
       else if (strcmp(pszData, gender) == 0)
		cDataMask |= g_cGenderBit;
       else if (strcmp(pszData, all) == 0)
	   {
		cDataMask = g_cCompleteRegRequiredFieldsMask | g_cGenderBit;
		
		// Ignore the rest
		break;
	   }
	   else
	   {
		sprintf(szErr, "CGoAuthIPCollection::checkRequiredData > Invalid data %s; ignored", pszData);
		CLog::log(szErr);
		memset(szErr, '\0', 128);
	   }

	   pszData = strtok(0, ",");
	}

    return cDataMask;
}

/** *****************************************************************
 *
 * CGoAuthIPCollection::trim
 *
 * Left/right whitepace trim [ '\n', '\r', '\t', ' ' ]
 *
 **/
bool CGoAuthIPCollection::trim( char * psz ) const
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



