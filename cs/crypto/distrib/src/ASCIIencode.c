

/* ASCIIencode.c		lumpy@cs.ucc.ie			20.8.1996		*/

/* Encodes and decodes data according to RFC-1421					*/
/* Adapted from encode.c from rsaeuro							*/


#define ENC(c) ((int)bintoasc[((c) & 077)])
#define PAD    '='

static unsigned char bintoasc[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

static unsigned char asctobin[] = {
	0200,0200,0200,0200,0200,0200,0200,0200,
	0200,0200,0200,0200,0200,0200,0200,0200,
	0200,0200,0200,0200,0200,0200,0200,0200,
	0200,0200,0200,0200,0200,0200,0200,0200,
	0200,0200,0200,0200,0200,0200,0200,0200,
	0200,0200,0200,0076,0200,0200,0200,0077,
	0064,0065,0066,0067,0070,0071,0072,0073,
	0074,0075,0200,0200,0200,0200,0200,0200,
	0200,0000,0001,0002,0003,0004,0005,0006,
	0007,0010,0011,0012,0013,0014,0015,0016,
	0017,0020,0021,0022,0023,0024,0025,0026,
	0027,0030,0031,0200,0200,0200,0200,0200,
	0200,0032,0033,0034,0035,0036,0037,0040,
	0041,0042,0043,0044,0045,0046,0047,0050,
	0051,0052,0053,0054,0055,0056,0057,0060,
	0061,0062,0063,0200,0200,0200,0200,0200
};


void encode(char *out, char *in, int count)
{
 int c1, c2, c3, c4;

 c1 = *in >> 2;
 c2 = ((*in << 4) & 060) | ((in[1] >> 4) & 017);
 c3 = ((in[1] << 2) & 074) | ((in[2] >> 6) & 03);
 c4 = in[2] & 077;
 *out++ = ENC(c1);
 *out++ = ENC(c2);
 if (count == 1){
   *out++ = PAD;
   *out++ = PAD;
 }
 else{
   *out++ = ENC(c3);
   if (count == 2) *out++ = PAD;
	    else *out++ = ENC(c4);
 }
}/* encode */


void ASCIIencodeBlock(char *block, int blockLen, char *encodedBlock, int *encodedBlockLen)
{
 unsigned int i, lastLen;

 *encodedBlockLen = 0;
 if(blockLen<1) return;

 for(i = 0, lastLen = blockLen; i < blockLen; i += 3){
   encode(&encodedBlock[4*i/3], &block[i], (lastLen >= 3 ? 3 : lastLen));
   lastLen -= 3;
   *encodedBlockLen += 4;
 }
}/* ASCIIencodeBlock */


void ASCIIdecodeBlock(char *block, int *blockLen, char *encodedBlock, int encodedBlockLen)
{
 unsigned char *bp;
 int      length;
 unsigned int c1,c2,c3,c4;
 register int j;

 if ((encodedBlockLen%4) || (encodedBlockLen<1)){
  *blockLen = 0;
  return;
 }

 length = 0;
 bp = (unsigned char *)encodedBlock;

 while(encodedBlockLen > 0){
   if (*bp&0x80 || (c1=asctobin[*bp])&0x80) return;
   ++bp;
   if (*bp&0x80 || (c2=asctobin[*bp])&0x80) return;
   if (*++bp == PAD){
     c3 = c4 = 0;
     length += 1;
     if (*++bp != PAD) return;
   }
   else if (*bp&0x80 || (c3=asctobin[*bp])&0x80)
     return;
   else{
     if (*++bp == PAD){
       c4 = 0;
       length += 2;
     }
     else if (*bp&0x80 || (c4=asctobin[*bp])&0x80)
       return;
     else
       length += 3;
   }
   ++bp;
   j = (c1 << 2) | (c2 >> 4);
   *block++=j;
   j = (c2 << 4) | (c3 >> 2);
   *block++=j;
   j = (c3 << 6) | c4;
   *block++=j;
   encodedBlockLen -= 4;
 }

 *blockLen = length;

}/* ASCIIdecodeBlock */
