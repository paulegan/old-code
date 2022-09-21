//--file: CXMLNode.h-------------------------------------------------
//
#ifndef _CXMLNode_h_
#define _CXMLNode_h_


#ifdef __OS_SOLARIS__
	#include "./../utils/platform_defines.h"
#endif


const int g_cMaxDataPairs = 32;
const int g_cMaxNodeChildren = 32;


#ifdef __OS_WINNT__
class ostream;
#endif


class CXMLNode
{
	friend class CSimpleXMLParser;
	
	public:
	
		/** Get the name (identifier) of a node */
		const char * getName() const { return m_pszName; }
		
		/** See if the node has a child of the given name */
		bool hasChild( const char *, CXMLNode ** ) const;
		
		/** Get a reference to the node's parent */
		bool hasParent( CXMLNode ** ) const;

		/** See if the node has a property value of the given name */
		bool hasProperty( const char *, char *, unsigned int ) const;
		
		/** See if the node has a property value of the given name, get a count */
		bool hasProperty( const char *, unsigned int * ) const;

		/** See if the node has a property value of the given name and index */
		bool hasProperty( const char *, unsigned int, char *, unsigned int ) const;

		/** See is a child exists anywhere in the hierarchy underneath the child */
		bool childExists( const char *, unsigned int * ) const;

		/** Accessor for a child when multiple children exist */
		bool getChild( const char *, unsigned int, CXMLNode ** ) const;

		/** Get an ASCII representation of the hierarchy */
		#ifdef __OS_WINNT__
		friend ostream&  operator << ( ostream &, const CXMLNode * );
		#endif

		/** Free the children and name/value data pairs */
		~CXMLNode();


	protected:
		/** The name of the node (primary identifier) */
		char * m_pszName;

		/** Parameters are name and depth level in tree */
		CXMLNode( const char *, unsigned int );

		/** Add a name/value pair as a property of the node */
		bool addProperty( const char * );
		
		/** Set the name (primary identifier) of the node */
		void setName( const char * );
		
		/* Add a child node to this node */
		bool addChild( CXMLNode * );

		
	private:
		/** The nodes parent */
		CXMLNode * m_Parent;

		/** The nodes children */
		CXMLNode * m_Children[g_cMaxNodeChildren];
		
		/** A count of the nodes children */
		unsigned int m_cChildCount;
		
		/** A count of the number of nodes with the same name */
		unsigned int m_cCount;

		/** This nodes index if there are multiples */
		unsigned int m_cIndex;

		/** The names of the nodes name/value pairs */
		char * m_pszNameData[g_cMaxDataPairs];

		/** The values of the nodes name/value pairs */
		char * m_pszValData[g_cMaxDataPairs];

		/** A count of the number of name/value pairs */
		unsigned int m_cDataCount;

		/** The dpeth of the node in the hierarchy */
		unsigned int m_cDepth;
};


typedef CXMLNode * PCXMLNode;

#endif
