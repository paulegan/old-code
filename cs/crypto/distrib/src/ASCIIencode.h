
/* ASCIIencode.h header file for ASCIIencode.c		lumpy@cs.ucc.ie		*/

#ifndef ASCIIencode_headder
#define ASCIIencode_headder

void ASCIIencodeBlock(char *block, int blockLen, char *encodedBlock, int *encodedBlockLen);
void ASCIIdecodeBlock(char *block, int *blockLen, char *encodedBlock, int encodedBlockLen);

#endif
