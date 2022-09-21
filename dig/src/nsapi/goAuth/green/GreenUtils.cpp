/*--file: GreenUtils.cpp---------------------------------------------*/

/**

    This file contains 5 functions to implement the 3 algorithms 
	required for generating, encrypting, and escaping GREENs - as 
	well as  2 functions for escaping/unescaping characters that 
	go into the GREEN.

    Function list -w- description:

        cryptXOR:      Encryption/decryption
		hexEscape:     HEX escape invalid characters in a string.
		hexUnescape:   Unescape HEX characters given a HEX string
		encodeHexMask: Hex encoding integers into a bit mask
		decodeHexMask: Hex decode a bit mask to find an encoded integer
		escapeInfo:    Escape special characters using HEX and a '%'
		unescapeInfo:  Unescape special characters using HEX and a '%'
	
    Utility functions:
        getEncryptionKey: Simulate a store of encryption keys

 
    Author: tom laramee (toml@starwave.com)
    Date: 10/16/98

*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#include "GreenUtils.h"



/** *****************************************************************
 *
 * cryptXOR
 *
 * XOR encrypt a string (lpszSrc) to a destination buffer (lpszDest) 
 * using the given encryption key.  This function also decrypts it's 
 * own output by running the output back through the function as 
 * input using the same encryption key.
 *
 * The output string will be the same size as the input string 
 * (assuming all buffers are big enough).
 *
 * @param lpszSrc: The source string to encrypt
 * @param cSrcLen: The length of the source string
 * @param lpszDest: The destination buffer.
 * @param cDestLen: The length/size of the destination buffer.
 * @param lpszCrypt: The encryption string
 *
 * @returns bool: true if the entire source string was encrypted,  
 *     false if not.
 *
 **/
bool cryptXOR( const unsigned char * lpszSrc, unsigned int cSrcLen, unsigned char * lpszDest,  unsigned int cDestLen, const unsigned char * lpszCrypt ) 
{
	unsigned int i;
	int cLen;

	try
	{	
		if ( (0==lpszSrc) || (0==lpszDest) || (0==lpszCrypt) ) 
			return false;

		cLen = strlen( (const char *)lpszCrypt );

		memset( lpszDest, '\0', cDestLen );
		cDestLen--;

		for ( i=0; (i<cSrcLen) && (i<cDestLen); i++ )
		{
			lpszDest[i] = ( lpszSrc[i] ^ lpszCrypt[ i%cLen ] );
		}

		if ( i>=cSrcLen ) return true;

		return false;

	
	}
	catch( ... )
	{
		return false;
	}
}



/** *****************************************************************
 *
 * hexEscape
 *
 * Map a single character to it's HEX equivalent using 2 HEX digits 
 * for each single character.  The output of this function will 
 * be 2X as long as the input.
 *
 * @param lpszSrc: The source string to escape
 * @param cSrcLen: The length of the source string
 * @param lpszDest: The destination buffer.
 * @param cDestLen: The length/size of the destination buffer.
 *
 * @returns bool: true if the entire source string was escaped, 
 *     false if not.
 * 
 **/
bool hexEscape( const unsigned char * lpszSrc, unsigned int cSrcLen, unsigned char * lpszDest, unsigned int cDestLen ) 
{
	char szVal[13] = {'\0'};
	int j;
	unsigned int i;
	unsigned char chu;

	try
	{

		if ( (0==lpszSrc) || (0==lpszDest) ) 
			return false;

		if ( cDestLen < ((cSrcLen*2)+1) )
			return false;

		memset( lpszDest, '\0', cDestLen );
		cDestLen--;

		for ( i=0; (i<cSrcLen); i++ )
		{
			chu = (unsigned char)lpszSrc[i];
			
			if( chu <= 0xf) { sprintf(szVal, "0%1X", chu ); }
			else           { sprintf(szVal,  "%2X", chu ); }

			j = i*2;
		
			lpszDest[j]   = szVal[0];
			lpszDest[j+1] = szVal[1];
		}

		if ( i>=cSrcLen ) return true;

		return false;


	}
	catch( ... )
	{
		return false;
	}
}



/** *****************************************************************
 *
 * hexUnescape
 *
 * Map each pair of HEX characters to a single byte character.
 *
 * @param lpszSrc: The source string to escape
 * @param cSrcLen: The length of the source string
 * @param lpszDest: The destination buffer.
 * @param cDestLen: The length/size of the destination buffer.
 *
 * @returns bool: true if the entire source string was unescaped, 
 *     false if not.
 * 
 **/
bool hexUnescape( const unsigned char * lpszSrc, unsigned int cSrcLen, unsigned char * lpszDest, unsigned int cDestLen )
{

	try
	{
		
		if ( (0==lpszSrc) || (0==lpszDest) ) 
			return false;

		char szVal[13] = {'\0'};
		int nVal = 0;
		unsigned int i;

		memset( lpszDest, '\0', cDestLen );
		cDestLen--;
	
		for( i=0; (i<cSrcLen)&&((i/2)<cDestLen); i+=2)
		{
			szVal[0] = lpszSrc[i];
			szVal[1] = lpszSrc[i+1];

			/*
			sscanf( szVal, "%x", &nVal ); 
			*/

			nVal = strtoul(szVal, 0, 16);
		
			lpszDest[ i/2 ] = nVal; 
		}

		if ( i>=cSrcLen ) return true;

		return false;

	}
	catch( ... )
	{
		return false;
	}
}



/** *****************************************************************
 *
 * encodeHexMask
 * 
 * Encode the given ID using HEX characters into a mask. 
 *
 * @param cId: The id to encode
 * @param lpszOldMask: The previous mask
 * @param lpszNewMask: The new mask to be generated.
 * @param cSize: The size of the new mask buffer 
 *
 * @returns bool: true if able to do the encoding, false if unable.
 *
 **/
bool encodeHexMask( unsigned int cId, const char * lpszOldMask, char * lpszNewMask, unsigned int cSize )
{
	static const char * pszLowercaseHex = { "abcdef" };
	unsigned int cNeeded=0, cLen=0;
	int i=0, j=0, cZeroes=0, n=0, nOld=0;
	char ch;

	try
	{

		if ( 0==lpszNewMask ) return false;
		if ( cId < 1 ) return false;
		memset( lpszNewMask, '\0', cSize );


		/** Enough room in destination buffer? */
		cNeeded = ( ((cId - 1) / 4) + 1 );
		if ( (cSize-1)<cNeeded ) return false;


		/** Get temp copy of the old mask */
		if ( 0 != lpszOldMask )
		{
			/*
			sprintf( lpszNewMask, "%s", lpszOldMask );
			*/

			strncpy(lpszNewMask, lpszOldMask, strlen(lpszOldMask) + 1);
			//strupr( lpszNewMask );
		}


		/** Mask string reverse, 'strupr' it on the fly */
		cLen = strlen( lpszNewMask );

		for ( i=0, j=cLen-1; (i<j); i++, j-- )
		{
			ch = lpszNewMask[i];

			if ( 0 != strchr( pszLowercaseHex, ch ) )
			{
				ch = (char)toupper(ch);
			}

			lpszNewMask[i] = lpszNewMask[j];
			lpszNewMask[j] = ch;
		}


		/** 
		 * Do bitwise OR of old mask (as hex string) with 2^nId. 
		 *
		 * Two cases:
		 *
		 * 1) Cookie's not long enough, just pad with zeroes and the appropriate digit 
		 * 2) Mask already has enough digits, just do the bitwise OR with the right char 
		 *
		 **/
		if ( cLen < cNeeded )
		{
			cZeroes = (cId - 1) / 4 - cLen;

			for ( i=0; i<cZeroes; i++ )
				strcat( lpszNewMask, "0" );

			switch ( cId % 4) 
			{
			  case 0: strcat( lpszNewMask, "8" );	break;
			  case 1: strcat( lpszNewMask, "1" );	break;
		 	  case 2: strcat( lpszNewMask, "2" );	break;
			  case 3: strcat( lpszNewMask, "4" );	break;
			}
		}
		else 
		{
			ch = lpszNewMask[ (cId - 1) / 4 ];

			if ( (ch>='0') && (ch<='9') ) 
			{
				n = (ch - '0');
			}
			else 
			{
				n = (10 + ch - 'A');
			}

			nOld = n;

			switch ( cId % 4 ) 
			{
			   case 0:  n |= 8;  break;
			   case 1:  n |= 1;  break;
			   case 2:  n |= 2;  break;
			   case 3:  n |= 4;  break;
			}

			if( nOld != n ) 
			{
				if ( n < 10 ) 
				{
					lpszNewMask[ (cId - 1) / 4 ] = (n + '0');
				}
				else 
				{
					lpszNewMask[ (cId - 1) / 4 ] = (n - 10 + 'A');
				}
			}
		}

		/** Reverse the mask */
		cLen = strlen( lpszNewMask );

		for ( i=0, j=cLen-1; (i<j); i++, j-- )
		{
			ch = lpszNewMask[i];
			lpszNewMask[i] = lpszNewMask[j];
			lpszNewMask[j] = ch;
		}
	
		return true;

	}
	catch( ... )
	{
		return false;
	}
}



/** *****************************************************************
 *
 * decodeHexMask
 *
 * Given an ID,  and a buffer containing an encoded string,  
 * determine if the ID has been encoded in the string.
 *
 * @param cId:       The ID to look for.
 * @param lpszBuffer: The buffer containing the encoded string.
 * @param pbError:   Bool to indicate any fatal processing errors.
 *
 * @returns bool: true if the value could be decoded,  false if not.
 *
 **/
bool decodeHexMask( unsigned int cId, const char * lpszBuffer, bool * pbError )
{
	char szCh[2] = {'\0'};
	int  cNeeded=0, cLen=0, cIdHexMask=0, cVal=0, cResult=0;
	

	try
	{
		/** default error condition is false */
		if ( 0 == pbError ) return false;
		(*pbError) = false;
	
		if ( 0 == lpszBuffer ) { (*pbError) = true; return false; }
		if ( cId<=0 )             { (*pbError) = true; return false; }

		/** Number of digits needed for mask... */
		cNeeded = ( ((cId - 1) / 4) + 1 );
		cLen = strlen( lpszBuffer );
	
		/** Not enough digits in mask to have encoded the ID */
		if ( cLen<cNeeded ) return false;

		switch ( cId % 4) 
		{
		  case 0: cIdHexMask = 8; break;
		  case 1: cIdHexMask = 1; break;
		  case 2: cIdHexMask = 2; break;
		  case 3: cIdHexMask = 4; break;
		}

		/** Need to read string from right to left */	
		szCh[0] = lpszBuffer[ cLen - cNeeded ];

		/*
		sscanf( szCh, "%x", &cVal ); 
		*/

		cVal = strtoul(szCh, 0, 16);

		cResult = (cVal&cIdHexMask);

		if ( (cVal&cIdHexMask)==cIdHexMask ) return true;
	
		return false;
	
	}
	catch( ... )
	{
		return false;
	}
}



/** *****************************************************************
 *
 * unescapeInfo
 *
 * Replace occurances of tokens '%HH' with their unescaped character 
 * equivalent. 
 *
 * On top of this,  we need to unescape characters that would have 
 * caused the C/C++ programming language to choke - eg: [ % ]
 *
 * Ex: %3B  ->  ;
 *     %2C  ->  ,
 *     %3D  ->  =
 *     %25  ->  %
 *
 * Return true if entire string parsed,  false if not.
 *
 **/
bool unescapeInfo( const char * pszSrc, char * pszDest, unsigned int cDestSize )
{
	if ( 0==pszSrc || 0==pszDest ) return false;

	unsigned int cLen = 0;
	unsigned int i, j;
	char szCh[13] = {'\0'};
	unsigned long ul;

	try
	{

		memset( pszDest, '\0', cDestSize );
		cDestSize--;

		cLen = strlen(pszSrc);	
	
		for ( i=0, j=0; (i<cLen) && (j<cDestSize); i++ )
		{
			if ( pszSrc[i] != '%' )
			{
				pszDest[j++] = pszSrc[i];
			}
			else
			{
				// invalid format
				if ( ('\0'==pszSrc[i+1]) || ('\0'==pszSrc[i+2]) ) 
				{
					pszDest[j++] = pszSrc[i];
					continue;
				}

				szCh[0] = pszSrc[i+1];
				szCh[1] = pszSrc[i+2];
			
				ul = strtoul( szCh, 0, 16 );
			
				pszDest[j++] = (char)ul;

				i+=2;
			}
		}	

		if ( i>=cLen ) return true;
	
		return false;

	}
	catch( ... )
	{
		return false;
	}
}



/** *****************************************************************
 *
 * escapeInfo
 *
 * Replace occurances of "special" characters [ , ; = % ] with their 
 * hexadecimal equivalents in the form "%HH".
 *
 * On top of this,  we need to escape characters that will cause the 
 * C/C++ programming language to choke - eg: [ % ]
 *
 * Ex: ;  ->  %3B
 *     ,  ->  %2C
 *     =  ->  %3D  
 *     %  ->  %25  
 *
 *	   %  ->  %%  
 *
 * Therefore,  %moo would be translated to %%25moo
 *
 * Return true if entire string parsed,  false if not.
 *
 **/
bool escapeInfo( const char * pszSrc, char * pszDest, unsigned int cDestSize )
{
	if ( 0==pszSrc || 0==pszDest ) return false;

	static const char * pszChars = ",;=%";
	unsigned int cLen = 0;
	unsigned int i, j;
	char szCh[13] = {'\0'};
	int hex;
	
	try
	{
		memset( pszDest, '\0', cDestSize );
		cDestSize--;
	
		cLen = strlen(pszSrc);	

		for ( i=0, j=0; (i<cLen) && (j<cDestSize); i++ )
		{
			/** char is special */
			if ( 0 != strchr( pszChars, pszSrc[i] )) 
			{
			
				if ( (j+4)>=cDestSize ) /** No room */
				{
					pszDest[j++] = pszSrc[i];
				}
				else	/** there's room for 4 chars */
				{
					hex = (int)pszSrc[i];
			
					if ( hex <= 15 )
					{ sprintf( szCh, "%%0%1X", hex ); }
					//{ sprintf( szCh, "%%%%0%1X", hex ); }
					else
					{ sprintf( szCh, "%%%2X", hex ); }
					//{ sprintf( szCh, "%%%%%2X", hex ); }
	
					strcat( pszDest, szCh );
					j+=3;
					//j+=4;
				}
			}
			else
			{
				pszDest[j++] = pszSrc[i];
			}
		}

		if ( i>=cLen ) return true;

		return false;
	
	}
	catch( ... )
	{
		return false;
	}
}



/** *****************************************************************
 *
 * getCryptionKey
 *
 * Extract a decryption/encryption key from a local store of keys.  
 * Eventually, this will be an HTTP GET request with the index.
 *
 **/
const unsigned char * getCryptionKey( unsigned int idx )
{
	static const char * pszCrypt[16] = { 
	 "&n'+,#'/*rv{}qgq1w+/w#cdnr/+,{}r/*de}68kik,+,/*{*25+,/w2256{%+,/w#q#n+,/#",
	 "#n+,/+k#;*+,/'r:'d*'3,}{w+K w'K:'+}e#';dq#'lq#'+d'K#!/+k#;{l,+,/n{n+,/+xq",
	 "wknw''{[iwk{KK{nl]!/w{%'l##w#'i:{nl]'/*{q#'ld;r'}{nlwb!/*de}'cdsl;AQ}/?.;",
	 "ek;dci&bK'(q)-[w]*%n+r3#l,{}:24tca-O;m.vpbksearv790m1`11569iry#j&160l<|.",
	 "dxgjidrjfigurei?}{g/45c'${?};sotle9t958iojhgikeqerf790[.pp0nyq4%4P{}?:>s",
	 "plsfg45dbmo.8e.'Ph'nglui2wrbt,1`AERG67_m4ulhu_R'l47yeh_33737wag2D3wf.f,.",
	 "sahgklfaghf'klgafs)~';i\52T#'lfhj'lghal'kfgshalfhglkfdhg'hkl.u90p7i4Y#%U",
	 "-22&&d%80-78]);f(!c-1839++c,is/s,/x,y,b);k/--i(!(c-184)iY&^x131$#&*b46^x",
	 "11+(a+f)%80,'',(+a+f)/80+1,1+(f+a+f)%80,'n(f+a+f);k;{{{.hjyu786&(*_h/w2g",
	 "p(asi(bcb(ao#j&]57%ce167(beq(ag'[i!=[105)176't141n(0145170i1640511S!&!&E",
	 "-s;v=a[j*=v]-a[i],k=i<s,j+=(v=j<s&&(!k&&!!printf(2%c-(!l<<!j),??dsafv#2Q",
	 "[l^v?(l^j)12])&:M'$#&*Ydsuf98425htpe9$w[i]<s&&v&&v-iaeb09lgnm'|3321r42%#^",
	 "w;r=d;}f;;{B(3),y);!247b()||(w=q34tf(l))<1)ddit(0);if(w&1||w>M||nw>y)B(1)",
	 "*x,*y,m,t;S(d,v,f,_,a,b)l*v;{l c,v+100,j=d<u-1?a:-900,wizig06111[E/51~/4]",
	 "732kjhdf8jvm*f&^t-c-l{:>?~``akj'<fdsdf}ds236+|,3+!~=%&^,-12,-1,12,dafvuw1",
	 "};[E)~'8/2]{i*T#j[E)~'2*[i*T#iY&^x]]}{$j<{p>30};145ss170i164ccxasxsrv4y=-" 
	};

	if ( (idx>=0) && (idx<16) )
	{
		return (unsigned char *)pszCrypt[idx];
	}


	/** Invalid index request */
	return (unsigned char *)pszCrypt[0];
}

