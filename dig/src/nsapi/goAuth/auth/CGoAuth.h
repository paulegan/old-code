//--file: CGoAuth.h--------------------------------------------------
//
// Declaration for a class that will perform authentication 
// for a Web server which is serving content for one or more 
// IP addresses.
//
// Initialization occurs via a directory name and an .ini 
// file,  the .ini file specifying the IP addresses that 
// the class will authenticate for.
//
// For each IP address,  an XML ini file is parsed and the 
// parameters used to perform authentication for the IP 
// address are read in.
//
// To authenticate,  call method "authenticate" with the 
// following parameters:
// 
//      Requested IP address
//      Requested Url
//      Query string
//      Cookie data
//      A reference to a CGoAuthResponse instance
//
// The client should then examine the CGoAuthResponse 
// instance to determine what action is required.
//
// A Query string of "?reinit" will cause reinitialization 
// of GoAuth from it's ini file.
//
//
#ifndef _CGoAuth_h_
#define _CGoAuth_h_

#include "./../utils/platform_defines.h"
#include "./../utils/CLog.h"
#include "CGoAuthIPCollection.h"


class CSimpleXMLParser;
class CGoAuthResponse;


class CGoAuth
{
	public:

		CGoAuth();

		~CGoAuth();

		/** Working directory,  .ini filename,  .log filename */
		bool init( const char *, const char *, const char * );

		/** Requested IP address, Requested Url, Query string, Cookie data, Response */
		bool authenticate( const char *, const char *, const char *, char *, CGoAuthResponse & );
		
		/** Close all file handles, deallocate Premium Areas */
		bool deinit();

		/** Has GoAuth been initialized for authentication */
		bool initialized() const { return m_bInitialized; }


	private:
		/** Initialize the active IP instance */
		bool initializeIP( const char *, const char * );

		/** Switch the active IP instance */
		bool toggleIPs();

		/** See if the request is for something administrative */
		bool requestIsAdmin( const char * ) const;
		
		/** Handle a request for administrative functionality */
		bool handleAdminRequest( unsigned long, int, const char *, const char *, const char *, CGoAuthResponse & );
		
		/** Administrative functions */
		bool handleReinitRequest( unsigned long, int, const char *, const char *, const char *, CGoAuthResponse & );
		bool handleStatsRequest( unsigned long, int, const char *, const char *, const char *, CGoAuthResponse & );
		bool handleConfigRequest( unsigned long, int, const char *, const char *, const char *, CGoAuthResponse & );
		bool handleGreenRequest( unsigned long, int, const char *, const char *, const char *, CGoAuthResponse & );
		bool handleRunModeRequest( unsigned long, int, const char *, const char *, const char *, CGoAuthResponse & );
		bool handleVersionRequest( unsigned long, int, const char *, const char *, const char *, CGoAuthResponse & );
		bool handleStatsReinitRequest( unsigned long, int, const char *, const char *, const char *, CGoAuthResponse & );

		
		/** Lock/unlock the statis critical section */
		inline void LockStats()   { m_mutexStats.Lock();   }
		inline void UnlockStats() { m_mutexStats.Unlock(); }

		inline void LockReinit()   { m_mutexReinit.Lock();   }
		inline void UnlockReinit() { m_mutexReinit.Unlock(); }



	private:
		/** Total Hit count */
		unsigned long m_cTotalHitCount;
		unsigned long m_cHitCount;
		
		/** Hits not serviced... */
		unsigned long m_cHitsNotServiced;
		
		unsigned long m_cHitsNotServicedInvalidRequest;
		unsigned long m_cHitsNotServicedAuthUninit;
		unsigned long m_cHitsNotServicedBadIPRequest;
		
	 	/** Hits serviced... */
        unsigned long m_cHitsServiced;

        unsigned long m_cHitAdminRequest;
        unsigned long m_cHitUserRequest;

        unsigned long m_cHitNonPremiumNonFree;
        unsigned long m_cHitLoginUpsellAccessDenied;
        unsigned long m_cHitFree;
        unsigned long m_cHitPremium;

        unsigned long m_cHitUpsell;
        unsigned long m_cHitGrantAccess;
        unsigned long m_cHitAccessDenied;
        unsigned long m_cHitUserLogin;
		unsigned long m_cHitSignupUser;
		

	private:
		/** A global logger */
		CLog * m_pLog;

		/** A global timer */
		CTime * m_pTime;

		/** Stats critical section */
		swport_mutex m_mutexStats;

		/* Reinitialization critical section */
		swport_mutex m_mutexReinit;

		/** Two sets of IPs to use - switch back and forth between them */
		CGoAuthIPCollection m_IPs[2];
	
		/** A pointer to the IP currently in use */
		CGoAuthIPCollection * m_pIP;

		char m_szDir[300];
		char m_szIni[300];
		
		bool m_bInitialized;

		char m_szStartTime[65];
		char m_szLastReinitTime[65];
		char m_szLastStatsResetTime[65];

		bool m_bVerbose;

};

#endif
