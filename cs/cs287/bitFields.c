#include <stdio.h>
#include "register.h"


void main()
{
 aRegister reg;
 int x;
 
 for(x=4;x>-4;x--){
  reg.decValue=x;
  writeReg(reg);
 }
 reg.decValue=10;
 writeReg(reg);
 shiftLeft(&reg);
 writeReg(reg);
 shiftRight(&reg);
 writeReg(reg);
 shiftRight(&reg);
 writeReg(reg);
 
}/* main */

writeReg(aRegister reg)
{
 int x;
 
 printf("%8d - ",reg.decValue);
 for(x=0;x<32;x++){
  printf("%d",reg.binValue.bit0);
  shiftRight(&reg);
 }
 printf("\n");

}/* writeReg */

shiftLeft(aRegister *reg)
{
 reg->binValue.bit31 = reg->binValue.bit30;
 reg->binValue.bit30 = reg->binValue.bit29;
 reg->binValue.bit29 = reg->binValue.bit28;
 reg->binValue.bit28 = reg->binValue.bit27;
 reg->binValue.bit27 = reg->binValue.bit26;
 reg->binValue.bit26 = reg->binValue.bit25;
 reg->binValue.bit25 = reg->binValue.bit24;
 reg->binValue.bit24 = reg->binValue.bit23;
 reg->binValue.bit23 = reg->binValue.bit22;
 reg->binValue.bit22 = reg->binValue.bit21;
 reg->binValue.bit21 = reg->binValue.bit20;
 reg->binValue.bit20 = reg->binValue.bit19;
 reg->binValue.bit19 = reg->binValue.bit18;
 reg->binValue.bit18 = reg->binValue.bit17;
 reg->binValue.bit17 = reg->binValue.bit16;
 reg->binValue.bit16 = reg->binValue.bit15;
 reg->binValue.bit15 = reg->binValue.bit14;
 reg->binValue.bit14 = reg->binValue.bit13;
 reg->binValue.bit13 = reg->binValue.bit12;
 reg->binValue.bit12 = reg->binValue.bit11;
 reg->binValue.bit11 = reg->binValue.bit10;
 reg->binValue.bit10 = reg->binValue.bit9;
 reg->binValue.bit9  = reg->binValue.bit8;
 reg->binValue.bit8  = reg->binValue.bit7;
 reg->binValue.bit7  = reg->binValue.bit6;
 reg->binValue.bit6  = reg->binValue.bit5;
 reg->binValue.bit5  = reg->binValue.bit4;
 reg->binValue.bit4  = reg->binValue.bit3;
 reg->binValue.bit3  = reg->binValue.bit2;
 reg->binValue.bit2  = reg->binValue.bit1;
 reg->binValue.bit1  = reg->binValue.bit0;
 reg->binValue.bit0  = 0;

}/* shiftLeft */

shiftRight(aRegister *reg)
{
 reg->binValue.bit0  = reg->binValue.bit1;
 reg->binValue.bit1  = reg->binValue.bit2;
 reg->binValue.bit2  = reg->binValue.bit3;
 reg->binValue.bit3  = reg->binValue.bit4;
 reg->binValue.bit4  = reg->binValue.bit5;
 reg->binValue.bit5  = reg->binValue.bit6;
 reg->binValue.bit6  = reg->binValue.bit7;
 reg->binValue.bit7  = reg->binValue.bit8;
 reg->binValue.bit8  = reg->binValue.bit9;
 reg->binValue.bit9  = reg->binValue.bit10;
 reg->binValue.bit10 = reg->binValue.bit11;
 reg->binValue.bit11 = reg->binValue.bit12;
 reg->binValue.bit12 = reg->binValue.bit13;
 reg->binValue.bit13 = reg->binValue.bit14;
 reg->binValue.bit14 = reg->binValue.bit15;
 reg->binValue.bit15 = reg->binValue.bit16;
 reg->binValue.bit16 = reg->binValue.bit17;
 reg->binValue.bit17 = reg->binValue.bit18;
 reg->binValue.bit18 = reg->binValue.bit19;
 reg->binValue.bit19 = reg->binValue.bit20;
 reg->binValue.bit20 = reg->binValue.bit21;
 reg->binValue.bit21 = reg->binValue.bit22;
 reg->binValue.bit22 = reg->binValue.bit23;
 reg->binValue.bit23 = reg->binValue.bit24;
 reg->binValue.bit24 = reg->binValue.bit25;
 reg->binValue.bit25 = reg->binValue.bit26;
 reg->binValue.bit26 = reg->binValue.bit27;
 reg->binValue.bit27 = reg->binValue.bit28;
 reg->binValue.bit28 = reg->binValue.bit29;
 reg->binValue.bit29 = reg->binValue.bit30;
 reg->binValue.bit30 = reg->binValue.bit31;
 reg->binValue.bit31  = 0;

}/* shiftRight */
