//--file: CIP.h------------------------------------------------------
//
// CIPs areas store their URLs in 2 forms: 
//     1)  Absolute
//     2)  Relative
//
// When one of:
//      o Login Url
//      o Upsell Page Url
//      o AccessDenied Page Url
//      o  Lame Browser Url
//
// is required. this class is to return the Absolute form of the Url.
//
#ifndef _CIP_h_
#define _CIP_h_

#include "./../utils/platform_defines.h"
#include "./../utils/UrlEncode.h"

#define MAX_URL_LENGTH 1024

const int g_cMaxPremiumAreas = 64;
const int g_cMaxFreeAreas = 32;

class CPremiumArea;

class CIP
{
	friend class CGoAuthIPCollection;

	public:

		CIP();
		~CIP();

		const char * getFreeArea( unsigned int idx ) const { return ( (idx>=0&&idx<=m_cFreeAreas) ? (m_pszFreeAreas[idx]) : ("(null)") ); }

		bool formatRedirectUrl( char *, unsigned int, const char *, const char *, const char *, const char * ) const;

		/** Accessors */
		inline unsigned int getPremiumAreaCount() const { return m_cAreas; }
		inline unsigned int getFreeAreaCount() const { return m_cFreeAreas; }

		inline const char * getHostName()         const { return ( m_pszHostName   ); }
		inline const char * getServiceName()      const { return ( m_pszServiceName); }
		inline const char * getDomainName()       const { return ( m_pszDomainName ); }
		inline const char * getPort()             const { return ( m_pszPort       ); }

		inline const char * getDefaultLoginPageUrl()        const { return (m_pszDefaultLoginPageUrl       ); }
		inline const char * getDefaultUpsellPageUrl()       const { return (m_pszDefaultUpsellPageUrl      ); }
		inline const char * getDefaultAccessDeniedPageUrl() const { return (m_pszDefaultAccessDeniedPageUrl); }
		inline const char * getDefaultSignupPageUrl()       const { return (m_pszDefaultSignupPageUrl      ); }
		inline const char * getLameBrowserUrl()             const { return (m_pszLameBrowserUrl            ); }
		
		inline const char * getDefaultAbsoluteLoginPageUrl()        const { return (m_pszDefaultAbsoluteLoginPageUrl); }
		inline const char * getDefaultAbsoluteUpsellPageUrl()       const { return (m_pszDefaultAbsoluteUpsellPageUrl); }
		inline const char * getDefaultAbsoluteAccessDeniedPageUrl() const { return (m_pszDefaultAbsoluteAccessDeniedPageUrl); }
		inline const char * getDefaultAbsoluteSignupPageUrl()       const { return (m_pszDefaultAbsoluteSignupPageUrl); }
		inline const char * getAbsoluteLameBrowserUrl()             const { return (m_pszAbsoluteLameBrowserUrl     ); }

		inline const char * getFreeAreaUrl( unsigned int idx ) const { return ( (idx>=0 && idx<m_cFreeAreas) ? (m_pszFreeAreas[idx]) : ("(null)") ); }
		inline const CPremiumArea * getPremiumArea( unsigned int idx ) const { return ( (idx>=0 && idx<m_cAreas) ? (m_Areas[idx]) : (0) ); }

		inline bool getDefaultActiveLoginRequired() const { return m_bDefaultActiveLoginRequired; }
        inline bool getDefaultCaching() const { return m_bDefaultCaching; }

		inline const char * getDefaultRequiredDataList() const { return m_pszDefaultRequiredDataList; }
		inline int          getDefaultRequiredDataMask() const { return m_cDefaultRequiredDataMask; }

		inline bool hasPort()                       const { return (0!=m_pszPort);                       }
		inline bool hasHostName()                   const { return (0!=m_pszHostName);                   }
		inline bool hasServiceName()                const { return (0!=m_pszServiceName);                }
		inline bool hasLameBrowserUrl()             const { return (0!=m_pszLameBrowserUrl);             }
		inline bool hasDefaultLoginPageUrl()        const { return (0!=m_pszDefaultLoginPageUrl);        }
		inline bool hasDefaultUpsellPageUrl()       const { return (0!=m_pszDefaultUpsellPageUrl);       }
		inline bool hasDefaultAccessDeniedPageUrl() const { return (0!=m_pszDefaultAccessDeniedPageUrl); }
		inline bool hasDefaultSignupPageUrl()       const { return (0!=m_pszDefaultSignupPageUrl);       }
		
		inline bool hasDefaultAbsoluteLoginPageUrl()        const { return (0!=m_pszDefaultAbsoluteLoginPageUrl);        }
		inline bool hasDefaultAbsoluteUpsellPageUrl()       const { return (0!=m_pszDefaultAbsoluteUpsellPageUrl);       }
		inline bool hasDefaultAbsoluteAccessDeniedPageUrl() const { return (0!=m_pszDefaultAbsoluteAccessDeniedPageUrl); }
		inline bool hasDefaultAbsoluteSignupPageUrl()       const { return (0!=m_pszDefaultAbsoluteSignupPageUrl);       }
		inline bool hasAbsoluteLameBrowserUrl()             const { return (0!=m_pszAbsoluteLameBrowserUrl);             }


	private:
		/** Is the request an Upsell, Login, or AccessDenied page? */
		bool isAuthRedirectPage( const char * ) const;
		
		/** Find the best premium area match for the given Url */
		bool findBestPremiumAreaMatch( const char * pszUrl, unsigned int cLen, CPremiumArea ** ppPa ) const;
		
		/** Name, Host, Lame Browser, Upsell, Access Denied, Signup, Active Login, Caching */
		bool init( const char *, const char *, const char *, const char *, const char *, const char *, const char *, const char *, const char *, const char *,
			       const char *, const char *, int );

		/** Name, Request Urls, Tokens Req, Upsell, Access Denied, Signup, Active Login Required, Caching, Required Data List, Required Data Mask */
		bool addPremiumArea( const char *, const char *, const char *, const char *, const char *, const char *, const char *, const char *, const char *,
			                 const char *, const char *, int );

		/** Free Area Url */
		bool addFreeArea( const char * );
		
		/** Nuke all premium areas, as well as IP info */
		void destroy(); 

		/** Is the given Url a Free Area request? */
		bool isFreeAreaUrl( const char * ) const;

		/** Take all 4 Urls and make them absolute */
		char * reformatUrl( const char *, char * pszBuffer = 0, unsigned int cLen = 0 ) const;
		

	private:
		
		/** A set of premium areas */
		CPremiumArea * m_Areas[g_cMaxPremiumAreas];

		/** A count of the current number of premium areas */
		unsigned int m_cAreas;

		/** A set of premium areas */
		char * m_pszFreeAreas[g_cMaxFreeAreas];

		/** A count of the current number of free areas */
		unsigned int m_cFreeAreas;

		/** The size of each Free Area Url */
		unsigned int m_cFreeAreaSize[g_cMaxFreeAreas];
		
		char * m_pszServiceName;
		char * m_pszLameBrowserUrl;
		char * m_pszDomainName;
		char * m_pszHostName;
		char * m_pszPort;

		bool   m_bDefaultActiveLoginRequired;
		bool   m_bDefaultCaching;

		char * m_pszDefaultRequiredDataList;
		int    m_cDefaultRequiredDataMask;

		char * m_pszDefaultLoginPageUrl;
		char * m_pszDefaultUpsellPageUrl;
		char * m_pszDefaultAccessDeniedPageUrl;
		char * m_pszDefaultSignupPageUrl;

		/** The sizes of these 4 strings */
		unsigned int m_cDefaultLoginPageUrlSize;
		unsigned int m_cDefaultUpsellPageUrlSize;
		unsigned int m_cDefaultAccessDeniedPageUrlSize;
		unsigned int m_cDefaultSignupPageUrlSize;


		char * m_pszDefaultAbsoluteLoginPageUrl;
		char * m_pszDefaultAbsoluteUpsellPageUrl;
		char * m_pszDefaultAbsoluteAccessDeniedPageUrl;
		char * m_pszDefaultAbsoluteSignupPageUrl;
		char * m_pszAbsoluteLameBrowserUrl;

};

#endif
