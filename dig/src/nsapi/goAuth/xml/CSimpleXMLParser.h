//--file: CSimpleXMLParser.h-----------------------------------------
//
// The declaration for a Simple XML Parser.
//
// To compile under Windows:  #define __OS_WINNT__
// To compile under Solaris:  #define __OS_SOLARIS__
//
//
// Legal XML tag values: 
// -----------------------------------------------
//    There are only 3 legal tag values parsed by this XML parser: 
//
//       Begin tags:       eg:   <order> á 
//       End tags:         eg:   </order> á 
//       Name/value tags:  eg: á <sold-to>john smith</sold-to> 
//
//
// Rules for XML files: 
// -----------------------------------------------
//  There can be no more than one tag on a line 
//
//  Begin/end tags can enclose either: 
//     More begin/end tags (nested) 
//     Name/value tags. 
//
//  Begin/end tags need not enclose any other tags. 
//
//  Each begin tag must have a matching end tag. 
//
//  Three illegal characters [ '<', '>', '&' ] must be 
//    escaped with the strings [ &lt; &gt; &amp; ] 
//
//
#ifndef _CSimpleXMLParser_h_
#define _CSimpleXMLParser_h_

#include "CXMLNode.h"

#ifdef __OS_SOLARIS__
	#include "./../utils/platform_defines.h"
#endif

class fstream;


/** The maximum number of errors that can be stored upon parsing */
const int g_cMaxErrors = 128;


class CSimpleXMLParser 
{

	public:
		CSimpleXMLParser();
		~CSimpleXMLParser();
		
		/** parsing */
		bool parse( const char *, const char * );

		/** How many errors occurred on unsuccessful parse? */
		unsigned int getErrorCount() const { return m_cErrCount; }

		/** Accessor for errors */
		const char * getError(unsigned int idx) const { return m_pszErrors[idx]; }

		/** Accessor for the root node - make a copy of it */
		CXMLNode * copyRoot() const { return m_Root; }
		
		/** Accessor for the root node - move it out of this class */
		CXMLNode * moveRoot();

		/** Memory leak check */
		static unsigned int s_cNodesAllocated;


	public:
		/** clean up between successive parses */
		void cleanUp();


	private:
		/** The root node of the hierarchy */
		CXMLNode * m_Root;

		/** The current line number */
		static unsigned int m_cLinenum;

		/** At least 1 begin tag was found */
		static bool m_bFoundBeginTag;

		/** A fatal parsing error has occurred */
		static bool m_bFatalError;

	
		/** Instantiate the hierarchy of nodes corresponding to XML input */
		bool recursiveParse( fstream &, fstream &, PCXMLNode *, int );

		/** The XML tag is a begin (open) tag <Name> */
		bool isBeginTag( const char *, unsigned int ) const;

		/** The XML tag is a name/value pair tag <Name>value</Name> */
		bool isValueTag( const char *, unsigned int ) const;

		/** The XML tag is an end (close) tag.  </Name> */
		bool   isEndTag( const char *, unsigned int ) const;

		/** Add an error to the current store of errors */
		bool   addError( const char * );

		/** Left,  then right-trim whitespace from a token */
		bool trim( char * ) const;



		/** Storage for error information */
		char * m_pszErrors[ g_cMaxErrors ];

		/** The number of current errors */
		unsigned int m_cErrCount;
};

#endif
