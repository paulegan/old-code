//--file: CGreen.h-----------------------------------------
//
// A C++ class that can be used to retrieve GREEN 
// information from a user's cookie data to make it 
// available for applications to use.
//
// Author: Tom Laramee,  01/20/99
//
//
// Usage:
//
//	// Data buffer for users cookie info
//	char szCookie[4096] = {'\0'};	
//		
//	// Get a copy of the users cookie data
//	sprintf( szCookie, "%s", pszCookieData );
//
//	// A CGreen cookie parser...
//	CGreen g( szCookie );
//	
//	// A buffer to hold data values...
//	char sz[1024] = {'\0'};
//
//
//	
//  // Make sure GREEN was parsable and has all required fields...
//	if ( true == g.isValid() )
//	{
//		// Access GREENs values...	
//		if ( true == g.getUserTokens( sz, 1024 ) )
//			printf ("List of users tokens [%s]\n", sz );
//		else
//			printf ("...could not get list of users tokens [%s]\n", sz );
//	
//		if ( true == g.userHasToken(1) ) 
//			printf ("User has token 1\n" );
//		else
//			printf ("...User does not have token 1\n" );
//
//		if ( true == g.getUserId( s1, 256 ) )
//			printf ("User Id: %s\n", s1 );
//		else
//			printf ("...could not getUserId\n" );
//	}
//	else
//	{
//		printf ("GREEN is not valid\n");
//	}
//
//	
//	if ( true == g.hasWarnings() )
//	{
//		printf ("GREEN has parse warnings...\n");
//		g.getWarningInfo( sz, 1024 );
//		printf ("GREEN warning information:\n%s", sz );
//	}
//		
//	if ( true == g.hasErrors() )
//	{
//		printf ("GREEN has parse errors...\n");
//		g.getErrorInfo( sz, 1024 );
//		printf ("GREEN error information:\n%s", sz );
//	}
//	
#ifndef _CGreen_h_
#define _CGreen_h_


#ifdef __OS_SOLARIS__
	#include "./../utils/platform_defines.h"
#endif


const int g_cMaxGreenNameSize       = 16;
const int g_cMaxGreenValueSize      = 256;
const int g_cMaxNameValuePairSize   = 273;
const int g_cMaxKnownNameValuePairs = 20;


const int g_cMaxKnownErrors   = 9;
const int g_cMaxKnownWarnings = 10;


/** Error codes - bitwise AND'd and OR'd to set/get */
const int g_cErrorMissingOrNullCookieData = 1;
const int g_cErrorInvalidCookieDataFormat = 2;
const int g_cErrorHexUnescapeError        = 4;
const int g_cErrorDecryptionError         = 8;
const int g_cErrorMissingOrNullGreenInfo  = 16;
const int g_cErrorGreenInfoIsIncomplete	  = 32;
const int g_cErrorEncryptionError		  = 64;
const int g_cErrorHexEscapeError		  = 128;
const int g_cCookieBufferOverflowError	  = 256;

/** Warning codes - bitwise AND'd and OR'd to set/get */
const int g_cWarnInvalidNameValuePair     = 1;
const int g_cWarnInvalidNameToken		  = 2;
const int g_cWarnOverflowNameValuePair	  = 4;
const int g_cWarnUnknownNameToken 		  = 8;
const int g_cWarnGreenInfoBufferOverflow  = 16;
const int g_cWarnInvalidTimeToExpire      = 32;
const int g_cWarnInvalidDate			  = 64;
const int g_cWarnInvalidTime			  = 128;
const int g_cWarnInvalidDateTime		  = 256;
const int g_cWarnInvalidTokenValue		  = 512;

/** field mask codes - bitwise AND'd and OR'd.  
Used to check for green completeness */
const int g_cUsernameBit = 1;
const int g_cFirstNameBit = 1 << 1;
const int g_cLastNameBit = 1 << 2;
const int g_cEmailAddressBit = 1 << 3;
const int g_cBirthdayBit = 1 << 4;
const int g_cGeoCodeBit = 1 << 5;
const int g_cDemoMaskBit = 1 << 6;
const int g_cGenderBit = 1 << 7;
    
const int g_cCompleteRegRequiredFieldsMask = g_cUsernameBit | g_cFirstNameBit | g_cLastNameBit | g_cEmailAddressBit | g_cBirthdayBit | g_cGeoCodeBit | g_cDemoMaskBit;


class CGreen
{

	public:
	
		/** Constructs an empty Green, used to SET a GREEN */
		CGreen(); 
		
		/** Used with the cookie string to GET (decode/parse) the GREEN */
		CGreen( char * );

		/** Copy constructor */
		CGreen( const CGreen & ); 

		/** Clean up name/value and overflow name/value pair data */
		~CGreen(); 


		/** True if Green contains all required name/value pairs, false otherwise  */
		bool isValid();
		

		/** Accessor for warning info status */
		inline bool hasWarnings() const { return (0!=m_ulWarnings); }

		/** Accessor for warning info status */
		inline bool hasErrors() const { return (0!=m_ulErrors); }

		/** Accessor for warning information */
		bool getWarningInfo( char *, unsigned int ) const;

		/** Accessor for error information */
		bool getErrorInfo( char *, unsigned int ) const;


		/** Get NAME/VALUE pairs parsed out of the cookie data - for debugging only */
		bool getRawInformation( char *, unsigned int ) const;


	
		/** See if a user has a particular token */
		bool userHasToken( unsigned int ) const;

		/** Get the users tokens in a comma delimited list */
		bool getUserTokens( char *, unsigned int ) const;




		/** Does the GREEN contain certain information? */
		inline bool hasUserId()           const { return ( 0 != m_pszValues[0] );  }
		inline bool hasLogId()            const { return ( 0 != m_pszValues[1] );  }
		inline bool hasSession()          const { return ( 0 != m_pszValues[2] );  }
		inline bool hasTokenMask()        const { return ( 0 != m_pszValues[3] );  }
		inline bool hasUsername()         const { return ( 0 != m_pszValues[4] );  }
		inline bool hasEmail()            const { return ( 0 != m_pszValues[5] );  }
		inline bool hasSWID()             const { return ( 0 != m_pszValues[6] );  }
		inline bool hasDateAssigned()     const { return ( 0 != m_pszValues[7] );  }
		inline bool hasTimeToExpire()     const { return ( 0 != m_pszValues[8] );  }
		inline bool hasDateLastLogin()    const { return ( 0 != m_pszValues[9] );  }
		inline bool hasDateLastModified() const { return ( 0 != m_pszValues[10] ); }
		inline bool hasFirstname()        const { return ( 0 != m_pszValues[11] ); }
		inline bool hasLastname()         const { return ( 0 != m_pszValues[12] ); }
		inline bool hasDemoMask()         const { return ( 0 != m_pszValues[13] ); }
		inline bool hasOldUsername()      const { return ( 0 != m_pszValues[14] ); }
		inline bool hasOldAffiliate()     const { return ( 0 != m_pszValues[15] ); }
		inline bool hasBirthday()         const { return ( 0 != m_pszValues[16] ); }
		inline bool hasGeoCode()          const { return ( 0 != m_pszValues[17] ); }
		inline bool hasGender()           const { return ( 0 != m_pszValues[18] ); }


		/** Accessors for specific name/value pairs */                          
        bool hasFieldMask( unsigned int ) const;
        bool isUserComplete() const;
		bool getUserId( char *, unsigned int ) const;
		bool getLogId( char *, unsigned int ) const;
		bool getSession( char *, unsigned int ) const;
		bool getTokenMask( char *, unsigned int ) const;
		bool getUsername( char *, unsigned int ) const;
		bool getEmail( char *, unsigned int ) const;
		bool getSWID( char *, unsigned int ) const;
		bool getDateAssigned( char *, unsigned int ) const;
		bool getTimeToExpire( char *, unsigned int ) const;
		bool getDateLastLogin( char *, unsigned int ) const;
		bool getDateLastModified( char *, unsigned int ) const;
		bool getFirstname( char *, unsigned int ) const;
		bool getLastname( char *, unsigned int ) const;
		bool getDemoMask( char *, unsigned int ) const;
		bool getOldUsername( char *, unsigned int ) const;
		bool getOldAffiliate( char *, unsigned int ) const;
		bool getBirthday( char *, unsigned int ) const;
		bool getGeoCode( char *, unsigned int ) const;
		bool getGender( char *, unsigned int ) const;

	
	protected:
		
		/** Initialize data members for constructors */
		void init();

		/** Parse raw information out of the cookie data */
		bool parseCookieData( char * );
		
		/** Parse unescaped/decrypted GREEN information */
		bool parseGreenData( char * );

		/** Parse out a NAME/VALUE or complex NAME/VALUE pair */
		bool parseNameValuePair( const char *, unsigned int, unsigned long * );
		
		/** Add a simple name value pair to the GREENs parsed info */
		bool addNameValuePair( char *, unsigned int, char *, unsigned int, int idx = -1 );

		/** Private accessor for name/value pair data */
		bool getValue( unsigned int, char *, unsigned int ) const;

		/** Private method for setting int or character data member values */
		bool setValue( unsigned int idx, const char * psz = 0, unsigned long l = 0 );

		/** Private method for setting any data member values */
		bool setNameValuePair( unsigned int, const char * );


	protected:
	
		/** The values of the current NAME/VALUE pairs */
		char * m_pszValues[ g_cMaxKnownNameValuePairs ];

		/** The size of these values.. */
		unsigned int m_cValueSize[ g_cMaxKnownNameValuePairs ];
			
		/** The cryption key index */
		long m_cCryptKeyIndex;

	
		/** The 2 letter names of the known Commerce Token names */
		char m_szNames[g_cMaxKnownNameValuePairs][3];
			
		/** Powers of 2 for Commerce token look-up */
		unsigned long m_ulMask[g_cMaxKnownNameValuePairs];
			

		/** Storage for parsing warnings (non-fatal errors)? */
		unsigned long m_ulWarnings;
			
		/** Storage for fatal errors? */
		unsigned long m_ulErrors;
			

	public:

		/** Do you really wanna know? */

		inline bool errorInvalidCookieData()       const { return ( (m_ulErrors&g_cErrorMissingOrNullCookieData)  == g_cErrorMissingOrNullCookieData ); }
		inline bool errorInvalidCookieDataFormat() const { return ( (m_ulErrors&g_cErrorInvalidCookieDataFormat)  == g_cErrorInvalidCookieDataFormat ); }
		inline bool errorHexUnescapeError()        const { return ( (m_ulErrors&g_cErrorHexUnescapeError)         == g_cErrorHexUnescapeError        ); }
		inline bool errorDecryptionError()         const { return ( (m_ulErrors&g_cErrorDecryptionError)          == g_cErrorDecryptionError         ); }
		inline bool errorInvalidCookieInfo()       const { return ( (m_ulErrors&g_cErrorMissingOrNullGreenInfo)   == g_cErrorMissingOrNullGreenInfo  ); }
		inline bool errorCookieInfoIncomplete()    const { return ( (m_ulErrors&g_cErrorGreenInfoIsIncomplete)    == g_cErrorGreenInfoIsIncomplete   ); }
		inline bool errorEncryptionError()         const { return ( (m_ulErrors&g_cErrorEncryptionError)          == g_cErrorEncryptionError         ); }
		inline bool errorHexEscapeError()          const { return ( (m_ulErrors&g_cErrorHexEscapeError)           == g_cErrorHexEscapeError          ); }
		inline bool errorBufferOverflow()          const { return ( (m_ulErrors&g_cCookieBufferOverflowError)     == g_cCookieBufferOverflowError    ); }

		inline bool warnInvalidNameValuePair()     const { return ( (m_ulWarnings&g_cWarnInvalidNameValuePair)    == g_cWarnInvalidNameValuePair     ); }
		inline bool warnInvalidNameToken()         const { return ( (m_ulWarnings&g_cWarnInvalidNameToken)        == g_cWarnInvalidNameToken         ); }
		inline bool warnOverflowNameValuePair()    const { return ( (m_ulWarnings&g_cWarnOverflowNameValuePair)   == g_cWarnOverflowNameValuePair    ); }
		inline bool warnUnknownNameToken()         const { return ( (m_ulWarnings&g_cWarnUnknownNameToken)        == g_cWarnUnknownNameToken         ); }
		inline bool warnGreenInfoBufferOverflow()  const { return ( (m_ulWarnings&g_cWarnGreenInfoBufferOverflow) == g_cWarnGreenInfoBufferOverflow  ); }
		inline bool warnInvalidDate()              const { return ( (m_ulWarnings&g_cWarnInvalidDate)             == g_cWarnInvalidDate              ); }
		inline bool warnInvalidTine()              const { return ( (m_ulWarnings&g_cWarnInvalidTime)             == g_cWarnInvalidTime              ); }
		inline bool warnInvalidDateTime()          const { return ( (m_ulWarnings&g_cWarnInvalidDateTime)         == g_cWarnInvalidDateTime          ); }
		inline bool warnInvalidTokenValue()        const { return ( (m_ulWarnings&g_cWarnInvalidTokenValue)       == g_cWarnInvalidTokenValue        ); }


};


#endif
