//--file: CGoAuthIPCollection.h--------------------------------------
//
#ifndef _CGoAuthIPCollection_h_
#define _CGoAuthIPCollection_h_

#include "./../utils/platform_defines.h"
#include "./../utils/CInterpreter.h"
#include "./../xml/CSimpleXMLParser.h"

#include "CIP.h"


const int g_cMaxIPs = 32;


class CPremiumArea;
class CGoAuthResponse;
class CXMLNode;


class CGoAuthIPCollection
{
	public:

		CGoAuthIPCollection();
		~CGoAuthIPCollection();

		/** Working directory,  .ini filename */
		bool init( const char *, const char *, bool *  );
		
		/** Destroy the IPs and their Premium Areas */
		bool destroy();

		/** See if the given IP is contained within the list */
		bool containsRequestIP( const char *, int * ) const;

		/** Attempt authentication */
		bool authenticate( unsigned long, unsigned int, const char *, const char *, const char *, char *, CGoAuthResponse &, bool );
		
		/** Check to see if the Url is an Auth Redirect Page */
		bool isAuthRedirectPage( unsigned int, const char * ) const;
		

		/** Accessors */
		unsigned int getIpCount() const { return m_cIP; }
		const char * getIp( unsigned int idx ) const { return m_pszIP[idx]; }

		/** Get config info */
		bool getConfigInfo( char *, unsigned int ) const;

		/** Set a green in the appropriate domain */
		bool setGreen( int, CGoAuthResponse & ) const; 



	private:
		
		/** Open up/parse an XML file for the given IP address */
		bool parseIPAddress( const char *, const char *, char * );

		/** Look for registered SWID (with curly brace) in the given cookie string */
		bool parseForSwid( const char * );

		/** Convert XML parse tree to Premium Area */
		bool xml2IP( const char *, const CXMLNode * );

		/** Redirect a user that has no GREEN */
		bool returnUserHasNoGreen( unsigned long, unsigned int, char *, CPremiumArea *, const char *, const char *, CGoAuthResponse &, bool ) const;

		/** Redirect a user that needs to be upsold... */
		bool returnUpsellTheUser( unsigned long, unsigned int, char *, CPremiumArea *, const char *, const char *, CGoAuthResponse &, bool ) const;

		/** Redirect a user that has been denied access... */
		bool returnUserAccessDenied( unsigned long, unsigned int, char *, CPremiumArea *, const char *, const char *, CGoAuthResponse &, bool ) const;

		/** Redirect a user that has not registered... */
		bool returnSignupUser( unsigned long, unsigned int, char *, CPremiumArea *, const char *, const char *, CGoAuthResponse &, bool ) const;

		/** check config params for validity */
		bool checkVals( char *, char *, char *, char *, char *, char *, char * ) const;

		/** Given the required data list, check for validity, and return the correct data mask if valid, return -1 is invalid */
		int  checkRequiredData(const char *) const;

		bool trim( char * psz ) const;


	private:
		bool m_bInitialized;
		
		unsigned int m_cIP;
		CIP *  m_IP[ g_cMaxIPs ];
		char * m_pszIP[ g_cMaxIPs ];
		
		CSimpleXMLParser m_Parser;
		CInterpreter m_Interpreter;

};

#endif
