/*--file: GreenUtils.h---------------------------------------------*/

/**
 * Utility functions for parsing/formatting/encrypting GREENs.
 * ============================================================
 *
 * cryptXOR:      Encrypt/decrypt a string using the given key.
 *
 * hexEscape:     Map a single character to it's equivalent 2 
 *                character HEX representation.
 * 
 * hexUnescape:   Map each pair of HEX characters to a single 
 *                byte character.
 * 
 * encodeHexMask: Encode the given ID using HEX characters 
 *                into a mask. 
 *
 * decodeHexMask: Given an ID,  and a buffer containing an 
 *                encoded string, determine if the ID has been 
 *                encoded in the string.
 *
 * unescapeInfo:  Replace occurances of tokens '%HH' with their 
 *                unescaped character equivalent. 
 *
 * escapeInfo:    Replace occurances of "special" characters 
 *                [ , ; = % ] with their hexadecimal equivalents 
 *                in the form "%HH".
 *
 *
 * Author: tom laramee (toml@starwave.com)
 * Date: 10/16/98
 *
 **/

#ifndef _GreenUtils_h_
#define _GreenUtils_h_


#ifdef __OS_SOLARIS__
	#include "./../utils/platform_defines.h"
#endif


bool      cryptXOR( const unsigned char *, unsigned int, unsigned char *, unsigned int, const unsigned char * );

bool     hexEscape( const unsigned char *, unsigned int, unsigned char *, unsigned int );
bool   hexUnescape( const unsigned char *, unsigned int, unsigned char *, unsigned int );

bool encodeHexMask( unsigned int, const char *, char *, unsigned int );
bool decodeHexMask( unsigned int, const char *, bool * );

bool  unescapeInfo( const char * , char *, unsigned int );
bool    escapeInfo( const char * , char *, unsigned int );


const unsigned char * getCryptionKey( unsigned int idx );

#endif
