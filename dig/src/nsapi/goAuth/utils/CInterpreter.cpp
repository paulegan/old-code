//--file: CInterpreter.h---------------------------------------------
//
#include "CInterpreter.h"
#include "CLog.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


/** Not '&', '!', ',', or a digit */
const char g_chDelim = '-';
const char g_chBreak = ',';
const char g_chAnd   = '&';
const char g_chNot   = '!';



/** *****************************************************************
 *
 * CInterpreter::clear
 *
 **/
bool CInterpreter::clear()
{
	m_bFailedOnNOT = false;

	m_cErr = INTERPRET_NOERROR;
	memset( m_szExpr, '\0', g_cMaxExpressionSize+1);
	return true;
}


/** *****************************************************************
 *
 * CInterpreter::CInterpreter
 *
 **/
CInterpreter::CInterpreter()
{
	this->clear();
	return;
}


/** *****************************************************************
 *
 * CInterpreter::setExpr
 *
 * This function (wrongly) assumes that whatever is setting 
 * the expression got this expression using "getExpr()",  
 * (1) the expression is valiud and (2) it's not too long.
 *
 * This is for expressions that need to be evaluated many 
 * many times but only need to be converted once.
 *
 **/
bool CInterpreter::setExpr( const char * pszExpr )
{
	if ( 0==pszExpr ) return false;

	memset( m_szExpr, '\0', g_cMaxExpressionSize+1);
	m_cErr = INTERPRET_NOERROR;

	//sprintf( m_szExpr, "%s", pszExpr );
	strncpy(m_szExpr, pszExpr, g_cMaxExpressionSize+1);
	return true;
}




/** *****************************************************************
 *
 * CInterpreter::~CInterpreter
 *
 **/
CInterpreter::~CInterpreter()
{

}



/** *****************************************************************
 *
 * CInterpreter::evaluate  (public)
 *
 * Assumes that the user of this function has called either:
 *
 *    convertExpression()
 *    getExpr(),
 *    setExpr()
 *    evaluate()
 *    
 * or
 *
 *    convertExpression()
 *    evaluate()
 *
 **/
bool CInterpreter::evaluate( const char * pszTokens, unsigned long cRequest )
{
	bool bEval;
	char * psz = 0;
	char szDelim[2] = { g_chBreak, '\0' };
	unsigned int cELen, cTLen;


	if ( 0 == pszTokens )
	{
		m_cErr |= INTERPRET_ERROR_FATAL;
		m_cErr |= INTERPRET_MISSING_TOKENS;
		return false;
	}

	/** If these are zero-length,  it may be ok */
	cTLen = strlen( pszTokens );

	if ( 0 == cTLen )
	{
		m_cErr |= INTERPRET_MISSING_TOKENS;
	}

	cELen = strlen( m_szExpr );


	if ( 0 == cELen )
	{
		m_cErr |= INTERPRET_MISSING_EXPR;
	}


	/** Interpret the contents of m_szExpr */
	psz = strtok( m_szExpr, szDelim );


	while ( 0 != psz )
	{
		/** Since the tokens are "OR", any one being */
       	/** true makes the entire expression true.   */
		cELen = strlen( psz );
		bEval = evaluate( pszTokens, cTLen, psz, cELen, cRequest );

		if ( true == bEval )
		{
			return true;
		}

		psz = strtok( 0, szDelim );
	}

	return false;
}



/** *****************************************************************
 *
 * CInterpreter::evaluate	(private)
 *
 **/
bool CInterpreter::evaluate( const char * pszTokens, const char * pszExpr, unsigned long cRequest )
{
	bool bEval;
	char * psz = 0;
	char szDelim[2] = { g_chBreak, '\0' };
	unsigned int cELen, cTLen;

	memset( m_szExpr, '\0', g_cMaxExpressionSize+1);
	m_cErr = INTERPRET_NOERROR;


	/** NULL pointers are a fatal error */
	if ( 0 == pszExpr )
	{
		m_cErr |= INTERPRET_ERROR_FATAL;
		m_cErr |= INTERPRET_MISSING_EXPR;
		return false;
	}

	if ( 0 == pszTokens )
	{
		m_cErr |= INTERPRET_ERROR_FATAL;
		m_cErr |= INTERPRET_MISSING_TOKENS;
		return false;
	}


	/** If these are zero-length,  it may be ok */
	cELen = strlen( pszExpr );
	cTLen = strlen( pszTokens );

	if ( 0 == cELen )
	{
		m_cErr |= INTERPRET_MISSING_EXPR;
	}

	if ( 0 == cTLen )
	{
		m_cErr |= INTERPRET_MISSING_TOKENS;
	}


	/** Conversion will always be non-fatal */
	this->convertExpression( pszExpr, cELen );


	cELen = strlen( m_szExpr );

	if ( 0 == cELen )
	{
		m_cErr |= INTERPRET_MISSING_EXPR;
	}



	/** Interpret the contents of m_szExpr */
	psz = strtok( m_szExpr, szDelim );

	while ( 0 != psz )
	{
		/** Since the tokens are "OR", any one being */
        /** true makes the entire expression true.   */
		cELen = strlen( psz );
		bEval = evaluate( pszTokens, cTLen, psz, cELen, cRequest );

		if ( true == bEval )
		{
			return true;
		}

		psz = strtok( 0, szDelim );
	}

	return false;
}



/** *****************************************************************
 *
 * CInterpreter::convertExpression
 *
 * Given an expression,  convert the tokens to an intermediate 
 * format for interpretation.  
 *
 *   o  Eliminate whitespace.  
 *   o  "AND" becomes '&'  (g_chAnd) 
 *   o  "NOT" becomes '!'  (g_chNot)
 *   o  All tokens are delimited by '-'  (g_chDelim)
 *   o  All expression ORs remain ','  (g_chBreak)
 *
 * eg:
 *     "1 AND 2 AND 3, 31 AND 415 AND NOT 522"
 *
 * becomes
 *
 *      1-&-2-&-3-,21-&-415-&-!-522-
 *
 * 
 * Sets the error condition on an error.
 *
 * Returns true on successful conversion,  false on error.
 * 
 * 
 **/
bool CInterpreter::convertExpression( const char * psz, unsigned int cELen )
{
	static const char pszLegal[17] = {g_chBreak,'A','N','D','O','T','0','1','2','3','4','5','6','7','8','9','\0'};
	unsigned int i, j;


	for ( i=0, j=0; i<cELen && j<(g_cMaxExpressionSize-1); i++ )
	{
		/** Skip whitespace */
		if ( isspace(psz[i]) ) {
			continue;
		}

		/** Skip illegal characters */
		if ( 0 == strchr( pszLegal, toupper(psz[i]) ) )	{
			m_cErr |= INTERPRET_ILLEGAL_CHAR;
		}

		/** Expression break,  logical equivalent of "OR" */
		if ( g_chBreak == psz[i] ) {
			m_szExpr[j++] = g_chBreak;
			continue;
		}

		/** Numbers */
		if ( isdigit(psz[i]) ) {	
			while ( isdigit(psz[i]) && (j<(g_cMaxExpressionSize-1)) ) 
			{
				m_szExpr[j++] = psz[i];
				i++;
			}

			i-=1;
			m_szExpr[j++] = g_chDelim;
			continue;
		}

		/** 'AND' */
		if ( 'A'==toupper(psz[i]) ) {
			if (  '\0'!=psz[i+1]  &&  'N'==toupper(psz[i+1])  ) 
			{
				if (  '\0'!=psz[i+2]  &&  'D'==toupper(psz[i+2])  ) 
				{
					i+=2;
					m_szExpr[j++] = g_chAnd;
					m_szExpr[j++] = g_chDelim;
					continue;
				}
				else continue;
			}
			else continue;
		}

		/** 'NOT' */
		if ( 'N'==toupper(psz[i]) ) {
			if (  '\0'!=psz[i+1]  &&  'O'==toupper(psz[i+1])  ) 
			{
				if (  '\0'!=psz[i+2]  &&  'T'==toupper(psz[i+2])  ) 
				{
					i+=2;
					m_szExpr[j++] = g_chNot;
					m_szExpr[j++] = g_chDelim;
					continue;
				}
				else continue;
			}
			else continue;
		}
	}


	/** The expression was too long */
	if ( j >= g_cMaxExpressionSize )
	{
		for ( j=g_cMaxExpressionSize; j>=0; j-- )
		{
			/** Trim everything after the last 'OR' so the expression is logical */
			if ( g_chBreak == m_szExpr[j] )
			{
				m_szExpr[j] = '\0';
				break;
			}
		}

		m_cErr |= INTERPRET_OVERFLOW;
		return false;
	}

	return true;
}



/** *****************************************************************
 *
 * CInterpreter::evaluate
 *
 * Given a set of tokens,  and a single expression,  evaluate 
 * the expression for truth agains the tokens.
 *
 * Eg:     Tokens          Expression        Will return
 *         ==============  ==============    ============
 *         "23,34,121"     23-&-34-&-121-    true
 *         "23"            23-&-34-          false
 *         "23,34,121"     23-&-34-&-!-121-  false
 *
 **/
bool CInterpreter::evaluate( const char * pszTokens, unsigned int cTLen, const char * pszExpr, unsigned int cELen, unsigned long cRequest )
{
	char * pszBegin = 0;
	bool bNot = false; 
	char szDelim[2]  = { g_chDelim, '\0' };
	bool bUserHasToken;
	char szExpr[257] = {'\0'};
	char szToken[20] = {'\0'};
	unsigned int i, j;


	/** Assume we're gonna fail not on a not... */
	m_bFailedOnNOT = false;
 
 
	/** No tokens,  no access... unless expr is zero length...*/
	if ( 0==cTLen ) 
	{
		if ( 0==cELen ) return true;  
		else return false;
	}

	memset(szExpr, '\0', 257);

	/** Make a copy of the expression to evaluate against */
	if ( cELen >= 256 )
	{
		m_cErr |= INTERPRET_OVERFLOW;
		memcpy( szExpr, pszExpr, 256 );
		cELen = 256;
	}
	else
	{
		//sprintf( szExpr, "%s", pszExpr );
		strncpy(szExpr, pszExpr, cELen+1);
	}


	/** get first token */
	memset( szToken, '\0', 20 );
	for ( i=0, j=0; (szExpr[i]!=g_chDelim) && (i<cELen) && (j<19); i++, j++ )
	{
		szToken[j] = szExpr[i];
	}
	i++;


	while ( szToken[0] != '\0' )
	{
		/** Found a number */
		if ( isdigit( szToken[0] ) )
		{
			bUserHasToken = tokenContained( szToken, pszTokens, cTLen, cRequest );	

			/** If it's a NOT,  and the user has it,  return false */
			if ( true == bNot )
			{
				if ( true == bUserHasToken )
				{	
					m_bFailedOnNOT = true;
					return false; 
				}
			}
			/** If it's not a NOT,  and the user doesn't have it,  return false */
			else
			{
				if ( false==bUserHasToken )
				{	
					return false; 
				}
			}

			bNot = false;
		}
		/** Found either '!' or '&' */
		else 
		{
			if ( g_chNot == szToken[0] )
			{
				bNot = true;
			}
			else //if ( g_chAnd == szToken[0] )
			{
				bNot = false;
			}			
		}


		/** Get next token */
		memset( szToken, '\0', 20 );
		for ( j=0; (szExpr[i]!=g_chDelim) && (i<cELen) && (j<19); i++, j++ )
		{
			szToken[j] = szExpr[i];
		}
		i++;
	}


	/** The user has all of the tokens... */
	return true;
}



/** *****************************************************************
 *
 * CInterpreter::userHasToken
 *
 * Given a list of tokens (eg: 23,45,92,121),  and a token (eg: 12),
 * see if the token is within the list of tokens.
 *
 * Return true if so,  false if not.
 *
 **/
bool CInterpreter::tokenContained( char * pszToken, const char * pszTokens, unsigned int cTLen, unsigned long cRequest )
{
	char sz[20] = {'\0'};
	unsigned int cLen=0;

	/** It's either the first one... */
	cLen = strlen( pszToken );

	if ( 0 == strncmp( pszTokens, pszToken, cLen ) )
	{
		return true;
	}

	/** Bad problems used to be here... */
	if ( cLen > 19 )
	{
		m_cErr |= INTERPRET_OVERFLOW;
		return false;
	}

	sprintf( sz, ",%s,", pszToken );


	/** ...or another one... */
	if ( 0 == strstr( pszTokens, sz ) )
	{
		return false;
	}
	else
	{
		return true;
	}
}




/** *****************************************************************
 *
 * CInterpreter::selfTest
 *
 * One of the 2 functions used to run a self-test of the logic 
 * used to evaluate expressions against a list of tokens.
 *
 * Param 1: What the expression *should* evaluate to 
 * Param 2: The user's tokens 
 * Param 3: The expression 
 *
 **/
bool CInterpreter::selfTest( bool bTruth, const char * pszTokens, const char * pszExpr )
{
	static bool bHeader = false;
	unsigned long c = 0L;

	if ( false == bHeader )
	{
			printf ("\r\n");
			printf ("testInterpreter>      STATUS: [result] [      tokens]  [expression]\r\n" );
			printf ("=================================================================");
			printf ("============================================\r\n");
			bHeader = true;
	}


	bool b = this->evaluate( pszTokens, pszExpr, c );

	if ( true == this->hasError() )
	{
		if ( true == this->hasFatalError() )
			printf ("testInterpreter> FATAL ERROR: ");
		else
			printf ("testInterpreter>       ERROR: ");
	}
	else
	{
			printf ("testInterpreter>    NO ERROR: ");
	}

	printf ( "[%6s] [%12s] [%25s]", ((true==b) ? ("true") : ("false")), pszTokens, pszExpr );

	if ( true == this->hasError() )
	{
		if ( this->hasInvalidExprError() )   printf("{invalid expr}"   );
		if ( this->hasMissingExprError() )   printf("{missing expr}"   );
		if ( this->hasMissingTokensError() ) printf("{missing tokens}" );
		if ( this->hasIllegalCharError() )   printf("{illegal char}"   );
		if ( this->hasOverflowError()    )   printf("{overflow}"       );
		printf("\r\n");
	}
	else
	{
		printf("\r\n");
		//assert( bTruth == b );
	}

	return b;
}


/** *****************************************************************
 *
 * CInterpreter::selfTest
 *
 * The public entry into the self-test.  Run a set of tokens 
 * against a set of expressions and test the result against 
 * the (known) anticipated result.
 *
 **/
void CInterpreter::selfTest()
{
	this->selfTest( true,  "1,2,3,",     "1 AND 2 AND 3, 31 AND 415 NOT 522, 6 AND 722 AND 8 AND NOT 9" );
	this->selfTest( true,  "6,722,8,",   "1 AND 2 AND 3, 31 AND 415 NOT 522, 6 AND 722 AND 8 AND NOT 9" );
	this->selfTest( false, "6,722,8,9,", "1 AND 2 AND 3, 31 AND 415 NOT 522, 6 AND 722 AND 8 AND NOT 9" );
	this->selfTest( false, "1,2,",       "1 AND 2 AND 3, 31 AND 415 NOT 522, 6 AND 722 AND 8 AND NOT 9" );
	this->selfTest( false, "1,",         "1 AND 2 AND 3, 31 AND 415 NOT 522, 6 AND 722 AND 8 AND NOT 9" );

	this->selfTest( false, "6,722,8,9,", "1 AND 2 AND 3, 31 AND 415 NOT 522, 6 AND 722 NOT 8 NOT 9"     );
	this->selfTest( false, "6,722,8,",   "1 AND 2 AND 3, 31 AND 415 NOT 522, 6 AND 722 NOT 8 NOT 9"     );
	this->selfTest( true,  "6,722,",     "1 AND 2 AND 3, 31 AND 415 NOT 522, 6 AND 722 NOT 8 NOT 9"     );

	this->selfTest( false, "1,2,3,",     "NOT 1 AND NOT 2, 31 AND 415 NOT 522, 6 AND 722 NOT 8 NOT 9"   );
	this->selfTest( false, "1,3,",       "NOT 1 AND NOT 2, 31 AND 415 NOT 522, 6 AND 722 NOT 8 NOT 9"   );
	this->selfTest( true,  "3,",         "NOT 1 AND NOT 2, 31 AND 415 NOT 522, 6 AND 722 NOT 8 NOT 9"   );
	

	this->selfTest( true,  "1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20",       "1 AND 2 AND 3" );

	this->selfTest( true,  "415,31,",    "NOT 1 AND NOT 2, 31 AND 415 NOT 522, 6 AND 722 NOT 8 NOT 9"   );
	this->selfTest( true,  "722,6,",     "NOT 1 AND NOT 2, 31 AND 415 NOT 522, 6 AND 722 NOT 8 NOT 9"   );
	this->selfTest( false, "722,6,8,",   "NOT 1 AND NOT 2, 31 AND 415 NOT 522, 6 AND 722 NOT 8 NOT 9"   );

	this->selfTest( false, "9,8,722,",   "NOT 9 NOT 8 AND 722"     );
	this->selfTest( false, "9,722,",     "NOT 9 NOT 8 AND 722"     );
	this->selfTest( false, "8,722,",     "NOT 9 NOT 8 AND 722"     );
	this->selfTest( true,  "722,",       "NOT 9 NOT 8 AND 722"     );

	this->selfTest( false, "9,722,8,",   "NOT 9 AND 722 NOT 8"     );
	this->selfTest( false, "722,8,",     "NOT 9 AND 722 NOT 8"     );
	this->selfTest( false, "9,722,",     "NOT 9 AND 722 NOT 8"     );
	this->selfTest( true,  "722,",       "NOT 9 AND 722 NOT 8"     );

	this->selfTest( true,  "1,2,3,",     ""                                                             );
	this->selfTest( false, "1,2,3,",     "1 AND 3 OR 2 AND 5"                                           );
	this->selfTest( true,  "1,",         "1"                                                            );
	this->selfTest( true,  "1,",         " "                                                            );
	this->selfTest( false, "1,",         "NOT 1"                                                        );
	this->selfTest( true,  "2,",         "NOT 1"                                                        );
	this->selfTest( false, "1,",         "AND NOT 1"                                                    );
	this->selfTest( true,  "2,",         "AND NOT 1"                                                    );
	this->selfTest( true,  "1,2,",       "1, 2"                                                         );
	this->selfTest( true,  "2,",         "1, 2"                                                         );
	this->selfTest( true,  "1,",         "1, 2, AND NOT 3"                                              );

	this->selfTest( true,  "5,",         "1, 2, NOT 3"                                              );

	this->selfTest( false, "3,4,5,",     "1, 2, NOT 3"                                                  );

	this->selfTest( true,  "1,2,3,",     "1, 2, 3"                                                      );
	this->selfTest( false, "",           "1, 2, 3"                                                      );
	this->selfTest( true,  "",           ""                                                             );
	return;
}

