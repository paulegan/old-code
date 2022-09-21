//--file: CPremiumArea.h---------------------------------------------
//
#include "CPremiumArea.h"

#include "./../utils/CLog.h"
#include "./../utils/QuickRegEx.h"

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>

/** *****************************************************************
 *
 * CPremiumArea::getLoginPage
 *
 * Accessor for the login page -- plus an option to retrieve 
 * it's size.
 *
 **/
const char * CPremiumArea::getLoginPageUrl( unsigned int * pcSize ) const
{
	try
	{

		if ( 0 != pcSize )
		{
			if ( 0!=m_pszLoginPageUrl )
				(*pcSize) = m_cLoginPageUrlSize;
			else
				(*pcSize) = 0;
		}

		return ( (0==m_pszLoginPageUrl) ? (0) : (m_pszLoginPageUrl) ); 

	}
	catch( ... )
	{
		CLog::log("CPremiumArea::getLoginPageUrl> Exception!");
		return "(null)";
	}
}



/** *****************************************************************
 *
 * CPremiumArea::getUpsellPage
 *
 * Accessor for the upsell page -- plus an option to retrieve 
 * it's size.
 *
 **/
const char * CPremiumArea::getUpsellPageUrl( unsigned int * pcSize ) const
{
	try
	{

		if ( 0 != pcSize )
		{
			if ( 0!=m_pszUpsellPageUrl )
				(*pcSize) = m_cUpsellPageUrlSize;
			else
				(*pcSize) = 0;
		}

		return ( (0==m_pszUpsellPageUrl) ? (0) : (m_pszUpsellPageUrl) ); 

	}
	catch( ... )
	{
		CLog::log("CPremiumArea::getUpsellPageUrl> Exception!");
		return "(null)";
	}
}



/** *****************************************************************
 *
 * CPremiumArea::getAccessDeniedPage
 *
 * Accessor for the access denied page -- plus an option to 
 * retrieve it's size.
 *
 **/
const char * CPremiumArea::getAccessDeniedPageUrl( unsigned int * pcSize ) const
{
	try
	{
		if ( 0 != pcSize )
		{
			if ( 0!=m_pszAccessDeniedPageUrl )
				(*pcSize) = m_cAccessDeniedPageUrlSize;
			else
				(*pcSize) = 0;
		}

		return ( (0==m_pszAccessDeniedPageUrl) ? (0) : (m_pszAccessDeniedPageUrl) ); 

	}
	catch( ... )
	{
		CLog::log("CPremiumArea::getAccessDeniedPageUrl> Exception!");
		return "(null)";
	}
}

/** *****************************************************************
 *
 * CPremiumArea::getSignupPage
 *
 * Accessor for the signup page -- plus an option to retrieve it's size.
 *
 **/
const char * CPremiumArea::getSignupPageUrl( unsigned int * pcSize ) const
{
	try
	{
		if ( 0 != pcSize )
		{
			if ( 0!=m_pszSignupPageUrl )
				(*pcSize) = m_cSignupPageUrlSize;
			else
				(*pcSize) = 0;
		}

		return ( (0==m_pszSignupPageUrl) ? (0) : (m_pszSignupPageUrl) ); 

	}
	catch( ... )
	{
		CLog::log("CPremiumArea::getSignupPageUrl> Exception!");
		return "(null)";
	}
}

/** *****************************************************************
 *
 * CPremiumArea::getPremiumUrl
 *
 * Accessor for a specific Url
 *
 **/
const char * CPremiumArea::getPremiumUrl( unsigned int idx ) const
{
	try
	{
		if ( idx<0 || idx>m_cUrls )
			return "(null)";

		return m_pszUrls[idx];

	}
	catch( ... )
	{
		CLog::log("CPremiumArea::getPremiumUrl> Exception!");
		return "(null)";
	}
}



/** *****************************************************************
 *
 * CPremiumArea::matchesPremiumUrl
 *
 * This function is supposed to determine the "Best" match for 
 * thre given Url with a Url contained within the Premium Area.
 *
 * Determine if the url matches one of the URLs in the premium area.
 *
 * If it is a match,  return the length that matched via 'pcMatch'.
 *
 * Return true if so,  false if not.
 *
 **/
bool CPremiumArea::matchesPremiumUrl( const char * pszUrl, unsigned int cSize, unsigned int * pcMatch ) const
{
	try
	{

		for ( unsigned int i=0; i<m_cUrls; i++ )
		{
			/** New code -- regular expressions */
			if ( true == matchQuickRegEx( pszUrl, m_pszUrls[i] ) )
			{
				(*pcMatch) = m_cLenUrls[i];
				return true;	
			}

		}

		return false;

	}
	catch( ... )
	{
		CLog::log("CPremiumArea::matchesPremiumUrl> Exception!");
		return false;
	}
}



/** *****************************************************************
 *
 * CPremiumArea::CPremiumArea
 *
 * Theis constructor is private.  It is only accessed by class 
 * CIP (friend).  Therefore,  the parameters have been checked 
 * for null values before the constructor was invoked.
 *
 * pszN = Area name
 * pszR = Area Urls
 * pszT = Area Tokens Required (the logical expression)
 * pszO = original area tokens required (the expression)
 * pszL = Area Login Url
 * pszU = Area Upsell Url
 * pszA = Area AccessDenied Url
 * pszS = Area Signup Url
 * pszAL= Active login flag
 * pszC = Caching flag
 * pszDL= Required Data List
 * cDM  = Require Data Mask
 *
 **/
CPremiumArea::CPremiumArea
(
	const char * pszN,
	const char * pszR,
	const char * pszT,
	const char * pszO,
	const char * pszL,
	const char * pszU,
	const char * pszA,
	const char * pszS,
	const char * pszAL,
	const char * pszC,
	const char * pszDL,
	int          cDM
)
{
	m_pszName                = 0;
	m_pszTokenExpr           = 0;
	m_pszOriginalTokenExpr   = 0;
	m_pszLoginPageUrl        = 0;
	m_pszUpsellPageUrl       = 0;
	m_pszAccessDeniedPageUrl = 0;
	m_pszSignupPageUrl       = 0;
	m_pszUrl                 = 0;

	m_cLoginPageUrlSize        = 0;
	m_cUpsellPageUrlSize       = 0;
	m_cAccessDeniedPageUrlSize = 0;
	m_cSignupPageUrlSize       = 0;

	m_pszAbsoluteLoginPageUrl = 0;
	m_pszAbsoluteUpsellPageUrl = 0;
	m_pszAbsoluteAccessDeniedPageUrl = 0;
	m_pszAbsoluteSignupPageUrl = 0;

	m_bHasActiveLoginRequired = false;
	m_bActiveLoginRequired = true;

	m_bHasCachingTag = false;
	m_bCaching = false;

	m_bHasRequiredData = false;
	m_pszRequiredDataList = 0;
	m_cRequiredDataMask = 0;

	for ( unsigned int i=0; i<g_cMaxAreaUrls; i++ )
	{
		m_pszUrls[i] = 0;
	}

	m_cUrls = 0;

	if ( 0==pszN || 0==pszR || 0==pszT || 0==pszL || 0==pszU || 0==pszA || 0==pszS)
		return;

	unsigned int cN=0, cR=0, cT=0, cO=0, cL=0, cU=0, cA=0, cS=0;
	char * pN=0, * pR=0, * pT=0, * pO=0, * pL =0, * pU=0, * pA=0, * pS=0, * pUrl=0;

	cN = strlen(pszN)   + 1;	if ( cN>1 ) { pN = new char[ cN ]; }
	cR = strlen(pszR)   + 1;	if ( cR>1 ) { pR = new char[ cR ]; }
	cT = strlen(pszT)   + 1;	if ( cT>1 ) { pT = new char[ cT ]; }
	cO = strlen(pszO)   + 1;	if ( cO>1 ) { pO = new char[ cO ]; }
	cL = strlen(pszL)   + 1;	if ( cL>1 ) { pL = new char[ cL ]; }
	cU = strlen(pszU)   + 1;	if ( cU>1 ) { pU = new char[ cU ]; }
	cA = strlen(pszA)   + 1;	if ( cA>1 ) { pA = new char[ cA ]; }
	cS = strlen(pszS)   + 1;	if ( cS>1 ) { pS = new char[ cS ]; }


	if ( 0!=pN )
	{
		memset( pN, '\0', cN );
		//sprintf( pN, "%s", pszN );
		strncpy(pN, pszN, cN);
		m_pszName = pN;
	}
	
	if ( 0!=pR )	/** Set the Url memory */
	{
		memset( pR, '\0', cR );
		//sprintf( pR, "%s", pszR );
		strncpy(pR, pszR, cR);
		m_pszUrl = pR;
	}

	if ( 0!=pT )
	{
		memset( pT, '\0', cT );
		//sprintf( pT, "%s", pszT );
		strncpy(pT, pszT, cT);
		m_pszTokenExpr = pT;
	}
	
	if ( 0!=pO )
	{
		memset( pO, '\0', cO );
		//sprintf( pO, "%s", pszO );
		strncpy(pO, pszO, cO);
		m_pszOriginalTokenExpr = pO;
	}

	if ( 0!=pL )
	{
		memset( pL, '\0', cL );
		//sprintf( pL, "%s", pszL );
		strncpy(pL, pszL, cL);
		m_pszLoginPageUrl = pL;
		m_cLoginPageUrlSize = cL-1;
	}


	if ( 0!=pU )
	{
		memset( pU, '\0', cU );
		//sprintf( pU, "%s", pszU );
		strncpy(pU, pszU, cU);
		m_pszUpsellPageUrl = pU;
		m_cUpsellPageUrlSize = cU-1;
	}

	if ( 0!=pA )
	{
		memset( pA, '\0', cA );
		//sprintf( pA, "%s", pszA );
		strncpy(pA, pszA, cA);
		m_pszAccessDeniedPageUrl = pA;
		m_cAccessDeniedPageUrlSize = cA-1;
	}

	if ( 0!=pS )
	{
		memset( pS, '\0', cS );
		//sprintf( pS, "%s", pszS );
		strncpy(pS, pszS, cS);
		m_pszSignupPageUrl = pS;
		m_cSignupPageUrlSize = cS-1;
	}

	if ( (pszAL != 0) && (pszAL[0] != '\0') )
	{
		m_bHasActiveLoginRequired = true;

		if ( ( toupper(pszAL[0]) == 'F' ) &&
			 ( (pszAL[1] != '\0') && (toupper(pszAL[1]) == 'A') ) &&
			 ( (pszAL[2] != '\0') && (toupper(pszAL[2]) == 'L') ) &&
			 ( (pszAL[3] != '\0') && (toupper(pszAL[3]) == 'S') ) &&
			 ( (pszAL[4] != '\0') && (toupper(pszAL[4]) == 'E') ) )
		{
			m_bActiveLoginRequired = false;
		}
	}

	// Caching flag

	if ( (pszC != 0) && (pszC[0] != '\0') )
	{
		m_bHasCachingTag = true;

		if ( ( toupper(pszC[0]) == 'T' ) &&
			 ( (pszC[1] != '\0') && (toupper(pszC[1]) == 'R') ) &&
			 ( (pszC[2] != '\0') && (toupper(pszC[2]) == 'U') ) &&
			 ( (pszC[3] != '\0') && (toupper(pszC[3]) == 'E') ) )
		{
			m_bCaching = true;
		}
	}

	// Required data list

	if ( (pszDL != 0) && (pszDL[0] != '\0') && (cDM >= 0) )
	{
		m_bHasRequiredData = true;

		int cDL = strlen(pszDL) + 1;
		m_pszRequiredDataList = new char[cDL];
		memset( m_pszRequiredDataList, '\0', cDL );
		strncpy(m_pszRequiredDataList, pszDL, cDL);

		m_cRequiredDataMask = cDM;
	}

	/** Set the URLs by tokenizing the incoming URL argument */
	if ( 0 != m_pszUrl )
	{
		/** First URL is first in line */
		m_pszUrls[m_cUrls++] = m_pszUrl;

		/** Find the rest of the URLs */
		for ( unsigned int i=1; i<cR && m_pszUrl[i]!='\0'; i++ )
		{
			if ( m_pszUrl[i] == ',' )
			{
				m_pszUrl[i] = '\0';

				if ( ('\0'!=m_pszUrl[i+1]) && ((m_cUrls+1)<=g_cMaxAreaUrls) )
				{
					m_pszUrls[m_cUrls++] = &m_pszUrl[i+1];
				}
			}
		}
	}



	/** Find all of the Url lengths only once... */ 
	for ( unsigned int k=0; k<m_cUrls; k++ )
	{
		 m_cLenUrls[k] = strlen( m_pszUrls[k] );
	}
	
	
	return;
}




/** *****************************************************************
 *
 * CPremiumArea::~CPremiumArea
 *
 **/
CPremiumArea::~CPremiumArea()
{
	if ( 0 != m_pszName                ) { delete [] m_pszName;                }
	if ( 0 != m_pszTokenExpr           ) { delete [] m_pszTokenExpr;           }
	if ( 0 != m_pszLoginPageUrl        ) { delete [] m_pszLoginPageUrl;        }
	if ( 0 != m_pszUpsellPageUrl       ) { delete [] m_pszUpsellPageUrl;       }
	if ( 0 != m_pszAccessDeniedPageUrl ) { delete [] m_pszAccessDeniedPageUrl; }
	if ( 0 != m_pszUrl                 ) { delete [] m_pszUrl;                 }
	
	if ( 0 != m_pszAbsoluteLoginPageUrl ) { delete [] m_pszAbsoluteLoginPageUrl; }
	if ( 0 != m_pszAbsoluteUpsellPageUrl ) { delete [] m_pszAbsoluteUpsellPageUrl; }
	if ( 0 != m_pszAbsoluteAccessDeniedPageUrl ) { delete [] m_pszAbsoluteAccessDeniedPageUrl; }

	if ( 0 != m_pszRequiredDataList ) { delete [] m_pszRequiredDataList; }
	/**
	  * Don't free individual Urls -- these are just indexes 
	  * into the m_pszUrl memory...they weren't used to 
	  * allocate the memory. 
	  *
	  **/
	return;
}

