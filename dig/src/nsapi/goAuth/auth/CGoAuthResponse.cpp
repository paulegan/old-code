//--file: CGoAuthResponse.cpp----------------------------------------
//
#include "CGoAuthResponse.h"
#include <string.h>


CGoAuthResponse::CGoAuthResponse()
{
	m_cStatus = GO_AUTH_REQUEST_CONTINUE;
	m_cUserStatus = GO_USER_STATUS_UNKNOWN;

	m_cHeaders = 0;
	m_pszMessage = 0;

	for ( int i=0; i<g_cMaxResponseHeaders; i++ )
	{
		m_pszHeaders[i] = 0;
	}
	
	return;
}



CGoAuthResponse::~CGoAuthResponse()
{
	this->reset();
	return;
}




bool CGoAuthResponse::reset()
{
	m_cStatus = GO_AUTH_REQUEST_CONTINUE;

	for ( unsigned int i=0; i<m_cHeaders; i++ )
	{
		if ( 0 != m_pszHeaders[i] )
		{
			delete [] m_pszHeaders[i];
		}

		m_pszHeaders[i] = 0;
	}
	
	m_cHeaders = 0;

	if ( 0 != m_pszMessage )
	{
		delete [] m_pszMessage;
	}

	m_pszMessage = 0;

	return true;
}





bool CGoAuthResponse::addResponseHeader( const char * psz )
{
	if ( (m_cHeaders+1) >= g_cMaxResponseHeaders ) 
		return false;

	char * p = 0;
	unsigned int cLen = strlen( psz ) + 1;

	p = new char[ cLen ];
	if ( 0==p ) return false;

	memset( p, '\0', cLen );
	strcat( p, psz );

	m_pszHeaders[ m_cHeaders++] = p;
	return true;
}




bool CGoAuthResponse::addMessageBody( const char * psz )
{
	/** This is bad... */
	if ( 0 != m_pszMessage )
	{
		delete [] m_pszMessage;
		m_pszMessage = 0;
	}

	char * p = 0;
	unsigned int cLen = strlen( psz ) + 1;

	p = new char[ cLen ];
	if ( 0==p ) return false;

	memset( p, '\0', cLen );
	strcat( p, psz );

	m_pszMessage = p;
	return true;
}



const char * CGoAuthResponse::getHeader( unsigned int idx ) const
{
	return ( m_pszHeaders[idx] );
}



const char * CGoAuthResponse::getMessageBody() const
{
	if ( 0 != m_pszMessage )
	{
		return (m_pszMessage);
	}
	else
	{
		return 0;
	}
}



char * CGoAuthResponse::moveHeader( unsigned int idx )
{
	char * pszRet = 0;
	unsigned int i = 0;

	if ( idx<(m_cHeaders-1) )
	{
		return pszRet;
	}

	// Get a copy of our internal memory...
	pszRet = m_pszHeaders[idx];
	m_pszHeaders[idx] = 0;


	// Shift all of the pointers down one...
	for ( i=idx; i<m_cHeaders; i++ )
	{
		m_pszHeaders[ i ] = m_pszHeaders[i+1];
	}

	// Make sure last pointer is NULL
	m_pszHeaders[i] = 0;
		
	m_cHeaders--;
	
	return pszRet;

}




char * CGoAuthResponse::moveMessageBody()
{
	char * pszRet = 0;
	
	pszRet = m_pszMessage;

	m_pszMessage = 0;

	return pszRet;
}
