//--file: CLog.cpp---------------------------------------------------
//
#include "CLog.h"
#include "swport_mutex.h"


swport_mutex  CLog::m_Lock;
CTime         CLog::m_Time;
FILE  *       CLog::m_pFile = 0;
bool          CLog::m_bInitialized = false;
char          CLog::m_szAppName[257];
unsigned long CLog::m_ulCounter = 0;


#ifdef __OS_WINNT__
	static char * g_pszFileDelim = "\r\n";
#elif defined __OS_SOLARIS__
	static char * g_pszFileDelim = "\n";
#endif




/** *****************************************************************
 *
 * CLog::create
 *
 **/
void CLog::create()
{
	CLog::m_Lock.Init();	
}


/** *****************************************************************
 *
 * CLog::destroy
 *
 **/
void CLog::destroy()
{
	CLog::m_Lock.Lock();
	
	/** Close the log file if it's open... */
	if ( 0 != CLog::m_pFile )
	{
		CLog::m_bInitialized = false;
		fclose( CLog::m_pFile );
		CLog::m_pFile = 0;
	}

	CLog::m_Lock.Unlock();

	CLog::m_Lock.Destroy();
}



/** *****************************************************************
 *
 * CLog::init
 *
 * Moves the filename to a new name if it exists.
 * Try to initialize the new file stream.
 *
 **/
bool CLog::init( const char * pszAppName, const char * pszFilename )
{
	char szRenameTo[300]    = {'\0'};
	char szRenameFrom[300]  = {'\0'};
	char szOldFileName[300] = {'\0'};
	char szDate[65]         = {'\0'};


	CLog::m_Lock.Lock();
	
	
	if ( 0==pszAppName || 0==pszFilename )
	{
		CLog::m_Lock.Unlock();
		return false;
	}


	/** Store the application name,  eg: "goauth" */
	if ( strlen( pszAppName ) > 256 )
	{
		for ( int i=0; i<256; i++ )
			CLog::m_szAppName[i] = pszAppName[i];
	}
	else
	{
		strcat( CLog::m_szAppName, pszAppName );
	}



	/** Create the old file name,  eg:  "GoAuth.log"  */
	if ( strlen( pszFilename ) > 295 )
	{
		for ( int i=0; i<295; i++ )
			szOldFileName[i] = pszFilename[i];
	}
	else
	{
		strcat( szOldFileName, pszFilename );
	}



	/** Keep the last 10 log files around...      */
	/** Rename "GoAuth.log.09 to GoAuth.log.10    */
	for ( int i=9; i>=0; i-- )
	{
		sprintf( szRenameFrom, "%s.%02d", szOldFileName, i   );
		sprintf( szRenameTo,   "%s.%02d", szOldFileName, i+1 );
		rename( szRenameFrom, szRenameTo );
	}



	/** And now do the last file...            */
	/** Rename "GoAuth.log to GoAuth.log.00    */
	sprintf( szRenameFrom, "%s",    szOldFileName );
	sprintf( szRenameTo,   "%s.00", szOldFileName );
	rename( szRenameFrom, szRenameTo );
	


	/** Create the new file,, eg: "GoAuth.log"  */
	CLog::m_pFile = fopen( szOldFileName, "w" );
	
	if ( 0 == CLog::m_pFile )
	{
		CLog::m_Lock.Unlock();
		return false;
	}


	/** The log file is valid */
	CLog::m_bInitialized = true;


	/** Use szFileName inappropriately as a buffer */
	CLog::m_Time.getLocalDateTime( szDate, 64 );
	sprintf( szOldFileName, "# Beginning %s log [%s]\r\n", pszAppName, szDate );


	/** Write header out to log file... */
	CLog::write( szOldFileName );
	
	
	CLog::m_Lock.Unlock();
	return true;
}




/** *****************************************************************
 *
 * Clog::write.  This occurrs within the Lock()/Unlock() already.
 *
 **/
bool CLog::write( const char * psz )
{
	if ( 0 == CLog::m_pFile) return false;
	if ( 0 == psz ) return false;

	fprintf( CLog::m_pFile, psz );
	fflush( CLog::m_pFile );
	
	return true;	
}




/** *****************************************************************
 *
 * CFilterLog::log
 *    Logs a message of the form:
 *
 *  <Application name> [Message priority level]:<Date/time> Message string...
 *
 **/
bool CLog::log( const char * pszLogString )
{
    char szDate[65] = {'\0'};  
	char fileBuffer[ g_cChunkSize + 128 ] = {'\0'};
    int cChunks = 0;
    int cRemainder = 0;
    int cLength = 0;
	int cCurrentLength = 0;

	    
	CLog::m_Lock.Lock();
	
	/** Early outs... */
	if ( false == CLog::m_bInitialized ) 
	{
		CLog::m_Lock.Unlock();	
		return false;
	}
	
	if ( 0 == pszLogString ) 
	{
		CLog::m_Lock.Unlock();
		return false;
	}

	
	/** Every 500 lines write out the date/time stamp */
	if ( (++CLog::m_ulCounter) >= 500 )
	{
		CLog::m_Time.getLocalDateTime( szDate, 64 );
		sprintf( fileBuffer, "%s[%s]%s%s", g_pszFileDelim, szDate, g_pszFileDelim, g_pszFileDelim );
		CLog::write( fileBuffer );
		CLog::m_ulCounter = 0;
	}


    /** Get the number of chunks... */
    cLength = strlen( pszLogString );
	cRemainder = ( cLength % g_cChunkSize );
    cChunks = (int)( cLength - cRemainder ) / g_cChunkSize;
     

    /** If there are some "g_cChunkSize" chunks to write out... */
    for ( int i=0; i<(cChunks); i++ )
    {
        memset( fileBuffer, '\0', g_cChunkSize + 128 );
		cCurrentLength = 0;
		
        for ( int c=0; c<g_cChunkSize; c++ )
        {
		    fileBuffer[ cCurrentLength + c ] = pszLogString[ c + g_cChunkSize*i ];
        }
		        
    	strcat( fileBuffer, g_pszFileDelim );
		CLog::write( fileBuffer );
	}

    
    /** Write out the rest of the string... */
    if ( cRemainder>0 )
    {
        memset( fileBuffer, '\0', g_cChunkSize + 128 );
		cCurrentLength = 0;

        for ( int c=0; c<cRemainder; c++ )
        {
		   fileBuffer[ cCurrentLength + c ] = pszLogString[ c + cChunks * g_cChunkSize ];
        }
		        
    	strcat( fileBuffer, g_pszFileDelim );
		CLog::write( fileBuffer );
    }


	CLog::m_Lock.Unlock();

	return true;
}

