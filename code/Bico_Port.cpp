#include "Bico_Port.h"

Bico_Port::Bico_Port()
{
  
}

void Bico_Port::setup(uint8_t* pin_array, uint8_t num_of_bit, uint8_t mode)
{
  if(num_of_bit <= MAX_NUM_OF_PORT_BIT)
  {
    _num_of_bit = num_of_bit;
  }
  else _num_of_bit = MAX_NUM_OF_PORT_BIT;
  
  _mode = mode;
  for(uint8_t i = 0; i < _num_of_bit; i++)
  {
    _pin_array[i] = pin_array[i];
    pinMode(_pin_array[i], _mode);
  }
}

void Bico_Port::writePort(long val)
{
  for(uint8_t i = 0; i < _num_of_bit; i++)
  {
    digitalWrite(_pin_array[i], getBit(val, i));
  }
}

long Bico_Port::readPort()
{
  long val = 0;
  for(uint8_t i = 0; i < _num_of_bit; i++)
  {
    val |= ((long)(digitalRead(_pin_array[i])) << i);
  }
  return val;
}





