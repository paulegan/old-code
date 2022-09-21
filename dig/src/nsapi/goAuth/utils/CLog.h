//--file: CLog.h-----------------------------------------------------
//
// A log file - not thread safe yet - that writes out messages 
// in 1KB chunks.
//
#ifndef _CLog_h_
#define _CLog_h_

#include <stdio.h>
#include <string.h>
#include "CTime.h"
#include "swport_mutex.h"
#include "platform_defines.h"


const int g_cChunkSize = 1024;

class CLog
{
	public:

		// Call 1X
		static void create();
		
		// Call as many times as necessary until it returns 'true'...
		static bool init( const char *, const char * );

		// Call as many times as you'd like
		static bool log ( const char * );
		
		// Call 1X
		static void destroy();
		
	
	
	private:
		static bool write( const char * );
		
		static swport_mutex   m_Lock;
		static FILE *         m_pFile;
		static CTime          m_Time;
		static bool           m_bInitialized;
		static char           m_szAppName[257];
		static unsigned long  m_ulCounter;

};

#endif

