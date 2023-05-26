#ifndef StepMotor_h
#define StepMotor_h

#include <Arduino.h>
#include <M5Stack.h>
#include <Wire.h>

#define STEPMOTOR_ADDRESS 0x70

class StepMotor{
public:
  StepMotor();
  void Forward(int step);
  void SendGcode(char *command);
};

#endif
