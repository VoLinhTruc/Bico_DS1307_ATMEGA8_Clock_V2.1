#include "Bico_Led7Seg.h"

uint8_t Bico_Led7Seg::Bico_seg7_anode_code[16] = {0xc0, 0xf9, 0xa4, 0xb0, 0x99, 0x92, 0x82, 0xf8, 0x80, 0x90, 0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E};

//Class Bico_Led7Seg-------------------------------------------------------------------------
Bico_Led7Seg::Bico_Led7Seg()
{
  _led_type = COMMON_ANODE;
  _num_of_digit = 8;
  _on_delay = DEFAULT_ON_DELAY;
  _off_delay = DEFAULT_OFF_DELAY;
}

Bico_Led7Seg::Bico_Led7Seg(uint8_t led_type, uint8_t num_of_digit, int on_delay, int off_delay)
{
  _led_type = led_type;
  if(num_of_digit <= MAX_NUM_OF_LED)
  {
    _num_of_digit = num_of_digit;
  }
  else
  {
    _num_of_digit = MAX_NUM_OF_LED;
  }
  _on_delay = on_delay;
  _off_delay = off_delay;
}

//----------------------------------------------------------------------------------------------------------------

void Bico_Led7Seg::setup(uint8_t* digit, uint8_t* code)
{
  _digit_port.setup(digit, _num_of_digit, OUTPUT);
  _code_port.setup(code, 8, OUTPUT); //so 8 gom 7 doan tren led va dau .
  for(uint8_t i = 0; i < _num_of_digit; i++)
  {
    clear(i);
  }
  digitalWrite(digit[0], LOW);// coi bo duoc dong nay led co hien thi dung khong, chu luc nay khong hieu sao phai co dong code nay thi led moi hien thi dung
}

void Bico_Led7Seg::setDigit(uint8_t stand, uint8_t val, bool set_point)
{
  if(set_point)
  {
    if(_led_type == COMMON_ANODE)
    {
      _output_data[stand] = Bico_seg7_anode_code[val] & ANODE_FLOAT_POINT_SYMBOL;
    }
    if(_led_type == COMMON_CATHODE)
    {
      _output_data[stand] = ~(Bico_seg7_anode_code[val] & ANODE_FLOAT_POINT_SYMBOL);
    }
  }
  else
  {
    if(_led_type == COMMON_ANODE)
    {
      _output_data[stand] = Bico_seg7_anode_code[val];
    }
    if(_led_type == COMMON_CATHODE)
    {
      _output_data[stand] = ~(Bico_seg7_anode_code[val]);
    }
  }
}

void Bico_Led7Seg::setSymbol(uint8_t stand, char symbol)
{
    if(_led_type == COMMON_ANODE)
    {
      switch(symbol)
      {
        case '-': {_output_data[stand] = ANODE_MINUS_SYMBOL; break;}
        case '.': {_output_data[stand] &= ANODE_FLOAT_POINT_SYMBOL; break;}
        default: {_output_data[stand] = symbol; break;}
      }
    }
    else if(_led_type == COMMON_CATHODE)
    {
      switch(symbol)
      {
        case '-': {_output_data[stand] = ~ANODE_MINUS_SYMBOL; break;}
        case '.': {_output_data[stand] |= ANODE_FLOAT_POINT_SYMBOL; break;}
        default: {_output_data[stand] = symbol; break;}
      }
    }
}

void Bico_Led7Seg::clear(uint8_t stand)
{
    if(_led_type == COMMON_ANODE)
    {
      _output_data[stand] = ANODE_NULL_SYMBOL;
    }
    if(_led_type == COMMON_CATHODE)
    {
      _output_data[stand] = ~ANODE_NULL_SYMBOL;
    }
}

void Bico_Led7Seg::setNumber(signed long num)
{
  seperateDigit(num, _output_data, _num_of_digit);
  for(uint8_t i = 0; i < _num_of_digit; i++)
  {
    if((i >= getNumberOfDigit(num)) && (_output_data[i] == 0))
    {
      if(_led_type == COMMON_ANODE)
      {
        _output_data[i] = ANODE_NULL_SYMBOL;
      }
      if(_led_type == COMMON_CATHODE)
      {
        _output_data[i] = ~ANODE_NULL_SYMBOL;
      }
    }
    else
    {
      if(_led_type == COMMON_ANODE)
      {
        _output_data[i] = Bico_seg7_anode_code[_output_data[i]];
      }
      if(_led_type == COMMON_CATHODE)
      {
        _output_data[i] = ~Bico_seg7_anode_code[_output_data[i]];
      }
    }
  }
  if(num < 0)
    {
      setSymbol(getNumberOfDigit(num), '-');
    }
}

void Bico_Led7Seg::hide()
{
  if(_led_type == COMMON_ANODE)
  {
    _digit_port.writePort(0x00);
  }
  if(_led_type == COMMON_CATHODE)
  {
    _digit_port.writePort(0xFF);
  }
}

void Bico_Led7Seg::show()
{
  for(uint8_t i = 0; i < _num_of_digit; i++)
  { 
    _code_port.writePort(_output_data[i]);
    if(_led_type == COMMON_ANODE)
    {
      _digit_port.writePort((uint8_t)(1<<i));
    }
    if(_led_type == COMMON_CATHODE)
    {
      _digit_port.writePort((uint8_t)(~(1<<i)));
    }

    delayMicroseconds(_on_delay);
    hide();
    delayMicroseconds(_off_delay);
  }
}

void Bico_Led7Seg::setNumberAndShow(signed long val)
{
  setNumber(val);
  show();
}

void Bico_Led7Seg::brightness(int on_delay, int off_delay)
{
  _on_delay = on_delay;
  _off_delay = off_delay;
}

//--------------------------------------------------------------------------------------

void Bico_Led7Seg::setLedType(uint8_t led_type)
{
  if((led_type == COMMON_ANODE) || (led_type == COMMON_CATHODE))
  {
    _led_type = led_type;
  }
}

void Bico_Led7Seg::setNumOfLed(uint8_t num_of_led)
{
  if(num_of_led <= MAX_NUM_OF_LED)
  {
    _num_of_digit = num_of_led;
  }
}

void Bico_Led7Seg::setOnDelay(int on_delay)
{
  _on_delay = on_delay;
}

void Bico_Led7Seg::setOffDelay(int off_delay)
{
  _off_delay = off_delay;
}

uint8_t Bico_Led7Seg::getLedType()
{
  return _led_type;
}

uint8_t Bico_Led7Seg::getNumOfLed()
{
  return _num_of_digit;
}

int Bico_Led7Seg::getOnDelay()
{
  return _on_delay;
}

int Bico_Led7Seg::getOffDelay()
{
  return _off_delay;
}

//------------------------------------------------------------------------------------------------

////Class Bico_Leg7Seg_HC595-------------------------------------------------------------------------
//Bico_Leg7Seg_HC595::Bico_Leg7Seg_HC595()
//{
//  Bico_Led7Seg();
//}
//
//Bico_Leg7Seg_HC595::Bico_Leg7Seg_HC595(uint8_t led_type, uint8_t num_of_digit, int on_delay, int off_delay)
//{
//  Bico_Led7Seg(led_type, num_of_digit, on_delay, off_delay);
//}
//
//void Bico_Leg7Seg_HC595::setup(uint8_t* digit, uint8_t* code)
//{
//  _digit_port.setup(digit, _num_of_digit);
//  _code_port.setup(code, 8); //so 8 gom 7 doan tren led va dau .
//  for(uint8_t i = 0; i < _num_of_digit; i++)
//  {
//    clear(i);
//  }
//  //digitalWrite(digit[0], LOW);// coi bo duoc dong nay led co hien thi dung khong, chu luc nay khong hieu sao phai co dong code nay thi led moi hien thi dung
//}

