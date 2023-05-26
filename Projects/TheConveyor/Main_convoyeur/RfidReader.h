#ifndef RfidReader_h
#define RfidReader_h
#include <Arduino.h>
#include <M5Stack.h>

class RfidReader
{
public:
  RfidReader();
  void setupRfid();
  String readRfcTag();
};
#endif
