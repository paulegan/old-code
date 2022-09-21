/*
 * bool.h
 * Defintion of a `datatype' for booleans and introduces 
 * constant values TRUE and FALSE.
 * Needed if lclint is to OK programs that use boolean variables! 
 */
#ifndef BOOL_H
#define BOOL_H

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE (! FALSE)
#endif

typedef int bool;

# define bool_initMod()
# define bool_unparse(b) ((b) ? "true" : "false" )
# define bool_not(b) ((b) ? FALSE : TRUE)
# define bool_equal(a,b) ((a) ? (b) : !(b))

# endif
