//--file: CPremiumArea.h---------------------------------------------
//
// The declaration for a Premium Area for authentication.
//
// An XML description of a Premium Area looks like so:
//
//   <Premium-Area>
//      <Name>Articles</Name>
//      <Request-Urls>/premium1a/,/premium1b/,/premium1c/</Request-Urls>
//      <Tokens-Required>1 AND 2, 3 AND 4</Tokens-Required>
//      <Login-Page-Url>/login/index.html</Login-Page-Url>
//      <Upsell-Page-Url>/upsell/index.html</Upsell-Page-Url>
//      <Access-Denied-Page-Url>/accessDenied1.html</Access-Denied-Page-Url>
//   </Premium-Area>
//
//
// Premium areas store their URLs in 2 forms: 
//     1)  Absolute
//     2)  Relative
//
// When a CIP parent of a Premium Area asks for one of:
//      o Login Url
//      o Upsell Page Url
//      o AccessDenied Page Url
//
// this class is to return the Absolute form of the Url.
//

#ifndef _CPremiumArea_h_
#define _CPremiumArea_h_


#include "./../utils/platform_defines.h"


const int g_cMaxAreaUrls = 32;


class CPremiumArea
{
	friend class CIP;

	public:
		
		/** See if the given Url matches a Url in the Premium Area */
		bool matchesPremiumUrl( const char * pszUrl, unsigned int, unsigned int * ) const;


	public:
		
		/** Accessors */
		const char * getPremiumUrl( unsigned int ) const;
		inline unsigned int getPremiumUrlCount() const { return m_cUrls; }

		inline bool hasName() const { return (0!=m_pszName); }
		inline bool hasTokenExpr() const { return (0!=m_pszTokenExpr); }
		inline bool hasOriginalTokenExpr() const { return (0!=m_pszOriginalTokenExpr); }
		inline bool hasLoginPageUrl() const { return (0!=m_pszLoginPageUrl); }
		inline bool hasUpsellPageUrl() const { return (0!=m_pszUpsellPageUrl); }
		inline bool hasAccessDeniedPageUrl() const { return (0!=m_pszAccessDeniedPageUrl); }
		inline bool hasSignupPageUrl() const { return (0!=m_pszSignupPageUrl); }

		inline const char * getName() const { return ( (0==m_pszName) ? ("(null)") : (m_pszName) ); }
		inline const char * getTokenExpr() const { return ( (0==m_pszTokenExpr) ? ("(null)") : (m_pszTokenExpr) ); }
		inline const char * getOriginalTokenExpr() const { return ( (0==m_pszOriginalTokenExpr) ? ("(null)") : (m_pszOriginalTokenExpr) ); }

		const char * getLoginPageUrl( unsigned int * pcSize = 0 ) const;
		const char * getUpsellPageUrl( unsigned int * pcSize = 0 ) const;
		const char * getAccessDeniedPageUrl( unsigned int * pcSize = 0 ) const;
		const char * getSignupPageUrl( unsigned int * pcSize = 0 ) const;

		inline bool hasActiveLoginRequired() const { return m_bHasActiveLoginRequired; }
		inline bool getActiveLoginRequired() const { return m_bActiveLoginRequired; }

		inline bool hasCachingTag() const { return m_bHasCachingTag; }
		inline bool getCaching() const    { return m_bCaching; }

		inline bool hasAbsoluteLoginPageUrl() const { return (0!=m_pszAbsoluteLoginPageUrl); }
		inline bool hasAbsoluteUpsellPageUrl() const { return (0!=m_pszAbsoluteUpsellPageUrl); }
		inline bool hasAbsoluteAccessDeniedPageUrl() const { return (0!=m_pszAbsoluteAccessDeniedPageUrl); }
		inline bool hasAbsoluteSignupPageUrl() const { return (0!=m_pszAbsoluteSignupPageUrl); }

		inline const char * getAbsoluteLoginPageUrl() const { return m_pszAbsoluteLoginPageUrl; }
		inline const char * getAbsoluteUpsellPageUrl() const { return m_pszAbsoluteUpsellPageUrl; }
		inline const char * getAbsoluteAccessDeniedPageUrl() const { return m_pszAbsoluteAccessDeniedPageUrl; }
		inline const char * getAbsoluteSignupPageUrl() const { return m_pszAbsoluteSignupPageUrl; }

		inline bool hasRequiredData() const { return m_bHasRequiredData; }
		inline const char * getRequiredDataList() const { return m_pszRequiredDataList; }
		inline int  getRequiredDataMask() const    { return m_cRequiredDataMask; }

	private:
		CPremiumArea( const char *, const char *, const char *, const char *, const char *, const char *, const char *, const char *, const char *, const char *,
			          const char *, int );
		~CPremiumArea();

		char * m_pszName;
		char * m_pszTokenExpr;
		char * m_pszOriginalTokenExpr;
		char * m_pszLoginPageUrl;
		char * m_pszUpsellPageUrl;
		char * m_pszAccessDeniedPageUrl;
		char * m_pszSignupPageUrl;

		unsigned int m_cLoginPageUrlSize;
		unsigned int m_cUpsellPageUrlSize;
		unsigned int m_cAccessDeniedPageUrlSize;
		unsigned int m_cSignupPageUrlSize;

		char * m_pszUrl;
		char * m_pszUrls[ g_cMaxAreaUrls ];
		unsigned int m_cLenUrls[ g_cMaxAreaUrls ];
		unsigned int m_cUrls;

		bool m_bHasActiveLoginRequired;
		bool m_bActiveLoginRequired;

		bool m_bHasCachingTag;
		bool m_bCaching;

		char * m_pszAbsoluteLoginPageUrl;
		char * m_pszAbsoluteUpsellPageUrl;
		char * m_pszAbsoluteAccessDeniedPageUrl;
		char * m_pszAbsoluteSignupPageUrl;

		bool   m_bHasRequiredData;
		char * m_pszRequiredDataList;
		int    m_cRequiredDataMask;
};

#endif
