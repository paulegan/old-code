//--file: CXMLNode.cpp-----------------------------------------------
//
#include "CSimpleXMLParser.h"
#include "CXMLNode.h"
#include <memory.h>
#include <string.h>
#include <stdio.h>

#ifdef __OS_WINNT__
#include <ostream.h>
#endif




/** *****************************************************************
 *
 * CXMLNode::getChild
 *
 * Accessor for a child when multiple children exist. The index 
 * of the desired child is given,  along with a pointer to 
 * return the child if it is found.
 *
 * Returns true if the desired child was found,  false if not.
 *
 **/
bool CXMLNode::getChild( const char * pszName, unsigned int idx, CXMLNode ** pRet ) const
{
	unsigned int i;

	if ( 0 == pRet ) return false;

	for ( i=0; i<m_cChildCount; i++ )
	{
		if ( ( 0 == strcmp( m_Children[i]->getName(), pszName ) ) && 
			 ( m_Children[i]->m_cIndex == idx ) )
		{
			(*pRet) = m_Children[i];
			return true;
		}
	}

	return false;
}



/** *****************************************************************
 *
 * CXMLNode::hasParent
 *
 * Obtain a reference to the node's parent.
 *
 * Returns: trus if the parent exists,  false if not. 
 *
 **/
bool CXMLNode::hasParent( CXMLNode ** pRet ) const
{
	if (0==pRet) return false;

	if ( 0 != m_Parent )
	{
		*pRet = m_Parent;
	}
	
	return false;
}


/** *****************************************************************
 *
 * CXMLNode::childExists
 *
 * This function will recurse the hierarchy looking for a 
 * node of the given name.  It will return the first 
 * occurance found.
 * 
 * NOTE: This isn't gonna help you if there are multiple 
 * instances of the same child in the tree and you're trying 
 * to somewhat randomly find them.
 *
 **/
bool CXMLNode::childExists( const char * pszName, unsigned int * pcCount ) const
{
	unsigned int i;
	if ((0==pszName) || (0==pcCount)) return false;

	(*pcCount) = 0;

	if ( 0 == strcmp( pszName, this->getName() ) )
	{
		(*pcCount) = this->m_cIndex;
		return true;
	}

	for ( i=0; i<m_cChildCount; i++ )
	{
		if ( true == m_Children[i]->childExists( pszName, pcCount ) )
		{
			(*pcCount) = m_Children[i]->m_cCount;
			return true;
		}
	}

	return false;
}




/** *****************************************************************
 *
 * CXMLNode::hasChild 
 *
 * See if a particular node has a child of the given name.
 * If so,  return a count of the number of chldren of that name. 
 *
 * Returns true if the child was found,  false if not.
 *
 **/
bool CXMLNode::hasChild( const char * psz, CXMLNode ** pRet ) const
{
	unsigned int i;

	if ( (0==psz) || (0==pRet) ) return false;

	for ( i=0; i<m_cChildCount; i++ )
	{
		if ( 0 ==strcmp( psz, m_Children[i]->getName() ) )
		{
			*pRet = m_Children[i];
			return true;
		}
	}

	return false;
}





/** *****************************************************************
 *
 * CXMLNode::hasProperty 
 *
 * An inefficient method to determine how many of a particular 
 * child exist.
 *
 * Returns true is name/value was found,  false if not.
 *
 **/
bool CXMLNode::hasProperty( const char * pszName, unsigned int * pcCount ) const
{
	unsigned int i;
	bool bFound = false;

	if ( (0==pszName) || (0==pcCount) ) return false;

	(*pcCount) = 0;

	for ( i=0; i<m_cDataCount; i++ )
	{
		if ( 0 == strcmp( m_pszNameData[i],  pszName ) )
		{
			bFound = true;
			(*pcCount)++;
		}
	}

	return bFound;
}




/** *****************************************************************
 *
 * CXMLNode::hasProperty 
 *
 * Another inefficient method to find a particular name/value pair 
 * if there are multiple instances of the pair (as found using 
 * the above "hasProperty()" method.
 *
 * Returns true is name/value was found,  false if not.
 *
 **/
bool CXMLNode::hasProperty( const char * pszName, unsigned int idx, char * pszRet, unsigned int cSize ) const
{
	unsigned int cCount=0;

	if ( (0==pszName) || (0==pszRet) ) return false;

	memset( pszRet, '\0', cSize);

	for ( unsigned int i=0; i<m_cDataCount; i++ )
	{
		if ( 0 == strcmp( m_pszNameData[i],  pszName ) )
		{
			/** Only copy it if it's the correct index */
			if ( (i-cCount)==idx )
			{
				if ( strlen(m_pszValData[i]) >= cSize )
				{ memcpy( pszRet, m_pszValData[i], cSize-1 ); }
				else
				{ sprintf( pszRet, "%s", m_pszValData[i] ); }

				return true;
			}
		}
		else
		{
			cCount++;
		}
	}

	return false;	
}




/** *****************************************************************
 *
 * CXMLNode::hasProperty 
 *
 * Query a node to see if it has a particular property name.
 * If so,  put the corresponding value into pszRet.
 *
 * Returns true is name/value was found,  false if not.
 *
 **/
bool CXMLNode::hasProperty( const char * pszName, char * pszRet, unsigned int cSize ) const
{
	unsigned int i;

	if ( (0==pszName) || (0==pszRet) ) return false;

	memset( pszRet, '\0', cSize);

	for ( i=0; i<m_cDataCount; i++ )
	{
		if ( 0 == strcmp( m_pszNameData[i],  pszName ) )
		{
			if ( strlen(m_pszValData[i]) >= cSize )
			{
				memcpy( pszRet, m_pszValData[i], cSize-1 );
			}
			else
			{
				sprintf( pszRet, "%s", m_pszValData[i] );
			}

			return true;
		}
	}

	return false;	
}



/** *****************************************************************
 *
 * operator << 
 *
 * Display the hierarchy of nodes in ASCII
 *
 **/
#ifdef __OS_WINNT__
ostream&  operator << ( ostream & os, const CXMLNode * pn )
{
	unsigned int i;
	unsigned int j;
	char szTab[16] = {'\0'};

	if ( 0 == pn ) return os;


	for ( j=0; (j<pn->m_cDepth) && (j<16); j++ )
		{ szTab[j] = '\t'; }


	os << szTab << "Node: [" << pn->m_pszName << "]" << endl;
	os << szTab << "Index/count: " << pn->m_cIndex << "/" << pn->m_cCount << endl;
	os << szTab << "Data Pairs: " << pn->m_cDataCount << endl;

	for ( i=0; i<pn->m_cDataCount; i++ )
	{ 
		os << szTab << pn->m_pszNameData[i] << "=" << pn->m_pszValData[i] << endl; 
	}

	/** If there are any children let's see them...*/
	if ( pn->m_cChildCount > 0 )
	{
		os << szTab << "Children: " << pn->m_cChildCount << " [";

		for ( i=0; i<pn->m_cChildCount; i++ )
		{ 
			os << pn->m_Children[i]->m_pszName; 
			os << "|";
			os << pn->m_Children[i]->m_cCount;
			os << "|";
			os << pn->m_Children[i]->m_cIndex;

			if ( i<(pn->m_cChildCount-1) )
				os << ", ";
		}

		os << "]" << endl << endl;
	}
	else
	{
		os << szTab << "Children: 0" << endl << endl;
	}

	/** Recurse on children */
	for ( i=0; i<pn->m_cChildCount; i++ )
	{ 
		os << pn->m_Children[i]; 
	}


	return os;
}
#endif




/** *****************************************************************
 *
 * CXMLNode::addChild
 *
 **/
bool CXMLNode::addChild( PCXMLNode p )
{
	unsigned int i;
	unsigned int count = 1;
	if ( 0==p ) return false;
		
	
	/** Overflow */
	if ( (m_cChildCount+1) >= g_cMaxNodeChildren )
		return false;


	/** See if it's a child name collision */
	for ( i=0; i<m_cChildCount; i++ )
	{
		if ( 0 == strcmp( m_Children[i]->getName(), p->getName() ) )
		{
			m_Children[i]->m_cIndex++;
			count++;
		}		
	}
	

	/** Add the child */
	m_Children[ m_cChildCount++ ] = p;


	for ( i=0; i<m_cChildCount; i++ )
	{
		if ( 0 == strcmp( m_Children[i]->getName(), p->getName() ) )
		{
			m_Children[i]->m_cCount = count;
		}		
	}


	return true;
}



/** *****************************************************************
 *
 * CXMLNode::addProperty
 *
 * Add a property to the list of name/value pairs.
 *
 * Begins with '<NAME>', ends with </NAME>,  has a value in the middle. 
 *
 * eg:  <FirstName>Fred</FirstName>
 *
 **/
bool CXMLNode::addProperty( const char * psz )
{
	static const char * pszDelim = "<>";
	int cNameLen = 0, cValLen = 0, cLen = 0;
	char * psz1  = 0;
	char * psz2  = 0;
	char * psz3  = 0;
	char * p     = 0;
	char * pszN  = 0;
	char * pszV  = 0;

	
	/** Bad data or no room */
	if ( 0==psz ) return false;
	if ( (m_cDataCount+1) >= g_cMaxDataPairs ) return false;


	/** Copy the input data */
	cLen = strlen( psz ) + 1;
	p = new char[ cLen ];
	if ( 0 == p ) return false;

	memset(  p, '\0', cLen );
	sprintf( p, "%s", psz );

	
	/** Retrieve the 3 tokens */
	psz1 = strtok( p, pszDelim );	
	psz2 = strtok( 0, pszDelim );	
	psz3 = strtok( 0, pszDelim );	

	if ( 0==psz1 || 0==psz2 || 0==psz3 ) 
		{ delete [] p; return false; }

	if ( strlen(psz3) < 2 ) 
		{ delete [] p; return false; }

	if ( 0 != strcmp( psz1, psz3+1 ) ) 
		{ delete [] p; return false; }


	cNameLen = strlen( psz1 ) + 1;
	cValLen = strlen( psz2 ) + 1;

	pszN = new char[ cNameLen ];
	pszV = new char[ cValLen ];
 
	if ( 0!=pszN && 0!=pszV )
	{
		memset( pszN, '\0', cNameLen );
		memset( pszV, '\0', cValLen );

		sprintf( pszN, "%s", psz1  );
		sprintf( pszV, "%s", psz2 );

		m_pszNameData[m_cDataCount ] = pszN;
		m_pszValData[m_cDataCount++ ] = pszV;

		delete [] p;
		return true;
	}
	else
	{
		if ( 0!=pszN ) delete [] pszN;
		if ( 0!=pszV ) delete [] pszV;

		delete [] p;
		return false;
	}

}




/** *****************************************************************
 *
 * CXMLNode::CXMLNode
 *
 **/
CXMLNode::CXMLNode( const char * pszName, unsigned int cLevel )
{
	int i;

	m_cChildCount = 0;
	m_cDataCount = 0;
	m_Parent = 0;

	m_cCount = 1;
	m_cIndex = 0;

	m_pszName = 0;

	m_cDepth = cLevel;

	this->setName(pszName);

	for ( i=0; i<g_cMaxDataPairs; i++ )
	{
		m_pszNameData[i] = 0;
		m_pszValData[i] = 0;
	}

	for ( i=0; i<g_cMaxNodeChildren; i++ )
	{
		m_Children[i] = 0;
	}

	return;
}




/** *****************************************************************
 *
 * CXMLNode::~CXMLNode
 *
 * Delete name/value pairs,  delete children.
 *
 **/
CXMLNode::~CXMLNode()
{
	unsigned int i;

	if ( 0 != m_pszName )
	{
		delete [] m_pszName;
	}

	for ( i=0; i<m_cDataCount; i++ )
	{
		if ( 0 != m_pszNameData[i] ) delete [] m_pszNameData[i];
		if ( 0 != m_pszValData[i]  ) delete [] m_pszValData[i];
	}

	for ( i=0; i<m_cChildCount; i++ )
	{
		if ( 0 != m_Children[i] ) delete m_Children[i];
	}

	CSimpleXMLParser::s_cNodesAllocated--;

	return;
}




/** *****************************************************************
 *
 * CXMLNode::setName
 *
 * Set the name of the node.  This is essentially the primary 
 * identifying key of the node.
 *
 * Strip off the '<' and '>' tags from the name. 
 *
 **/
void CXMLNode::setName( const char * pszName )
{
	static const char * pszNull = "(null)";
	int cLen = 0;
	char * p = 0;
	int cCount = 0;

	if ( 0 == pszName ) 
		p = (char*)pszNull;
	else 
		p = (char*)pszName;

	cLen = strlen( p );

	if ( p[0] == '<' )      cCount++;
	if ( p[cLen-1] == '>' ) cCount++;


	cLen = ( cLen - cCount + 2 );
	m_pszName = new char[ cLen ];


	if ( 0 != m_pszName )
	{
		memset( m_pszName, '\0', cLen );

		/** Trim off left-hand '<' char */
		if ( p[0] == '<' )    
		{ sprintf( m_pszName, "%s", p+1 ); }	
		else
		{ sprintf( m_pszName, "%s", p ); }

		/** Trim off right-hand '>' char */
		if ( m_pszName[cLen-2] == '>' )
			m_pszName[cLen-2] = '\0';

	}

	return;
}

