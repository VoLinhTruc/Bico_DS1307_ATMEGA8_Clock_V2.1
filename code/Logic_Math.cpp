#include "Logic_Math.h"

//--------------------------------------------------
unsigned char getBit(signed long num, unsigned char bit)
{
  return !!(num &= (1 << bit));
}
//--------------------------------------------------
signed long setBit(signed long num, unsigned char bit)
{
  return (num |= (1 << bit));
}
//--------------------------------------------------
signed long clrBit(signed long num, unsigned char bit)
{
  return (num &= ~(1 << bit));
}
//--------------------------------------------------


