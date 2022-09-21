//--file: CInterpreter.h---------------------------------------------
//
// This is the declaration for a class used to interpret the logic 
// within a Premium Area definition that either allows or disallows 
// a user access to a premium area based on the tokens that the 
// user has (or doesn't have).
//
// The syntax of the expression are similar to:
//
//     21 AND 34, 56 AND 77 NOT 102, NOT 12
//
// The tokens that the expression are evaluated against are 
// comma-delimited integers.  
//
//     12,34,71,123,
//
// Before evaluation,  the expression is converted to an 
// intermediary format:
//
//    21-&-34-,56-&-77-!-102,!-12-
//
//
// The "evaluate" function returns "true" if the tokens possessed 
// satisfy the expression criterion,  "false" if they do not.  
// A call to any of the error accessors below provides the 
// necessary information for a client to operate correctly 
// in an error condition.
//
#ifndef _CInterpreter_h_
#define _CInterpreter_h_


#define INTERPRET_NOERROR         1
#define INTERPRET_ERROR_FATAL     2
#define INTERPRET_ERROR_NOTFATAL  4
#define INTERPRET_INVALID_EXPR    8
#define INTERPRET_MISSING_TOKENS  16
#define INTERPRET_MISSING_EXPR    32
#define INTERPRET_ILLEGAL_CHAR    64
#define INTERPRET_OVERFLOW        128



/** The maxiumum length of an expression after conversion to inermediary format */
const int g_cMaxExpressionSize = 500;

#include "platform_defines.h"

class CInterpreter 
{
	public:
		CInterpreter();
		~CInterpreter();

		/** Use either (1) */
		
		/** Evaluate the expression against the given tokens */
		bool evaluate( const char * pszTokens, const char *, unsigned long );
		
		/** ...or (2) [4 functions] */

		/** Convert expression to intermediary format */
		bool convertExpression( const char *, unsigned int );

		/** Accessor for converted expression */
		const char * getExpr() const { return m_szExpr; }

		/** Evaluate the expression against the given tokens */
		bool setExpr( const char * pszExpr );

		/** Evaluate the expression against the given tokens */
		bool evaluate( const char * pszTokens, unsigned long );
		

		

	public:

		/** did the evaluation fail on a NOT */
		inline bool failedOnNOT() const { return m_bFailedOnNOT; }


		/** Original expression accessors */
		//inline bool hasOriginalExpression() const { return (0!=m_pszOriginalExpression); }
		//inline const char * getOriginalExpression() const { return (m_pszOriginalExpression); }
		
		
		/** Error handling accessors */
		inline bool hasError()              const { return ( m_cErr!=INTERPRET_NOERROR );              }
		inline bool hasFatalError()         const { return ( (m_cErr&INTERPRET_ERROR_FATAL) != 0 );    }
		inline bool hasInvalidExprError()   const { return ( (m_cErr&INTERPRET_INVALID_EXPR) != 0 );   }
		inline bool hasMissingExprError()   const { return ( (m_cErr&INTERPRET_MISSING_EXPR) != 0 );   }
		inline bool hasMissingTokensError() const { return ( (m_cErr&INTERPRET_MISSING_TOKENS) != 0 ); }
		inline bool hasIllegalCharError()   const { return ( (m_cErr&INTERPRET_ILLEGAL_CHAR) != 0 );   }
		inline bool hasOverflowError()      const { return ( (m_cErr&INTERPRET_OVERFLOW) != 0 );       }

		/* Run a self-test on the evaluation logic */
		void selfTest();

		/** Clear all errors/warnings */
		bool clear();



	private:

		
		/** Evaluate a single logic expression against tokens */
		bool evaluate( const char *, unsigned int, const char *, unsigned int, unsigned long );
		
		/** Is a given tokens contained within the list of tokens */
		bool tokenContained( char *, const char *, unsigned int, unsigned long );

		/** Part of the public self-test functionality */
		bool selfTest( bool, const char *, const char * );


	private:
		/** Error information */
		long m_cErr;

		/** A copy of the expression to evaluate */
		char m_szExpr[g_cMaxExpressionSize+1];

		bool m_bFailedOnNOT;
};

#endif


