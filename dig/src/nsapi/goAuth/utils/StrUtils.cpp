//--file: StrUtils.cpp----------------------------------------------
//

/****************************************************
 * StrUtils.cpp
 *
 * Provide string manipulation utility functions
 *
 * Declared in StrUtils.h
 *
 * @version 1.x
 * $Revision: 1 $
 * $Modtime: 4/6/2000 9:00a $
 * @author Anne Fu
 *
 *****************************************************/


#include <string.h>
#include <stdio.h>
#include <ctype.h>


/** *****************************************************************
 *
 * strtrim
 *
 * Trim leading and trailing spaces in the source string and copy
 * into destination string
 *
 * NOTE: Destination string needs to be at least the same length with
 * the source string
 *
 * Returns length of destination string when done, or -1 if error 
 *
 **/

int strtrim(const char *src, char *dest)
{
	if (src == NULL || strlen(src) == 0 || dest == NULL)
		return -1;

	int i, j, k;
	int len = strlen(src);

	// Skip leading space

	for (i = 0; i < len; ++i)
	{
		if (!isspace(src[i]))
			break;
	}

	// Strip trailing space

	for (j = len - 1; j >= 0; --j)
	{
		if (!isspace(src[j]))
			break;
	}

	// Copy string

	int retLen = 0;
	for (k = i; k <= j; ++k)
	{
		dest[retLen++] = src[k];
	}
	dest[retLen] = '\0';

	return retLen;
}
