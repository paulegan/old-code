//--file: CGoAuthResponse.h------------------------------------------
//
// The definition of a class that determines the response header 
// format that accompanies CGoAuth.  
//
#ifndef _CGoAuthResponse_h_
#define _CGoAuthResponse_h_

#include "./../utils/platform_defines.h"

const int g_cMaxResponseHeaders = 16;


#define GO_AUTH_REQUEST_COMPLETE  1
#define GO_AUTH_REQUEST_CONTINUE  2


#define GO_USER_STATUS_UNKNOWN       1
#define GO_USER_FREE_REQUEST         2
#define GO_USER_PREMIUM_REQUEST      4
#define GO_USER_UPSOLD               8
#define GO_USER_GRANT_ACCESS         16
#define GO_USER_ACCESS_DENIED        32
#define GO_USER_MUST_LOGIN           64
#define GO_USER_NOT_PREMIUM_REQUEST  128
#define GO_USER_EXCEPTION            256
#define GO_USER_MUST_SIGNUP          512


class CGoAuthResponse
{
	friend class CGoAuth;
	friend class CGoAuthIPCollection;

	public:
		CGoAuthResponse();
		~CGoAuthResponse();

		bool reset();

		bool addResponseHeader( const char * );
		bool addMessageBody( const char * );

		const char * getHeader( unsigned int ) const;
		const char * getMessageBody() const;

		inline bool hasMessageBody()         const { return ( 0 != m_pszMessage ); }
		inline bool hasHeaderData()          const { return ( m_cHeaders > 0 );    }
		inline unsigned int getHeaderCount() const { return ( m_cHeaders );        }
		inline unsigned int getStatus()      const { return ( m_cStatus );         }
		inline unsigned int getUserStatus()  const { return ( m_cUserStatus );     }

		inline bool wasFreeRequest() const { return ((m_cUserStatus&GO_USER_FREE_REQUEST) != 0 ); }
		inline bool wasPremiumRequest() const { return ((m_cUserStatus&GO_USER_PREMIUM_REQUEST) != 0 ); }
		inline bool result( int status ) const { return ((m_cUserStatus&status) != 0 ); }


		char * moveHeader( unsigned int );
		char * moveMessageBody( );


	private:

		char * m_pszHeaders[ g_cMaxResponseHeaders ];
		
		unsigned int m_cHeaders;
		
		unsigned int m_cStatus;

		unsigned long m_cUserStatus;

		char * m_pszMessage;

};

#endif
