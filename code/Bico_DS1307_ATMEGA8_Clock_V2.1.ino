#include <Wire.h>
#include "DS1307.h"
#include "Bico_Led7Seg.h"
#include <EEPROM.h>

#define PWM_PIN 9

#define PRESS_DATE_TIME_BUTTON ((analogRead(A3) >= 380) && (analogRead(A3) < 580)) // 470
#define PRESS_JUMP_BUTTON ((analogRead(A3) >= 680) && (analogRead(A3) < 900)) // 723
#define PRESS_PLUS_BUTTON ((analogRead(A3) >= 100) && (analogRead(A3) < 350)) // 238
#define PRESS_PLUS_AND_DATE_TIME_BUTTON ((analogRead(A3) >= 580) && (analogRead(A3) < 680))
#define PRESS_NO_BUTTON (analogRead(A3) <= 20)

#define INDICATE_TIME (indicate_time_date == 0)
#define INDICATE_DATE (indicate_time_date == 1)

#define NUM_OF_LED_EFFECT 6
#define TURN_OFF_LED 0
#define TURN_ON_LED 1
#define ONE_SENCOND_LIGHT_UP 2
#define ONE_SECOND_FADE 3
#define ONE_SECOND_BLINK 4
#define PARTY_MODE 5

DS1307 clock;
RTCDateTime dt;

Bico_Led7Seg my_leg7seg(COMMON_ANODE, 8, 2000, 1);
uint8_t digit[8] = {0, 1, 2, 3, 4, 5, 6, 7};
uint8_t code[8] = {A2, A1, A0, 13, 12, 11, 10, 8};

uint8_t point_position = 0; //use to indicate the number which need to be change
uint8_t indicate_time_date = 0; //0: indicate time, 1: indicate date
uint8_t show_led = TURN_ON_LED;
uint8_t pwm_value = 50;

void setup() {
  // put your setup code here, to run once:
  clock.begin();
//  if(PRESS_BUTTON_4)
//  {
//    clock.setDateTime(__DATE__, __TIME__);
//  }
  if (!clock.isReady())
  {
    // Set sketch compiling time
    clock.setDateTime(__DATE__, __TIME__);
  }
  my_leg7seg.setup(digit, code);

  pinMode(PWM_PIN, OUTPUT);

  // Show bicotruc ----------------------------------
  my_leg7seg.setSymbol(0, B10100111); // o
  my_leg7seg.setSymbol(1, B11100011); // c
  my_leg7seg.setSymbol(2, B10101111); // i
  my_leg7seg.setSymbol(3, B10000111); // b
  my_leg7seg.setSymbol(4, B10100011); // c
  my_leg7seg.setSymbol(5, B10100111); // u
  my_leg7seg.setSymbol(6, B11111011); // r
  my_leg7seg.setSymbol(7, B10000011); // t

  for(uint8_t i = 0; i<100; i++)
  {
    my_leg7seg.show(); 
  }
  // Show bicotruc ----------------------------------

  show_led = EEPROM.read(0);
}

void loop() {
  // put your main code here, to run repeatedly:
  dt = clock.getDateTime();

  if(PRESS_NO_BUTTON)
  {
    if(INDICATE_TIME)
    {
      my_leg7seg.setDigit(7, dt.hour/10, false);
      my_leg7seg.setDigit(6, dt.hour%10, (point_position==3));
    
      my_leg7seg.setSymbol(5, '-');
      
      my_leg7seg.setDigit(4, dt.minute/10, false);
      my_leg7seg.setDigit(3, dt.minute%10, (point_position==2));
      
      my_leg7seg.setSymbol(2, '-');
      
      my_leg7seg.setDigit(1, dt.second/10, false);
      my_leg7seg.setDigit(0, dt.second%10, (point_position==1));
    }


    if(INDICATE_DATE)
    {
      my_leg7seg.setDigit(0, (dt.year-2000)%10, (point_position==1));
      my_leg7seg.setDigit(1, (dt.year-2000)/10, false);
    
      my_leg7seg.setSymbol(2, '-');
      
      my_leg7seg.setDigit(3, dt.month%10, (point_position==2));
      my_leg7seg.setDigit(4, dt.month/10, false);
      
      my_leg7seg.setSymbol(5, '-');
      
      my_leg7seg.setDigit(6, dt.day%10, (point_position==3));
      my_leg7seg.setDigit(7, dt.day/10, false);
    }
  }

  if(PRESS_DATE_TIME_BUTTON)
  {
    indicate_time_date = (indicate_time_date+1)%2;
    delay(200);
  }

  if(PRESS_JUMP_BUTTON)
  {
    point_position = (point_position+1)%4;
    delay(200);
  }

  if(PRESS_PLUS_BUTTON)
  {
    switch(point_position)
    {
      case 0:
      {
        pwm_value += 20;
        break;
      }
      
      case 1:
      {
        if(INDICATE_TIME)
        {
          clock.setDateTime(dt.year, dt.month, dt.day, dt.hour, dt.minute, (dt.second < 59)? (dt.second + 1):0);
        }
        if(INDICATE_DATE)
        {
          clock.setDateTime(((dt.year - 2000) < 99)? (dt.year + 1):2000, dt.month, dt.day, dt.hour, dt.minute, dt.second);
        }
        break;
      }

      case 2:
      {
        if(INDICATE_TIME)
        {
          clock.setDateTime(dt.year, dt.month, dt.day, dt.hour, (dt.minute < 59)? (dt.minute + 1):0, dt.second);
        }
        if(INDICATE_DATE)
        {
          clock.setDateTime(dt.year, (dt.month < 12)? (dt.month + 1):1, dt.day, dt.hour, dt.minute, dt.second);
        }
        break;
      }

      case 3:
      {
        if(INDICATE_TIME)
        {
          clock.setDateTime(dt.year, dt.month, dt.day, (dt.hour < 23)? (dt.hour + 1):0, dt.minute, dt.second);
        }
        if(INDICATE_DATE)
        {
          clock.setDateTime(dt.year, dt.month, (dt.day < 31)? (dt.day + 1):1, dt.hour, dt.minute, dt.second);
        }
        break;
      }
    }
    delay(200);
  }
//--------------------------------------------------------------------------------------------------------------------------------
  if(PRESS_PLUS_AND_DATE_TIME_BUTTON)
  {
    show_led = (show_led + 1) % NUM_OF_LED_EFFECT;
    EEPROM.write(0, show_led);
    delay(200);
  }

  
  switch(show_led)
  {
    case TURN_OFF_LED:
    {
      analogWrite(PWM_PIN, 0);
      break;
    }
    
    case TURN_ON_LED:
    {
      analogWrite(PWM_PIN, pwm_value);
      break;
    }
    
    case ONE_SENCOND_LIGHT_UP:
    {
      static uint8_t value = 0;
      static uint8_t previous_seconds = dt.second;
      if(dt.second != previous_seconds)
      {
        value = 0;
        previous_seconds = dt.second;
      }

      #define PWM_INCREASE_PERIOD 10
      static uint32_t last_millis_check = millis();
      if(millis() - last_millis_check > PWM_INCREASE_PERIOD)
      {
        value += 2;
        last_millis_check = millis();
      }

      analogWrite(PWM_PIN, value);
      break;
    }
    
    case ONE_SECOND_FADE:
    {
      static uint8_t value_1 = 4;
      static uint8_t fade_up_or_down = 1;
      static uint8_t previous_seconds = dt.second;
      if(dt.second != previous_seconds)
      {
        fade_up_or_down = !fade_up_or_down;
        previous_seconds = dt.second;
      }

      #define PWM_INCREASE_PERIOD 10
      static uint32_t last_millis_check_1 = millis();
      if(millis() - last_millis_check_1 > PWM_INCREASE_PERIOD)
      {
        if(fade_up_or_down)
        {
          value_1 = ((value_1 + 2) <= 210)? (value_1 + 2):210;
        }
        else
        {
          value_1 = ((value_1 - 2) >= 10)? (value_1 - 2):10;
        }
        last_millis_check_1 = millis();
      }

      analogWrite(PWM_PIN, value_1);
      break;
      break;
    }
    
    case ONE_SECOND_BLINK:
    {
      static uint8_t previous_seconds = dt.second;
      if(dt.second != previous_seconds)
      {
        digitalWrite(PWM_PIN, !digitalRead(PWM_PIN));
        previous_seconds = dt.second;
      }
      break;
    }
    
    case PARTY_MODE:
    {
      #define PARTY_MODE_BLINK_PERIOD 50
      static uint32_t last_millis_check_2 = millis();
      if(millis() - last_millis_check_2 > PARTY_MODE_BLINK_PERIOD)
      {
        digitalWrite(PWM_PIN, !digitalRead(PWM_PIN));
        last_millis_check_2 = millis();
      }
      break;
    }
  }
//----------------------------------------------------------------------------------------------------------------------------
  my_leg7seg.show();  
}
