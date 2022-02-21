#include "Algebra_Math.h"

//--------------------------------------------------
signed long get_abs(signed long num)
{
  return ((num < 0)? (-num):num);
}
//--------------------------------------------------
signed long orderNumber(signed long num, unsigned char order)
{
  signed long result = 1;
  unsigned char i = 0;
  for(i = 0; i < order; i++)
    result *= num;
  return result;
}
//--------------------------------------------------
unsigned char getNumberOfDigit(signed long num)
{
  unsigned char num_of_digit = 0;
  num = get_abs(num);
  do
  {
    num_of_digit++;
  }
  while((num /= 10) >0 );

  return num_of_digit;
}
//--------------------------------------------------
unsigned char getDigitInNumber(signed long num, unsigned char stand) //stand - vi tri cua so can get
{
  return ((get_abs(num) / orderNumber(10, stand))%10);
}
//--------------------------------------------------
void seperateDigit(signed long num, unsigned char* storage, unsigned char storage_size)
{
  unsigned char i = 0;
  num = get_abs(num);
  for(i = 0; i < storage_size; i++)
  {
    storage[i] = getDigitInNumber(num, i);
  }
}
//--------------------------------------------------
