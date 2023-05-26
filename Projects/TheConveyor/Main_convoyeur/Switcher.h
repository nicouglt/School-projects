#ifndef Switcher_h
#define Switcher_h

#include <Arduino.h>
#include <M5Stack.h>
#include <Wire.h>
#include <driver/rmt.h>     //Required driver for the RX/TX connection

// Servo address and Servo Module declaration
#define SWITCHER_ADDR 0x00
#define GOPLUS_ADDR 0x38

class Switcher{
public:
  Switcher();
  void SetPosition(char address);
  void rx_channel_init();
  void tx_channel_init();
  void WritePosition(uint8_t number, uint8_t angle);
};
#endif
