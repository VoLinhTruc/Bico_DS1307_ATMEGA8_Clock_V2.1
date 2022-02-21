#ifndef BICO_PORT_H_
#define BICO_PORT_H_

#include <Arduino.h>
#include "Logic_Math.h"

#define MAX_NUM_OF_PORT_BIT 16

class Bico_Port
{
  public:
    Bico_Port();

    void setup(uint8_t* pin_array, uint8_t num_of_bit, uint8_t mode);
    void writePort(long val);
    long readPort();
//    void setupPinMode(uint8_t bit, uint8_t mode);
//    void writePin(uint8_t bit, uint8_t val);
//    uint8_t readPin(uint8_t bit);
    

  private:
    uint8_t _pin_array[MAX_NUM_OF_PORT_BIT];
    uint8_t _num_of_bit;
    uint8_t _mode;
};

#endif
