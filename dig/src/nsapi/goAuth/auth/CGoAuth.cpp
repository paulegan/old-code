//--file: CGoAuth.cpp------------------------------------------------
//
#include <stdio.h>
#include <memory.h>
#include <fstream.h>
#include <string.h>
#include "CGoAuth.h"
#include "CGoAuthResponse.h"
#include "./../utils/CLog.h"


/** Copyright info: */
const char * g_pszCopyright = "2001 Walt Disney Internet Group";

const char * g_pszVersion   = "1.2.0";

const char * g_pszBuildDate = "04/09/2001";

#ifdef __OS_SOLARIS__
	const char * g_pszType = "libGoAuth.so";
#elif defined __OS_WINNT__
	const char * g_pszType = "GoAuthISAPI.dll";
#endif



/** '\\' for WINNT,  '/' otherwise... */
extern const char * g_pszFileDelim;



/** HTTP 1.1 text/html response header */
const char * g_pszHTTPMoved = "HTTP/1.1 302 Moved Temporarily\r\n";

/** HTTP 1.1 html response header */
const char * g_pszHTTPHeader = "HTTP/1.1 200 OK\r\n";

/** HTTP 1.1 text/html response header */
const char * g_pszContentTypeHeader = "Content-type: text/html\r\n";

/** HTTP 1.1 cache control response header */
const char * g_pszCacheControlHeader = "Cache-Control: no-cache, no-store, private\r\n";



/** *****************************************************************
 *
 * CGoAuth::CGoAuth
 *
 **/
CGoAuth::CGoAuth()
{
	m_mutexStats.Init();
	m_mutexReinit.Init();


	memset( m_szDir, '\0', 300 );
	memset( m_szIni, '\0', 300 );

	memset( m_szStartTime, '\0', 65 );
	memset( m_szLastReinitTime, '\0', 65 );
	memset( m_szLastStatsResetTime, '\0', 65 );

			
	LockReinit();
	m_pIP = 0;
	m_pIP = &m_IPs[0];
	UnlockReinit();

	m_pLog = 0;
	m_pTime = 0;

	m_bInitialized = false;
	m_bVerbose = true;		// changed default behaviour 10/4/99 -jam


	m_cTotalHitCount = 0;
	m_cHitCount = 0;                        m_cHitsNotServiced = 0;
	m_cHitsNotServicedInvalidRequest = 0;   m_cHitsNotServicedAuthUninit = 0;
	m_cHitsNotServicedBadIPRequest = 0;     m_cHitsServiced = 0;
	m_cHitAdminRequest = 0;                 m_cHitUserRequest = 0;
	m_cHitLoginUpsellAccessDenied = 0;      m_cHitFree = 0;
	m_cHitPremium = 0;                      m_cHitUpsell = 0;
	m_cHitGrantAccess = 0;                  m_cHitAccessDenied = 0;
	m_cHitUserLogin = 0;                    m_cHitSignupUser = 0;
	m_cHitNonPremiumNonFree = 0;

	return;
}




/** *****************************************************************
 * 
 * CGoAuth::init
 *
 **/
bool CGoAuth::init( const char * pszDir, const char * pszIni, const char * pszLog )
{
	bool bLogInit=false;
	bool bIniInit=false;
	char szLogFilename[500] = {'\0'};
	unsigned int cLen;


	m_pLog  = new CLog();
	m_pTime = new CTime();

	m_pLog->create();

	m_pTime->getLocalDateTime( m_szStartTime, 64 );

	if ( 0==pszDir || 0==pszIni || 0==pszLog )
		return false;

	/*
	sprintf( m_szIni, "%s", pszIni );
	sprintf( m_szDir, "%s", pszDir );
	*/

	strncpy(m_szIni, pszIni, strlen(pszIni) + 1);
	strncpy(m_szDir, pszDir, strlen(pszDir) + 1);

	/** Generate new log filename */
	cLen = strlen(pszDir) + strlen(g_pszFileDelim) + strlen( pszLog ) + 1;
	memset(szLogFilename, '\0', 500);

	if ( cLen < 497 )
		sprintf( szLogFilename, "%s%s%s", pszDir, g_pszFileDelim, pszLog );
	else
		//sprintf( szLogFilename, "GoAuth.log" );
		strcpy( szLogFilename, "GoAuth.log" );


	/** Try to initialize logging */
	bLogInit = m_pLog->init( "GoAuth", szLogFilename );



	/** Try to initialize from the .ini file */
	bIniInit = initializeIP( pszDir, pszIni );

	if ( 0 != CSimpleXMLParser::s_cNodesAllocated )
	{
		sprintf (szLogFilename, "Initialization XML node leak [%d]", CSimpleXMLParser::s_cNodesAllocated );
		CLog::log(szLogFilename);
	}


	/** Was initialization successful? */
	if ( true == bIniInit && true == bLogInit )
	{
		m_pTime->getLocalDateTime( m_szLastReinitTime, 64 );
		m_pTime->getLocalDateTime( m_szLastStatsResetTime, 64 );
		m_bInitialized = true;
	}	

	return m_bInitialized;
}





/** *****************************************************************
 * 
 * CGoAuth::initializeIP
 *
 * Given an .ini file name,  ask one of the 2 IPs to initialize 
 * itself. 
 *
 **/
bool CGoAuth::initializeIP( const char * pszDir, const char * pszIni )
{
	bool b;

	/** This function is called from within "LockReinit()/UnlockReinit()" */

	/** m_pIP points to the current IP instance in use... */
	b = m_pIP->init( pszDir, pszIni, &m_bVerbose  );
	
	return (b);
}




/** *****************************************************************
 * 
 * CGoAuth::toggleIPs
 *
 **/
bool CGoAuth::toggleIPs()
{
	try
	{

	/** This function is called from within "LockReinit()/UnlockReinit()" */

		if ( m_pIP == (&m_IPs[0]) )
		{
			m_pIP = &m_IPs[1];
		}
		else
		{
			m_pIP = &m_IPs[0];
		}
	
		return true;

	}
	catch( ... )
	{
		CLog::log("CGoAuth::toggleIPs> Exception!");
		return false;
	}
}




/** *****************************************************************
 * 
 * CGoAuth::deinit
 *
 **/
bool CGoAuth::deinit()
{
	char szLog[64];


	try
	{
		LockReinit();
	
		if ( true == m_bInitialized )
		{
			m_bInitialized = false;

			CLog::log("CGoAuth::deinit> begin");

			m_IPs[0].destroy();
			m_IPs[1].destroy();

			if ( true == m_bVerbose )
			{
				sprintf( szLog, "XMLNodes outstanding: %d", CSimpleXMLParser::s_cNodesAllocated );
				CLog::log( szLog );
				
				CLog::log("CGoAuth::deinit> end");
			}
		}

		m_bInitialized = false;

		UnlockReinit();

		return true;

	}
	catch( ... )
	{
		return false;
	}
}



/** *****************************************************************
 * 
 * CGoAuth::authenticate
 *
 **/
bool CGoAuth::authenticate( const char * pszIP, const char * pszUrl, const char * pszQryStr, char * pszCookieData, CGoAuthResponse & r )
{
	unsigned long cRequest = 0;
	char szMsg[718] = {'\0'};
	int cIndex = -1;
	bool bContainsIp = false;
	bool bIsAuthRedirect = false;
	
	
	try
	{

		/* Update total hit count */
		LockStats();
		  m_cTotalHitCount++;
		  m_cHitCount++;
		  cRequest = m_cHitCount;
		UnlockStats();
	
	
		/** Log the beginning of the auth request */
		if ( true == m_bVerbose )
		{
			sprintf( szMsg, "CGoAuth::authenticate> (%06ld) Begin request.", cRequest );
			CLog::log( szMsg );
		}



		/** Bad params... this is bad */
		if ( 0==pszIP || 0==pszUrl || 0==pszQryStr || 0==pszCookieData )
		{
			if ( true == m_bVerbose )
			{
				sprintf( szMsg, "CGoAuth::authenticate> (%06ld) End request. Auth request contains bad input parameters...", cRequest );
				CLog::log( szMsg );
			}

			LockStats();
			  m_cHitsNotServiced++;
			  m_cHitsNotServicedInvalidRequest++;
			UnlockStats();
		
			r.m_cStatus = GO_AUTH_REQUEST_CONTINUE;
			return false;
		}



		/** Uninitialized auth... this is also bad. */
		if ( false == m_bInitialized )
		{
			if ( true == m_bVerbose )
			{
				sprintf( szMsg, "CGoAuth::authenticate> (%06ld) End request. Auth request for uninitialized auth...", cRequest );
				CLog::log( szMsg );
			}

			r.m_cStatus = GO_AUTH_REQUEST_CONTINUE;

			LockStats();
			  m_cHitsNotServiced++;
			  m_cHitsNotServicedAuthUninit++;
			UnlockStats();
		
			return false;
		}



		/** A request for an IP we're authenticating for... */
		LockReinit();
		bContainsIp = m_pIP->containsRequestIP( pszIP, &cIndex );
		UnlockReinit();

	
		
		if ( true == bContainsIp )
		{
			if ( true == m_bVerbose )
			{
				sprintf( szMsg, "CGoAuth::authenticate> (%06ld) Serving auth request for IP [%s]", cRequest, pszIP );
				CLog::log( szMsg );
			}


			LockStats();   
			  m_cHitsServiced++;  
			UnlockStats();

		
			/** A request to do something to Auth */
			if ( true == this->requestIsAdmin( pszQryStr) )
			{
				if ( true == m_bVerbose )
				{
					sprintf( szMsg, "CGoAuth::authenticate> (%06ld) Auth request is administrative...[%s]", cRequest, pszQryStr );
					CLog::log( szMsg );
				}

				LockStats();
				  m_cHitAdminRequest++;
				UnlockStats();

				return ( this->handleAdminRequest( cRequest, cIndex, pszIP, pszUrl, pszQryStr, r ) );
			}
		
		
			LockStats();   
			  m_cHitUserRequest++;
			UnlockStats();


			/** A request for a Login, Upsell, AccessDenied, or Signup Page Url... */
			LockReinit();
			bIsAuthRedirect = m_pIP->isAuthRedirectPage( cIndex, pszUrl );
			UnlockReinit();

						
			if ( true == bIsAuthRedirect )
			{
				r.m_cStatus = GO_AUTH_REQUEST_CONTINUE;
				
				LockStats();   
				  m_cHitLoginUpsellAccessDenied++;   
				UnlockStats();
		
				return true;
			}

			
			LockReinit();
			m_pIP->authenticate( cRequest, (unsigned int)cIndex, pszIP, pszUrl, pszQryStr, pszCookieData, r, m_bVerbose );
			UnlockReinit();


			
			/** Update stats */
			if ( r.wasFreeRequest() )
			{
				LockStats();   
				  m_cHitFree++;   
				UnlockStats();
			}
			else if ( r.wasPremiumRequest() )
			{
				LockStats();   
				  m_cHitPremium++;   
				UnlockStats();

				if ( true == r.result(GO_USER_UPSOLD) )
				{
					LockStats();   
					  m_cHitUpsell++;   
					UnlockStats();
				}
				else if ( true == r.result(GO_USER_GRANT_ACCESS) )
				{
					LockStats();   
					  m_cHitGrantAccess++;   
					UnlockStats();
				}
				else if ( true == r.result(GO_USER_ACCESS_DENIED) )
				{
					LockStats();   
					  m_cHitAccessDenied++;   
					UnlockStats();
				}
				else if ( true == r.result(GO_USER_MUST_SIGNUP) )
				{
					LockStats();   
					  m_cHitSignupUser++;
					UnlockStats();
				}
				else if ( true == r.result(GO_USER_MUST_LOGIN) )
				{
					LockStats();   
					  m_cHitUserLogin++;   
					UnlockStats();
				}
			}
			else
			{
				LockStats();   
				  m_cHitNonPremiumNonFree++;   
				UnlockStats();
			}

			return true;
		}


		/** The request is not being serviced because the IP addres is not valid */
		if ( true == m_bVerbose )
		{
			sprintf( szMsg, "CGoAuth::authenticate> (%06ld) End request. Auth request for invalid IP address [%s]", cRequest, pszIP );
			CLog::log( szMsg );
		}

		LockStats();
		  m_cHitsNotServiced++;
		  m_cHitsNotServicedBadIPRequest++;
		UnlockStats();

		r.m_cStatus = GO_AUTH_REQUEST_CONTINUE;
		return true;

	}
	catch( ... )
	{
		CLog::log("CGoAuth::authenticate> Exception!");
		return false;
	}
}





/** *****************************************************************
 *
 * CGoAuth::~CGoAuth
 *
 **/
CGoAuth::~CGoAuth()
{
	this->deinit();

	m_mutexStats.Destroy();
	m_mutexReinit.Destroy();

	CLog::destroy();
		
	return;
}




/** *****************************************************************
 * 
 * CGoAuth::requestIsAdmin
 *
 * Is the authenticate request actually a request for administration 
 * of auth.
 *
 **/
bool CGoAuth::requestIsAdmin( const char * pszQryStr ) const
{
	try
	{
		if ( 0!=pszQryStr ) 
		{
			if( ( 0 == strncmp( pszQryStr, "goauth_reinit",     13 )) ||
				( 0 == strncmp( pszQryStr, "goauth_stats",      12 )) ||
				( 0 == strncmp( pszQryStr, "goauth_config",     13 )) ||
				( 0 == strncmp( pszQryStr, "goauth_verbose",    14 )) ||
				( 0 == strncmp( pszQryStr, "goauth_silent",     13 )) ||
				( 0 == strncmp( pszQryStr, "goauth_version",    14 )) ||
				( 0 == strncmp( pszQryStr, "goauth_getgreen",   15 )) || 
				( 0 == strncmp( pszQryStr, "goauth_resetstats", 17 )) )
			{
				return true;
			}
		}

		return false;

	}
	catch( ... )
	{
		CLog::log("CGoAuth::requestIsAdmin> Exception!");
		return false;
	}
}




/** *****************************************************************
 * 
 * CGoAuth::handleAdminRequest
 *
 **/
bool CGoAuth::handleAdminRequest( unsigned long cRequest, int cIdx, const char * pszIP, const char * pszUrl, const char * pszQryStr, CGoAuthResponse & r )
{
	char szLog[80] = {'\0'};
	bool b = false;
		
	try
	{

		if ( 0 != pszQryStr ) 
		{
			if ( 0 == strncmp( pszQryStr, "goauth_reinit", 13 ) )
			{
				b = handleReinitRequest( cRequest, cIdx, pszIP, pszUrl, pszQryStr, r );
			}
			else if ( 0 == strncmp( pszQryStr, "goauth_stats", 12 ))
			{
				b = handleStatsRequest( cRequest, cIdx, pszIP, pszUrl, pszQryStr, r );
			}
			else if ( 0 == strncmp( pszQryStr, "goauth_config", 13 ))
			{
				b = handleConfigRequest( cRequest, cIdx, pszIP, pszUrl, pszQryStr, r );
			}
			else if ( 0 == strncmp( pszQryStr, "goauth_verbose", 14 ))
			{
				b = handleRunModeRequest( cRequest, cIdx, pszIP, pszUrl, pszQryStr, r );
			}
			else if ( 0 == strncmp( pszQryStr, "goauth_silent", 13 ))
			{
				b = handleRunModeRequest( cRequest, cIdx, pszIP, pszUrl, pszQryStr, r );
			}
			else if ( 0 == strncmp( pszQryStr, "goauth_version", 14 ))
			{
				b = handleVersionRequest( cRequest, cIdx, pszIP, pszUrl, pszQryStr, r );
			}
			else if ( 0 == strncmp( pszQryStr, "goauth_getgreen", 15 ))
			{
				b = handleGreenRequest( cRequest, cIdx, pszIP, pszUrl, pszQryStr, r );
			}
			else if ( 0 == strncmp( pszQryStr, "goauth_resetstats", 17 ))
			{
				b = handleStatsReinitRequest( cRequest, cIdx, pszIP, pszUrl, pszQryStr, r );
			}

		}
	
		if ( true == b )
		{
			sprintf( szLog, "CGoAuth::handleAdminR> (%06ld) End request. Admin request served.", cRequest );
			CLog::log( szLog );
			return true;
		}
		else
		{
			sprintf( szLog, "CGoAuth::handleAdminR> (%06ld) End request. Admin request NOT served.", cRequest );
			CLog::log( szLog );
			return false;
		}

	}
	catch( ... )
	{
		CLog::log("CGoAuth::handleAdminRequest> Exception!");
		return false;
	}
}





/** *****************************************************************
 *
 * CGoAuth::handleStatsReinitRequest
 *
 **/
bool CGoAuth::handleStatsReinitRequest( unsigned long cRequest, int cIdx, const char * pszIP, const char * pszUrl, const char * pszQry, CGoAuthResponse & r ) 
{
	char szMsg[512] = {'\0'};
	

	LockStats();
	m_pTime->getLocalDateTime( m_szLastStatsResetTime, 64 );
	m_cHitCount = 0;
	m_cHitsNotServiced = 0;
	m_cHitsNotServicedInvalidRequest = 0;
	m_cHitsNotServicedAuthUninit = 0;
	m_cHitsNotServicedBadIPRequest = 0;
	m_cHitsServiced = 0;
	m_cHitAdminRequest = 0;
	m_cHitUserRequest = 0;
	m_cHitNonPremiumNonFree = 0;
	m_cHitLoginUpsellAccessDenied = 0;
	m_cHitFree = 0;
	m_cHitPremium = 0;
	m_cHitUpsell = 0;
	m_cHitGrantAccess = 0;
	m_cHitAccessDenied = 0;
	m_cHitUserLogin = 0;
	m_cHitSignupUser = 0;
	UnlockStats();


	/** Log entry */
	sprintf( szMsg, "CGoAuth::handleStatsR> (%06ld) Stats reinit [%s]...", cRequest, g_pszVersion );
	CLog::log( szMsg );

	/**  "HTTP/1.1 200 OK\r\n  */
	r.addResponseHeader( g_pszHTTPHeader );

	/** Content-Type: text/html\r\n\r\n" */
	sprintf( szMsg, "%s\r\n", g_pszContentTypeHeader );
	r.addResponseHeader( szMsg );
	
	/** "<html><body> ... </body></html>"  */
	memset( szMsg, '\0', 512 );
	sprintf( szMsg, "<html><body>Stats reset...\n</body</html>" );
	r.addMessageBody( szMsg );
			
	r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
	return true;

}




/** *****************************************************************
 *
 * CGoAuth::handleVersionRequest
 *
 **/
bool CGoAuth::handleVersionRequest( unsigned long cRequest, int cIdx, const char * pszIP, const char * pszUrl, const char * pszQry, CGoAuthResponse & r ) 
{
	char szMsg[512] = {'\0'};
	
	/** Log entry */
	sprintf( szMsg, "CGoAuth::handleVersio> (%06ld) Version info [%s]...", cRequest, g_pszVersion );
	CLog::log( szMsg );

	/**  "HTTP/1.1 200 OK\r\n  */
	r.addResponseHeader( g_pszHTTPHeader );

	/** Content-Type: text/html\r\n\r\n" */
	sprintf( szMsg, "%s\r\n", g_pszContentTypeHeader );
	r.addResponseHeader( szMsg );

	/** "<html><body> ... </body></html>"  */
	memset( szMsg, '\0', 512 );
	sprintf( szMsg, "<html><body><h3>%s</h3><p>\n<b>Copyright (C)</b> %s<p>\n<b>Version:</b> %s<p>\n<b>Build Date:</b> %s<p>\n</body</html>", g_pszType, g_pszCopyright, g_pszVersion, g_pszBuildDate );
	r.addMessageBody( szMsg );
			
	r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
	return true;
}





/** *****************************************************************
 *
 * CGoAuth::handleReinitRequest
 *
 **/
bool CGoAuth::handleRunModeRequest( unsigned long cRequest, int cIdx, const char * pszIP, const char * pszUrl, const char * pszQry, CGoAuthResponse & r ) 
{
	char szMsg[90] = {'\0'};
	
	try
	{

		if ( 0 == strncmp( pszQry, "goauth_verbose", 14 ))
		{
			LockReinit();
			m_bVerbose = true;	  
			UnlockReinit();

			/** Log entry */
			sprintf( szMsg, "CGoAuth::handleRunMde> (%06ld) Switching to verbose run mode...", cRequest );
			CLog::log( szMsg );

			
			/**  "HTTP/1.1 200 OK\r\n  */
			r.addResponseHeader( g_pszHTTPHeader );

			/** Content-Type: text/html\r\n\r\n" */
			sprintf( szMsg, "%s\r\n", g_pszContentTypeHeader );
			r.addResponseHeader( szMsg );

	
			/** "<html><body> ... </body></html>"  */
			sprintf( szMsg, "<html><body>Running in verbose mode...</body</html>" );
			r.addMessageBody( szMsg );
			
			r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
			return true;
		}
		else if ( 0 == strncmp( pszQry, "goauth_silent", 13 ))
		{
			LockReinit();
			m_bVerbose = false;	  
			UnlockReinit();

			/** Log entry */
			sprintf( szMsg, "CGoAuth::handleRunMde> (%06ld) Switching to silent run mode...", cRequest );
			CLog::log( szMsg );

			/**  "HTTP/1.1 200 OK\r\n  */
			r.addResponseHeader( g_pszHTTPHeader );

			/** Content-Type: text/html\r\n\r\n" */
			sprintf( szMsg, "%s\r\n", g_pszContentTypeHeader );
			r.addResponseHeader( szMsg );
	
			/** "<html><body> ... </body></html>"  */
			sprintf( szMsg, "<html><body>Running in silent mode...</body</html>" );
			r.addMessageBody( szMsg );
			
			r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
			return true;
		}
		else
		{
			return false;
		}
	}
	catch( ... )
	{
		CLog::log("CGoAuth::handleRunModeRequest> Exception!");
		return false;
	}
}




/** *****************************************************************
 *
 * CGoAuth::handleReinitRequest
 *
 **/
bool CGoAuth::handleReinitRequest( unsigned long cRequest, int cIdx, const char * pszIP, const char * pszUrl, const char * pszQry, CGoAuthResponse & r) 
{
	CGoAuthIPCollection * p = 0;
	char szResponse[80] = {'\0'};
	char szHdr[64] = {'\0'};
	bool bSwitchActiveBankTo0 = false;
	
	
	try
	{

		LockReinit();
	
		if ( m_pIP == &m_IPs[0] )
		{
			bSwitchActiveBankTo0 = false;
			p = &m_IPs[1];
		}
		else
		{
			bSwitchActiveBankTo0 = true;
			p = &m_IPs[0];
		}

		p->destroy();

		if ( true == p->init( m_szDir, m_szIni, &m_bVerbose ) )
		{
			m_pTime->getLocalDateTime( m_szLastReinitTime, 64 );

			toggleIPs();
			sprintf( szResponse, "<html><body>Reinit: success<br>Active bank: %d</body></html>\r\n", 
				( (true==bSwitchActiveBankTo0) ? (0) : (1)) );
		}
		else
		{
			sprintf( szResponse, "<html><body>Reinit: failed<br>Active bank: %d</body></html>\r\n", 
				( (true==bSwitchActiveBankTo0) ? (0) : (1)) );
		}
	
		UnlockReinit();

	
		/**  "HTTP/1.1 200 OK\r\n  */
		r.addResponseHeader( g_pszHTTPHeader );

		/** Content-Type: text/html\r\n\r\n" */
		sprintf( szHdr, "%s\r\n", g_pszContentTypeHeader );
		r.addResponseHeader( szHdr );
	
		/** "<html><body> ... </body></html>"  */
		r.addMessageBody( szResponse );
	
		r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
		return true;

	}
	catch( ... )
	{
		CLog::log("CGoAuth::handleReinitRequest> Exception!");
		return false;
	}
}




/** *****************************************************************
 *
 * CGoAuth::handleConfigRequest
 *
 * Need about 2KB for each active IP address for config information.
 *
 **/
bool CGoAuth::handleConfigRequest( unsigned long cRequest, int cIdx, const char * pszIP, const char * pszUrl, const char * pszQry, CGoAuthResponse & r ) 
{
	char szMsg[8192] = {'\0'};
	char szHdr[64] = {'\0'};

	try
	{

		/** Get IP bank's config information */
		LockReinit();
		m_pIP->getConfigInfo( szMsg, (sizeof(szMsg)-1) );
		UnlockReinit();

		/**  "HTTP/1.1 200 OK\r\n  */
		r.addResponseHeader( g_pszHTTPHeader );

		/** Content-Type: text/html\r\n\r\n" */
		sprintf( szHdr, "%s\r\n", g_pszContentTypeHeader );
		r.addResponseHeader( szHdr );
	
		/** "<html><body> ... </body></html>"  */
		r.addMessageBody( szMsg );

		r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
		return true;

	}
	catch( ... )
	{
		CLog::log("CGoAuth::handleConfigRequest> Exception!");
		return false;
	}
}





/** *****************************************************************
 *
 * CGoAuth::handleReinitRequest
 *
 **/
bool CGoAuth::handleStatsRequest( unsigned long cRequest, int cIdx, const char * pszIP, const char * pszUrl, const char * pszQry, CGoAuthResponse & r ) 
{
	char szMsg[1024] = {'\0'};
	char szHdr[128]  = {'\0'};
	char szNum[50];

	try
	{

		strcat ( szMsg, "<html><body><font size=\"2\">\n");
		strcat ( szMsg, "<b>Auth stats:</b><hr>\n");
	 
		sprintf( szHdr, "<b>Begin time:</b> %s <i>(local)</i><br>\n",         m_szStartTime );
		strcat ( szMsg, szHdr );
		sprintf( szHdr, "<b>Last .dll/.so init time:</b> %s <i>(local)</i><br>\n", m_szLastReinitTime );
		strcat ( szMsg, szHdr );
		
		LockStats();

		sprintf( szHdr, "<b>Last stats reset time:</b> %s <i>(local)</i><hr>\n", m_szLastStatsResetTime );
		strcat ( szMsg, szHdr );
	
		strcat ( szMsg, "<ul>" );
 		sprintf( szNum, "<li>Total hit count since .dll/.so loaded: %ld<p>\n", m_cTotalHitCount );
		strcat ( szMsg, szNum );

 		sprintf( szNum, "<li>Total hit count since stats reset: %ld\n", m_cHitCount );
		strcat ( szMsg, szNum );

		strcat ( szMsg, "<ul>" );
		sprintf( szNum, "<li>Hits not serviced: %ld\n\n", m_cHitsNotServiced );
		strcat ( szMsg, szNum );

		strcat ( szMsg, "<ul>" );
 		 
		if ( 0 != CGoAuth::m_cHitsNotServicedInvalidRequest )
		{
		   sprintf( szNum, "<li>Invalid requests: %ld\n", m_cHitsNotServicedInvalidRequest );
		   strcat ( szMsg, szNum );
		}
	 
		if ( 0 != CGoAuth::m_cHitsNotServicedAuthUninit )
		{
		   sprintf( szNum, "<li>Auth not initialized: %ld\n", m_cHitsNotServicedAuthUninit );
		   strcat ( szMsg, szNum );
		}
	 
		if ( 0 != CGoAuth::m_cHitsNotServicedBadIPRequest )
		{
			sprintf( szNum, "<li>Invalid IP request: %ld\n", m_cHitsNotServicedBadIPRequest );
			strcat ( szMsg, szNum );
		}
		strcat ( szMsg, "</ul>\n\n" );
	 
		sprintf( szNum, "<li>Hits serviced: %ld\n", m_cHitsServiced );
		strcat ( szMsg, szNum );

		strcat ( szMsg, "<ul>" );
		sprintf( szNum, "<li>Admin Requests: %ld\n", m_cHitAdminRequest );
		strcat ( szMsg, szNum );
		sprintf( szNum, "<li>User auth requests: %ld\n", m_cHitUserRequest );
		strcat ( szMsg, szNum );

		strcat ( szMsg, "<ul>" );
 		sprintf( szNum, "<li>Login/Upsell/Signup/Access Denied hits: %ld\n", m_cHitLoginUpsellAccessDenied );
		strcat ( szMsg, szNum );
		sprintf( szNum, "<li>Non-Premium-Area/Non-Free-Area Requests: %ld\n", m_cHitNonPremiumNonFree );
		strcat ( szMsg, szNum );
		sprintf( szNum, "<li>Free Area requests: %ld\n", m_cHitFree );
		strcat ( szMsg, szNum );
		sprintf( szNum, "<li>Premium Area Requests: %ld\n", m_cHitPremium );
		strcat ( szMsg, szNum );

		strcat ( szMsg, "<ul>" );
 		sprintf( szNum, "<li>Users upsold: %ld", m_cHitUpsell );
		strcat ( szMsg, szNum );
		sprintf( szNum, "<li>Users granted access: %ld\n", m_cHitGrantAccess );
		strcat ( szMsg, szNum );
		sprintf( szNum, "<li>Users denied access: %ld\n", m_cHitAccessDenied );
		strcat ( szMsg, szNum );
		sprintf( szNum, "<li>Users redirected to login: %ld\n", m_cHitUserLogin );
		strcat ( szMsg, szNum );
		sprintf( szNum, "<li>Users redirected to signup: %ld\n", m_cHitSignupUser );
		strcat ( szMsg, szNum );
		strcat ( szMsg, "</ul>\n\n" );

		
		strcat ( szMsg, "</ul>\n\n" );
		strcat ( szMsg, "</ul>\n\n" );
		strcat ( szMsg, "</ul>\r\n" );
	 
		strcat ( szMsg, "</ul>\r\n" );
		strcat ( szMsg, "</font></body></html>\r\n\r\n" );

		UnlockStats();


		/**  "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"  */
		memset( szHdr, '\0', 128 );


		/**  "HTTP/1.1 200 OK\r\n  */
		r.addResponseHeader( g_pszHTTPHeader );

		/** Content-Type: text/html\r\n\r\n" */
		sprintf( szHdr, "%s\r\n", g_pszContentTypeHeader );
		r.addResponseHeader( szHdr );

	
		/** "<html><body> ... </body></html>"  */
		r.addMessageBody( szMsg );

		r.m_cStatus = GO_AUTH_REQUEST_COMPLETE;
		return true;

	}
	catch( ... )
	{
		CLog::log("CGoAuth::handleStatsRequest> Exception!");
		return false;
	}
}




/** *****************************************************************
 *
 * CGoAuth::handleGreenRequest
 *
 **/
bool CGoAuth::handleGreenRequest( unsigned long cRequest, int cIdx, const char * pszIP, const char * pszUrl, const char * pszQry, CGoAuthResponse & r) 
{
	bool bRet;

	try
	{
	
		LockReinit();
		bRet = m_pIP->setGreen( cIdx, r );
		UnlockReinit();

		return bRet;
	}
	catch( ... )
	{
		CLog::log("CGoAuth::handleGreenRequest> Exception!");
		return false;
	}
}

