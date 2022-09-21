//--file: main.cpp-----------------------------------------------------------
//
#include "main.h"


//#ifdef _DEBUG
  static SYS_FILE g_fd = SYS_ERROR_FD;
  static unsigned long g_cCount = 0;
  static swport_mutex g_csLogging;
//#endif


static char g_szServerIp[32]   = {'\0'};
static char g_szServerPort[16] = {'\0'};
static char g_szServerName[64] = {'\0'};

static char g_szIniDir[128]  = {'\0'};
static char g_szIniFile[128] = {'\0'};
static char g_szLogFile[128] = {'\0'};

static CGoAuth * g_pAuth = 0;

static bool g_bInit = false;
static bool g_bVerbose = true;




/** ****************************************************************
 *
 * addResponseHeaders: Given the response instance,  add the 
 *                     headers in the response to the outgoing 
 *                     Netscape response.
 *
 *                     This is essentially a translation from the 
 *                     CGoAuthResponse object to Netscape headers.
 *
 **/
bool addResponseHeaders( pblock * pb, Session * sn, Request * rq, CGoAuthResponse & resp, bool * bRedirect )
{
	char chSave;
	char szLog[256] = {'\0'};
	unsigned int cHeaders = 0;
	unsigned int cLen = 0;
	char * pszHeader = 0;
	char * pszSend = 0;

	(*bRedirect) = false;

	cHeaders = resp.getHeaderCount();
	

	/** *********************************************** Enumerate the headers.... */
	for ( unsigned int i=0; i<cHeaders; i++ )
	{
		pszHeader = (char *)resp.getHeader(i);
		cLen = strlen( pszHeader );

		/** No matter what the header - strip off the "\r\n" or "\r\n\r\n" */
		if ( 0 == pszHeader )
		{
			continue;
		}

		/** Nuke \r\n\r\n or \r\n from the end of the string... */
		if ( '\r' == pszHeader[cLen-4] )
		{
			pszHeader[cLen-4] = '\0';
			cLen -= 4;
		}
		else if ( '\r' == pszHeader[cLen-2]  )
		{
			pszHeader[cLen-2] = '\0';
			cLen -= 2;
		}
	
		#ifdef _DEBUG	
		  /** Log the header you got... */
		  if ( cLen > 30 )
		  {
			chSave = pszHeader[30];
			pszHeader[30] = '\0';
			sprintf( szLog, "addResponseHeaders> Got Hdr [%s...cont...]", pszHeader );
			goauth_log( szLog );
			pszHeader[30] = chSave;
		  }
		  else
		  {
			sprintf( szLog, "addResponseHeaders> Got Hdr [%s]", pszHeader );
			goauth_log( szLog );
	 	  }
		#endif


		/** ************************************************** HTTP/1.1 200 */
		if ( 0==strncmp( pszHeader, "HTTP/1.1 200", 12 ) )	
		{
			protocol_status( sn, rq, PROTOCOL_OK, NULL );
			
			#ifdef _DEBUG
			  sprintf( szLog, "addResponseHeaders> Setting PROTOCOL_OK" );
			  goauth_log( szLog );
			#endif
		}
		/** ************************************************** Cache-Control */
		else if ( 0==strncmp( pszHeader, "Cache-Control: ", 15 ) )
		{
			pszSend = &pszHeader[15];
	
			pblock_nvinsert("cache-control", pszSend, rq->srvhdrs);

			#ifdef _DEBUG
			  sprintf( szLog, "addResponseHeaders> Setting Cache-Control [%s]", pszSend );
			  goauth_log( szLog );
			#endif
		}
		/** ************************************************** Content-type */
		else if ( 0==strncmp( pszHeader, "Content-type: ", 14 ) )
		{
			pszSend = &pszHeader[14];
	
			pblock_nvinsert("content-type", pszSend, rq->srvhdrs);

			#ifdef _DEBUG
			  sprintf( szLog, "addResponseHeaders> Setting Content-type [%s]", pszSend );
			  goauth_log( szLog );
			#endif
		}
		/** ************************************************** Content-length */
		else if ( 0==strncmp( pszHeader, "Content-length: ", 16 ) )
		{
			pszSend = &pszHeader[16];

			pblock_nvinsert("content-length", "text/plain", rq->srvhdrs);

			#ifdef _DEBUG
			  sprintf( szLog, "addResponseHeaders> Setting Content-length: [%s]", pszSend );
			  goauth_log( szLog );
			#endif
		}
		/** ************************************************** HTTP/1.1 302 */
		else if ( 0==strncmp( pszHeader, "HTTP/1.1 302", 12 ) )	
		{
		        protocol_status(sn, rq, PROTOCOL_REDIRECT, NULL);

			(*bRedirect) = true;

			#ifdef _DEBUG
			  sprintf( szLog, "addResponseHeaders> Setting PROTOCOL_REDIRECT" );
			  goauth_log( szLog );
			#endif

			pszSend = strstr( pszHeader, "Location: ");
				
			if ( 0==pszSend )
			{
				pblock_nvinsert("error", "addResponseHeaders> Redirect exception.", pb );
				#ifdef _DEBUG
				  sprintf( szLog, "addResponseHeaders> Redirect exception: [%s...cont...]", pszHeader );
				  goauth_log( szLog );
				#endif
				return REQ_EXIT;
			}

			if ( strlen(pszSend) < 11 )
			{
				pblock_nvinsert("error", "addResponseHeaders> Redirect exception.", pb );
				#ifdef _DEBUG
				  sprintf( szLog, "addResponseHeaders> Redirect exception: [%s...cont...]", pszHeader );
				  goauth_log( szLog );
				#endif
				return REQ_EXIT;
			}

			pszSend += 10;
			cLen = strlen( pszSend );

			#ifdef _DEBUG
			  if ( cLen > 30 )
			  {
				chSave = pszSend[30];
				pszSend[30] = '\0';
				sprintf( szLog, "addResponseHeaders> Location: [%s...cont...]", pszSend );
				goauth_log( szLog );
				pszSend[30] = chSave;
			  }
			  else
			  {
				sprintf( szLog, "addResponseHeaders> Location: [%s]", pszSend );
				goauth_log( szLog );
			  }
			#endif

			pblock_nvinsert("url", pszSend, rq->vars);
		}
		
		/** ************************************************** Set-Cookie: */
		else if ( 0==strncmp( pszHeader, "Set-Cookie: ", 12 ) )
		{
			#ifdef _DEBUG
			  sprintf( szLog, "addResponseHeaders> Setting a cookie" );
			  goauth_log( szLog );
			#endif

			pszSend = &pszHeader[12];			

			#ifdef _DEBUG
			  if ( cLen > 30 )
			  {
				chSave = pszSend[30];
				pszSend[30] = '\0';
				sprintf( szLog, "addResponseHeaders> Set-cookie: [%s...cont...]", pszSend );
				goauth_log( szLog );
				pszSend[30] = chSave;
			  }
			  else
			  {
				sprintf( szLog, "addResponseHeaders> Set-Cookie: [%s]", pszSend );
				goauth_log( szLog );
			  }
			#endif

			pblock_nvinsert("set-cookie", pszSend, rq->srvhdrs);
		}   	


		pszHeader = 0;
		pszSend = 0;
	}
	
	return true;
}




/** ****************************************************************
 *
 * performAdminRequest: Hit AUTH with the given pszQry string to 
 *                      get back the data to send to the client. In 
 *                      this case Netscape isn't handling the request, 
 *                      we are - so write a response to the client.
 *
 **/
int performAdminRequest(  pblock * pb, Session * sn, Request * rq, char * pszQry )
{
        char * pszResp1 = "<html><body>Admin function failed...</body></html>\r\n";
	char * pszResp2 = "<html><body>Admin function no message body...</body></html>\r\n";
        char szUrl[2]         = {'\0'};
        char szQry[32]        = {'\0'};
        char szIpA[32]        = {'\0'};
        char szLog[256]       = {'\0'};
	char szCookie[2]      = {'\0'};

	CGoAuthResponse resp;
	bool bRedirect = false;


        #ifdef _DEBUG
	  sprintf( szLog, "performAdminRequest> begin" ); 	
 	  goauth_log( szLog );
	#endif

        sprintf( szQry, "%s", pszQry );
        sprintf( szIpA, "%s", g_szServerIp );
        sprintf( szUrl, "/" );


	#ifdef _DEBUG
	  sprintf( szLog, "performAdminRequest> Sending [%s, %s, %s]", szQry, szIpA, szUrl );
	  goauth_log( szLog );
	  sprintf( szLog, "performAdminRequest> calling authenticate()..." );
	  goauth_log( szLog );
	#endif


	g_pAuth->authenticate( szIpA, szUrl, szQry, szCookie, resp );

        
	#ifdef _DEBUG
	  sprintf( szLog, "performAdminRequest> after authenticate()..." );
          goauth_log( szLog );
	#endif


        /** Add headers to outgoing response... */
	#ifdef _DEBUG
	  sprintf( szLog, "performAdminRequest> Header count: [%d]", resp.getHeaderCount() );
          goauth_log( szLog );
	#endif

	
	param_free( pblock_remove("content-type", rq->srvhdrs ));
	param_free( pblock_remove("content-length", rq->srvhdrs ));



	if ( false == addResponseHeaders( pb, sn, rq, resp, &bRedirect ) )
	{
		return REQ_EXIT;
	}


	/* Set the response status if we didn't already..  */
	if ( 0 == resp.getHeaderCount() )
	{
		protocol_status(sn, rq, PROTOCOL_OK, NULL);
	}


	if ( protocol_start_response( sn, rq ) == REQ_NOACTION )
        {
		#ifdef _DEBUG
		  sprintf( szLog, "performAdminRequest> protocol_start_response == REQ_NOACTION" ); 
		  goauth_log( szLog );
		#endif

		return REQ_EXIT;
	}


	if ( true == resp.hasMessageBody() )
	{
		const char * psz = resp.getMessageBody();

		#ifdef _DEBUG
		  if ( 0 == psz )
		  {
			sprintf( szLog, "performAdminRequest> NULL message body" ); 
			goauth_log( szLog );
		  }
		  else
		  {
			sprintf( szLog, "performAdminRequest> non-NULL message body.." ); 
			goauth_log( szLog );	
		  }
		#endif

		if ( net_write( sn->csd, (char*)psz, strlen(psz)) == IO_ERROR )
        	{
	                return REQ_EXIT;
		}
	}
	else
	{
		if ( net_write( sn->csd, (char*)pszResp2, strlen(pszResp2)) == IO_ERROR )
        	{
	                return REQ_EXIT;
		}

		#ifdef _DEBUG
		  sprintf( szLog, "performAdminRequest> No message body.." ); 
		  goauth_log( szLog );
		#endif
	}


	#ifdef _DEBUG
	  sprintf( szLog, "performAdminRequest> end" ); 
	  goauth_log( szLog );
	#endif


	if ( true == bRedirect )
	{
		return REQ_ABORTED;
	}


        return REQ_PROCEED;
}



/** ****************************************************************
 *
 * performAdminRedirect: The request has been determined to be 
 *                       administrative.  Return the appropriate 
 *                       HTTP redirect - to this server - such 
 *                       that one of the admin functions in 
 *                       obj.conf will pick up the request.
 * eg:
 *     <Object ppath="/docroot/docs/goauth_reinit">
 *     Service fn="goauth-reinit"
 *     </Object>
 *
 **/
int performAdminRedirect( pblock * pb, Session * sn, Request * rq, char * pszQry, unsigned long cRequest )
{
        char szLog[256] = {'\0'};

        if ( 0==pszQry ) 
        { 
                return 0; 
        }

	#ifdef _DEBUG
	  sprintf( szLog, "performAdminRedirect [%06d]> [%s]" , cRequest, pszQry );
	  goauth_log( szLog );
	#endif


	if ( 0==strcmp( pszQry, "goauth_reinit"   ) ) 
        {
                return ( assignRedirect( pb, sn, rq, "/goauth_reinit", cRequest ) );
        }
        else if ( 0==strcmp( pszQry, "goauth_stats"    ) )
        {
                return ( assignRedirect( pb, sn, rq, "/goauth_stats", cRequest ) );
        }
        else if ( 0==strcmp( pszQry, "goauth_config"   ) ) 
        { 
                return ( assignRedirect( pb, sn, rq, "/goauth_config", cRequest ) ); 
        }
        else if ( 0==strcmp( pszQry, "goauth_verbose"  ) ) 
        { 
                return ( assignRedirect( pb, sn, rq, "/goauth_verbose", cRequest ) ); 
        }
        else if ( 0==strcmp( pszQry, "goauth_silent"   ) ) 
        {
                return ( assignRedirect( pb, sn, rq, "/goauth_silent", cRequest ) ); 
        }
        else if ( 0==strcmp( pszQry, "goauth_getgreen" ) )
        {
                return ( assignRedirect( pb, sn, rq, "/goauth_getgreen", cRequest ) ); 
        }
	else if ( 0==strcmp( pszQry, "goauth_version" ) )
        {
                return ( assignRedirect( pb, sn, rq, "/goauth_version", cRequest ) ); 
        }
	else if ( 0==strcmp( pszQry, "goauth_resetstats" ) )
        {
                return ( assignRedirect( pb, sn, rq, "/goauth_resetstats", cRequest ) ); 
        }

 	return REQ_PROCEED;
}




/** ****************************************************************
 *
 * requestIsAdmin: Is the request for something that we need to 
 *                 handle ourselves instead of letting Netscape 
 *                 handle it for/with us?
 *
 **/
int requestIsAdmin( const char * pszQry, unsigned long cRequest )
{
        char szLog[64] = {'\0'};

        if ( 0==pszQry ) { return 0; }
	if ( 0 !=strncmp( pszQry, "goauth_", 7 )) { return 0; }


	if ( 0==strcmp( pszQry, "goauth_verbose" ) )
	{
		#ifdef _DEBUG
		  sprintf( szLog, "# Switching to verbose mode..." );
		  goauth_log( szLog );
		#endif

		g_bVerbose = true;
	}


	if ( 0==strcmp( pszQry, "goauth_silent" ) )
	{
		#ifdef _DEBUG
		  sprintf( szLog, "# Switching to silent mode..." );
		  goauth_log( szLog );
		#endif

		g_bVerbose = false;
	}


	if (    ( 0==strcmp( pszQry, "goauth_reinit"     ) ) ||
		( 0==strcmp( pszQry, "goauth_stats"      ) ) ||
		( 0==strcmp( pszQry, "goauth_config"     ) ) ||
		( 0==strcmp( pszQry, "goauth_verbose"    ) ) ||
		( 0==strcmp( pszQry, "goauth_silent"     ) ) ||
		( 0==strcmp( pszQry, "goauth_getgreen"   ) ) ||
		( 0==strcmp( pszQry, "goauth_version"    ) ) ||
		( 0==strcmp( pszQry, "goauth_resetstats" ) ) 
	   )
	{
		#ifdef _DEBUG
		  sprintf( szLog, "requestIsAdmin [%06d]> [%s]" , cRequest, pszQry );
	          goauth_log( szLog );
		#endif

		return 1; 
	}

        return 0;
}



/** ****************************************************************
 *
 * assignRedirect
 *
 *
 **/
int assignRedirect( pblock * pb, Session * sn, Request * rq, char * pszUrl, unsigned long cRequest ) 
{
        char szUrl[1024] = {'\0'};
        char * pszServer = 0;

        if ( 0 == pszUrl )
	{
		pblock_nvinsert("error", "assignRedirect> Missing Url. Aborting request.", pb );

		#ifdef _DEBUG
		  sprintf( szUrl, "assignRedirect [%06d]> ERROR. NULL Url to redirect to. Aborting request.", cRequest );
		  goauth_log( szUrl );
		#endif

		return REQ_ABORTED;
	}

	if ( strlen(pszUrl) > 950 )
	{
		pblock_nvinsert("error", "assignRedirect> Redirect Url too long. Aborting request.", pb );
		
		#ifdef _DEBUG
		  sprintf( szUrl, "assignRedirect [%06d]> ERROR. Url too long to redirect to (>950bytes). Truncating.", cRequest );
		  goauth_log( szUrl );
		#endif

		return REQ_ABORTED;		
	}


	#ifdef _DEBUG
	  sprintf( szUrl, "assignRedirect [%06d]> Url: [%s]", cRequest, pszUrl );
          goauth_log( szUrl );
	#endif

        if ( 0 != strcmp( g_szServerPort, "80" ) )
        {
                sprintf( szUrl, "http://%s:%s%s", g_szServerName, g_szServerPort, pszUrl );
        }
        else
        {
                sprintf( szUrl, "http://%s%s", g_szServerName, pszUrl );
        }

        /* Set the return code to 302 Redirect */
        protocol_status(sn, rq, PROTOCOL_REDIRECT, NULL);


        /* The error handling functions use this to set the Location: */
        pblock_nvinsert("url", szUrl, rq->vars);

	#ifdef _DEBUG
	  sprintf( szUrl, "assignRedirect [%06d]> Location: [%s]", cRequest, szUrl );
          goauth_log( szUrl );
	#endif

        return REQ_ABORTED;          
}




/** ***************************************************************
 *
 * goauth_terminate.  Close the log file handle and invalidate 
 *                it's value.
 *
 **/
void goauth_terminate(void * parameter )
{
        system_fclose(g_fd);
        g_fd = SYS_ERROR_FD;

	if ( 0 != g_pAuth )
	{
		delete g_pAuth;
	}

	g_pAuth = 0;

	g_bInit = false;

	g_csLogging.Destroy();

        return;
}


/** ****************************************************************
 *
 * goauth_log. Write an entry out to the log file with a \r\n  
 *         appended to the end.
 *
 **/
void goauth_log( char * psz )
{
        char szBuffer[512] = {'\0'};
        unsigned int cLen = 0;

        if ( 0==psz ) return;
	if ( false == g_bVerbose ) return;

        if ( SYS_ERROR_FD != g_fd )
        {
                cLen = strlen( psz );

                if ( cLen > 490 )
                {
                        memcpy( szBuffer, psz, 490 );
                        strcat( szBuffer, "...trunc...\n" );
                }
                else
                {
                        sprintf( szBuffer, "%s\n", psz );
                }

		g_csLogging.Lock();
		system_fwrite( g_fd, szBuffer, strlen(szBuffer) ); 
		g_csLogging.Unlock();
        }

        return;
}



/** ****************************************************************
 *
 * goauth_init.  Open a log file and initialize the libGoAuth.so for 
 *           authentication.  Retrieve the 'iniFile', 'iniDir', 
 *           and 'logFile' entries from the magnus.conf file.
 *           Also retrive 'serverIp', 'serverName', and 'serverPort'.
 *
 **/
extern "C" NSAPI_PUBLIC int goauth_init( pblock * pb, Session * sn, Request * rq )
{
	char szRenameFrom[256] = {'\0'};
	char szRenameTo[256]   = {'\0'};
	char szLog[256]        = {'\0'};
	char szErr[512]        = {'\0'};

	int i;
        char * pszIniFile = 0;
        char * pszIniDir  = 0;
        char * pszLogFile = 0;

        char * pszIp   = 0;
        char * pszName = 0;
        char * pszPort = 0;


	/** Initialize the logging critical section */
	g_csLogging.Init();


	/** Retrieve 'AuthTrans' parameters from magnus.conf file... */
        pszIniFile  = pblock_findval( "iniFile", pb );
        pszIniDir   = pblock_findval( "iniDir",  pb );
        pszLogFile  = pblock_findval( "logFile",  pb );


        if ( 0==pszIniFile || 0==pszIniDir || 0==pszLogFile ) 
        {
		pblock_nvinsert("error", "goauth-init> Missing 'iniDir' and/or 'iniFile' and/or 'logFile'. Aborting request.", pb);
                return REQ_ABORTED;
        }


	/** Rename the last 9 files... */
	for ( i=9; i>=0; i-- )
        {
                sprintf( szRenameFrom, "%s/GoAuthNsapi.log.%02d", pszIniDir, i     );
                sprintf( szRenameTo,   "%s/GoAuthNsapi.log.%02d", pszIniDir, (i+1) );
                rename( szRenameFrom, szRenameTo );
        }

	
	/** And the last one... */
	sprintf( szRenameFrom, "%s/GoAuthNsapi.log", pszIniDir    );
        sprintf( szRenameTo,   "%s/GoAuthNsapi.log.00", pszIniDir );
        rename( szRenameFrom, szRenameTo );


        /** Open the new log file */
        sprintf( szRenameTo, "%s/GoAuthNsapi.log", pszIniDir );
	g_fd = system_fopenWA( szRenameTo );


        if ( SYS_ERROR_FD == g_fd ) 
        {
                sprintf( szErr, "goauth-init> unable to open log file [%s]", szRenameTo );
		pblock_nvinsert("error", szErr, pb);
                return REQ_ABORTED;
        }


	sprintf( szLog, "# GoAuthNSAPI log begin\n" );
	goauth_log( szLog );


        /** Have Netscape call the go_terminate function upon restart */
        magnus_atrestart(goauth_terminate, NULL);


        /** Log my start-up params... */
	sprintf( g_szIniDir,  "%s", pszIniDir  );
	sprintf( g_szIniFile, "%s", pszIniFile );
	sprintf( g_szLogFile, "%s", pszLogFile );


	sprintf( szLog, "goauth-init> iniDir: %s", g_szIniDir );
	goauth_log( szLog );
	sprintf( szLog, "goauth-init> iniFile: %s", g_szIniFile );
	goauth_log( szLog );
	sprintf( szLog, "goauth-init> logFile: %s", g_szLogFile );
	goauth_log( szLog );



        /** Retrieve server info from magnus.conf */
        pszIp   = pblock_findval( "serverIp",    pb );
        pszName = pblock_findval( "serverName",  pb );
        pszPort = pblock_findval( "serverPort",  pb );

        if ( 0==pszIp || 0==pszName || 0==pszPort ) 
        {
		pblock_nvinsert("error", "goauth-init> Missing 'serverName' and/or 'serverIp', and/or 'serverPort'. Abporting request.", pb);

                sprintf( szLog, "goauth-init> ERROR. Missing 'serverName' or 'serverIp' or 'serverPort'. Aborting request. ");
                goauth_log( szLog );
                return REQ_ABORTED;
        }

        sprintf( g_szServerIp,   "%s", pszIp   );
        sprintf( g_szServerName, "%s", pszName );
        sprintf( g_szServerPort, "%s", pszPort );

	sprintf( szLog, "goauth-init> ip: %s", g_szServerIp );
	goauth_log( szLog );
	sprintf( szLog, "goauth-init> name: %s", g_szServerName);
	goauth_log( szLog );
	sprintf( szLog, "goauth-init> port: %s", g_szServerPort );
	goauth_log( szLog );



	sprintf( szLog, "goauth-init> Creating new instance of CGoauth" );
        goauth_log( szLog );
	g_pAuth = new CGoAuth();



        /** Perform authentication initialization */
        sprintf( szLog, "goauth-init> calling go_authenticate_init()" );
        goauth_log( szLog );
       
        if ( true == g_pAuth->init( pszIniDir, pszIniFile, pszLogFile ) )
	{
		sprintf( szLog, "goauth-init> initialization success." );
	        goauth_log( szLog );

	        g_bInit = true;

		#ifdef _DEBUG
		  sprintf( szLog, "goauth-init> GoAuthNSAPI log is verbose" );
	          goauth_log( szLog );
		#else
		  sprintf( szLog, "goauth-init> GoAuthNSAPI log is silent" );
	          goauth_log( szLog );
		#endif

		sprintf( szLog, "goauth-init> end" );
        	goauth_log( szLog );

	        return REQ_PROCEED;
	}
	else
	{
		pblock_nvinsert("error", "goauth-init> Initialization failure", pb);

		sprintf( szLog, "goauth-init> ERROR. initialization failure." );
		goauth_log( szLog );
		
        	g_bInit = false;

		#ifdef _DEBUG
		  sprintf( szLog, "goauth-init> GoAuthNSAPI log is verbose" );
	          goauth_log( szLog );
		#else
		  sprintf( szLog, "goauth-init> GoAuthNSAPI log is silent" );
	          goauth_log( szLog );
		#endif

		sprintf( szLog, "goauth-init> end" );
	        goauth_log( szLog );

        	return REQ_ABORTED;
	}
}



/** ****************************************************************
 *
 * goauth-admin
 *
 **/
extern "C" NSAPI_PUBLIC int goauth_admin( pblock * pb, Session * sn, Request * rq )
{
 	char * pszFnc = 0;
	char szLog[128] = {'\0'};

	if ( false==g_bInit )
        {
                pblock_nvinsert("error", "goauth-admin> call init() before invoking methods", pb);
		return REQ_ABORTED;
        }

	if ( 0 == g_pAuth )
	{
		pblock_nvinsert("error", "goauth-admin> init() failed. call init() before invoking methods", pb);

		#ifdef _DEBUG
		  sprintf( szLog, "goauth_admin> ERROR. Global Auth pointer is NULL!" );
        	  goauth_log( szLog );
		#endif

		return REQ_ABORTED;
	}

	pszFnc = pblock_findval( "adminFunc", pb );

	if ( 0==pszFnc )
	{
		pblock_nvinsert("error", "goauth-admin> Missing 'adminFunc' parameter. Aborting request.", pb );

		#ifdef _DEBUG
		  sprintf( szLog, "goauth_admin> ERROR. Missing 'adminFunc' parameter. Aborting request." );
		  goauth_log( szLog );
		#endif
	}
	else
	{
		return ( performAdminRequest( pb, sn, rq, pszFnc ) );
	}
}



/** ****************************************************************
 *
 * goauth-response
 *
 **/
extern "C" NSAPI_PUBLIC int goauth_response( pblock * pb, Session * sn, Request * rq ) 
{
        const char * psz = "<html><body>test message...</body></html>\r\n";

	char szResponse[128] = {'\0'};
        char szLog[128]      = {'\0'};
	char szNum[20]       = {'\0'};


        if ( false==g_bInit )
        {
                pblock_nvinsert("error", "goauth-response> call init() before invoking methods", pb);
		return REQ_ABORTED;
        }

	if ( 0 == g_pAuth )
	{
		pblock_nvinsert("error", "goauth-response> init() failed. call init() before invoking methods", pb);

		#ifdef _DEBUG
		  sprintf( szLog, "goauth-response> ERROR. Global Auth pointer is NULL!" );
        	  goauth_log( szLog );
		#endif

		return REQ_ABORTED;
	}

	#ifdef _DEBUG
          sprintf( szLog, "go-response> begin" );
          goauth_log( szLog );
	#endif

        sprintf( szResponse, "%s", psz );
        sprintf( szNum, "%d", strlen(psz) );

        param_free( pblock_remove("content-type", rq->srvhdrs ));
        param_free( pblock_remove("content-length", rq->srvhdrs ));

        protocol_status( sn, rq, PROTOCOL_OK, NULL );

        pblock_nvinsert( "content-type", "text/html", rq->srvhdrs );
        pblock_nvinsert( "content-length", szNum, rq->srvhdrs );

        if ( protocol_start_response( sn, rq ) == REQ_NOACTION )
        {
                pblock_nvinsert("error", "goauth-response> failure on protocol_start_response()", pb );
		return REQ_EXIT;
        }

        if ( net_write( sn->csd, szResponse, strlen(szResponse)) == IO_ERROR )
        {
                pblock_nvinsert("error", "goauth-response> failure on net_write()", pb);
		return REQ_EXIT;
        }

	#ifdef _DEBUG
          sprintf( szLog, "go-response> End" );
          goauth_log( szLog );
	#endif

        return REQ_PROCEED;
}



/** ****************************************************************
 *
 * goauth_auth: Retrieve the data necessary to authenticate the user 
 *           and call g_authenticate (part of libGoAuth.so).  Examine 
 *           the response struct and act accordingly.
 *
 **/
extern "C" NSAPI_PUBLIC int goauth_auth( pblock * pb, Session * sn, Request * rq )
{
        CGoAuthResponse resp;

        unsigned long cRequest = 0;
        unsigned int cHeaders  = 0;
        unsigned int i         = 0;

	unsigned int cIpLen  = 0;
	unsigned int cUrlLen = 0;
	unsigned int cQryLen = 0;
	unsigned int cCookieLen = 0;

	char chSave;

        char szUrl[513]      = {'\0'};
        char szIpA[32]       = {'\0'};
        char szQry[2048]      = {'\0'};
        char szCookie[4096]  = {'\0'};
        char szLog[256]      = {'\0'};

        char * pszIp      = 0;
        char * pszUrl     = 0;
        char * pszQry     = 0;
        char * pszCookie  = 0;

    	bool bRedirect = false;




	/** *********************** We can't auth if we haven't been initialized properly */
        if ( false==g_bInit )
        {
                pblock_nvinsert("error", "auth> call init() before invoking methods", pb);
		return REQ_ABORTED;
        }

	if ( 0 == g_pAuth )
	{
		pblock_nvinsert("error", "auth> init() failed. call init() before invoking methods", pb);
		
		#ifdef _DEBUG
		  sprintf( szLog, "go-auth [%06d]> ERROR. Global Auth pointer is NULL!", cRequest );
        	  goauth_log( szLog );
		#endif

		return REQ_ABORTED;
	}



        /** **************************************************************** Log the hit */
        cRequest = (++g_cCount);

        #ifdef _DEBUG
	  sprintf( szLog, "go-auth [%06d]> begin", cRequest );
          goauth_log( szLog );
	#endif



        /** ******************************************** Retrieve the servers IP address */      
        pszIp = pblock_findval( "ip", pb ); /* g_szServerIp; */

	if ( 0 != pszIp )
        {
                cIpLen = strlen( pszIp );
                if ( cIpLen > 30 )
                {
                        pblock_nvinsert("error", "auth> Invalid IP address. Aborting request.", pb);

			#ifdef _DEBUG
			  sprintf( szLog, "go-auth [%06d]> ERROR. IP address longer than 30 bytes. Aborting request", cRequest );
                          goauth_log( szLog );
			#endif                        

			return REQ_ABORTED;
                }
                else
                {
                        sprintf( szIpA, "%s", pszIp );
                }
        }



	/** ********************************************** Retrieve the users request Url */
        pszUrl  = pblock_findval( "uri", rq->reqpb );

	if ( 0 != pszUrl )
        {
                cUrlLen = strlen( pszUrl );
                if ( cUrlLen > 512 )
                {
                        pblock_nvinsert("error", "auth> Invalid Url. Aborting request.", pb);
			
			#ifdef _DEBUG
			  sprintf( szLog, "go-auth [%06d]> ERROR. Url longer than 512 bytes. Aborting request.", cRequest );
                          goauth_log( szLog );
                        #endif

			return REQ_ABORTED;
                }
                else
                {
                        sprintf( szUrl, "%s", pszUrl );
                }
        }

	
	/** Unencode the Url -- THERE IS NO NEED TO DO THIS -- Netscape has done it for us.. */
	/** URLDecodeString( szUrl ); */

	/** Verify the format of the URL */
	URLVerifyFormat( szUrl );




	/** ********************************************** Retrieve the users query string data */
        pszQry = pblock_findval( "query", rq->reqpb );

        if ( 0 != pszQry )
        {
                cQryLen = strlen( pszQry );
                if ( cQryLen > 2048 )
                {
                        pblock_nvinsert("error", "auth> Invalid Query string. Aborting request.", pb);
			
			#ifdef _DEBUG
			  sprintf( szLog, "go-auth [%06d]> ERROR. Query string longer than 2048 bytes. Aborting request.", cRequest );
                          goauth_log( szLog );
                        #endif

			return REQ_ABORTED;
                }
                else
                {
                        sprintf( szQry, "%s", pszQry );
                }
        }


	/** Unencode the Query String...  
	  *
	  * This needs to be done this because Netscape will encode it on the way out... 
	  * 
          * If my query string comes in as:       rate=33%25 
          * this value needs to be unencoded to:  rate=33%
	  *
          * ..so that when it's sent back out NS will convert it back to: rate=33%25
	  *
          **/
	URLDecodeString( szQry ); 


	

	/** ********************************************** Retrieve users cookie data */
        pszCookie = pblock_findval( "cookie",rq->headers );

	if ( 0 != pszCookie )
        {
                cCookieLen = strlen( pszCookie );
                if ( cCookieLen > 4095 )
                {
                        pblock_nvinsert("error", "auth> Invalid cookie data. Aborting request.", pb);

			#ifdef _DEBUG
			  sprintf( szLog, "go-auth [%06d]> ERROR. Cookie data string longer than 4k bytes. Aborting request.", cRequest );
                          goauth_log( szLog );
                        #endif

			return REQ_ABORTED;
                }
                else
                {
                        sprintf( szCookie, "%s", pszCookie );
                }
        }



 	/** ******************************************************** Log user data */
	#ifdef _DEBUG
	  sprintf( szLog, "go-auth [%06d]> IpA:    [%s]", cRequest, szIpA );
          goauth_log( szLog );
	#endif


	#ifdef _DEBUG
	  if ( cUrlLen > 50 )
	  {
		chSave = szUrl[50];
		szUrl[50] = '\0';
		sprintf( szLog, "go-auth [%06d]> Url: [%s...etc...]", cRequest, szUrl );
        	goauth_log( szLog );
		szUrl[50] = chSave;
	  }
	  else
	  {
		sprintf( szLog, "go-auth [%06d]> Url:    [%s]", cRequest, szUrl );
        	goauth_log( szLog );
	  }
	#endif


	#ifdef _DEBUG
	  if ( cQryLen > 50 )
	  {
		chSave = szQry[50];
		szQry[50] = '\0';
		sprintf( szLog, "go-auth [%06d]> QryStr: [%s...etc...]", cRequest, szQry );
        	goauth_log( szLog );
		szQry[50] = chSave;
	  }
	  else
	  {
		sprintf( szLog, "go-auth [%06d]> QryStr: [%s]", cRequest, szQry );
        	goauth_log( szLog );
	  }
	#endif


	#ifdef _DEBUG
	  if ( cCookieLen > 50 )
	  {
		chSave = szCookie[50];
		szCookie[50] = '\0';
		sprintf( szLog, "go-auth [%06d]> Cookie: [%s...etc...]", cRequest, szCookie );
        	goauth_log( szLog );
		szCookie[50] = chSave;
	  }
 	  else
	  {
		sprintf( szLog, "go-auth [%06d]> Cookie: [%s]", cRequest, szCookie );
        	goauth_log( szLog );
	  }
	#endif


      

	/** **************************************** See if it's an ADMIN request */
        if ( requestIsAdmin( szQry, cRequest ) )
        {
                return ( performAdminRedirect( pb, sn, rq, szQry, cRequest ) );
        }


	/** ********************************************* Perform authentication */
	#ifdef _DEBUG
	  sprintf( szLog, "go-auth [%06d]> calling authenticate()...", cRequest );
	  goauth_log( szLog );
	#endif


	g_pAuth->authenticate( szIpA, szUrl, szQry, szCookie, resp );


	#ifdef _DEBUG
          sprintf( szLog, "go-auth [%06d]> after authenticate()...", cRequest );
          goauth_log( szLog );
     	#endif

	

	/** **************************************************************************** */
        /* The response is not complete... if there are any headers as part of AUTHs     */
        /* response, add them to the outgoing response...                                */
        /** **************************************************************************** */
	if ( GO_AUTH_REQUEST_CONTINUE == resp.getStatus() )
	{
		#ifdef _DEBUG
		  sprintf( szLog, "go-auth [%06d]> status: GO_AUTH_REQUEST_CONTINUE", cRequest );
       		  goauth_log( szLog );
		  sprintf( szLog, "go-auth [%06d]> header count: %d ", cRequest, resp.getHeaderCount() );
        	  goauth_log( szLog );
		  sprintf( szLog, "go-auth [%06d]> End.", cRequest );
        	  goauth_log( szLog );
		#endif

		addResponseHeaders( pb, sn, rq, resp, &bRedirect );

		if ( true == bRedirect )
		{
			return REQ_ABORTED;
		}

		return REQ_PROCEED;
	}
	



	/** **************************************************************************** */
        /* The response is complete... this is most definately a 302 - but only because  */
        /* we took care of the ADMIN functions earlier (to *really* handle the request)  */
        /*                                                                               */
        /* Parse out the location and return the HTTP redirect...                        */
        /** **************************************************************************** */
	if ( GO_AUTH_REQUEST_COMPLETE == resp.getStatus() )
	{
		#ifdef _DEBUG
		  sprintf( szLog, "go-auth [%06d]> status: GO_AUTH_REQUEST_COMPLETE", cRequest );
        	  goauth_log( szLog );
  		  sprintf( szLog, "go-auth [%06d]> header count: %d ", cRequest, resp.getHeaderCount() );
        	  goauth_log( szLog );

	  	  if ( true == resp.hasMessageBody() )
		  {
			sprintf( szLog, "go-auth [%06d]> has message body", cRequest );
        		goauth_log( szLog );
		  }
		  else
		  {
			sprintf( szLog, "go-auth [%06d]> does not have message body", cRequest );
        		goauth_log( szLog );
		  }
		#endif

		addResponseHeaders( pb, sn, rq, resp, &bRedirect );

		#ifdef _DEBUG
		  sprintf( szLog, "go-auth [%06d]> End.", cRequest );
        	  goauth_log( szLog );
		#endif

		if ( true == bRedirect )
		{
			return REQ_ABORTED;
		}

	        return REQ_PROCEED;
	}



	#ifdef _DEBUG
	  sprintf( szLog, "go-auth [%06d]> ERROR. End. This code should never execute.", cRequest );
          goauth_log( szLog );
        #endif

	return REQ_PROCEED;
}






















