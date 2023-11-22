/*
  Adeept PWM (PCA9685) Module library V1.0
  This is a library for our Adeept 16-channel PWM & Servo driver
  These displays use I2C to communicate, 2 pins are required to  
  interface. For Arduino UNO Nano Mega2560 and etc, thats SCL -> Analog 5 pin(Digital SCL pin) , SDA -> Analog 4 pin(Digital SDA pin)

  Adeept invests time and resources providing this open source code, 
  please support Adeept and open-source hardware by purchasing 
  products from Adeept!

  2017 Copyright (c) Adeept Technology Inc.  All right reserved.
  Written by Adeept.  
  Our website: http://www.adeept.com
*/

#ifndef _ADEEPT_PWMPCA9685_H
#define _ADEEPT_PWMPCA9685_H

#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif


#define PCA9685_SUBADR1 0x2
#define PCA9685_SUBADR2 0x3
#define PCA9685_SUBADR3 0x4

#define PCA9685_MODE1 0x0
#define PCA9685_PRESCALE 0xFE

#define LED0_ON_L 0x6
#define LED0_ON_H 0x7
#define LED0_OFF_L 0x8
#define LED0_OFF_H 0x9

#define ALLLED_ON_L 0xFA
#define ALLLED_ON_H 0xFB
#define ALLLED_OFF_L 0xFC
#define ALLLED_OFF_H 0xFD


class Adeept_PWMPCA9685 {
 public:
  Adeept_PWMPCA9685(uint8_t addr = 0x40);
  void begin(void);
  void reset(void);
  void setPWMFreq(float freq);
  void setPWM(uint8_t num, uint16_t on, uint16_t off);
  void setPin(uint8_t num, uint16_t val, bool invert=false);

 private:
  uint8_t _i2caddr;

  uint8_t read8(uint8_t addr);
  void write8(uint8_t addr, uint8_t d);
};

#endif
