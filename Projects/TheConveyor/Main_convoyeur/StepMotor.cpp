#include "StepMotor.h"
#include <M5Stack.h>
#include <Wire.h>
const int rate = 125;

StepMotor::StepMotor(){
  
}

void StepMotor::Forward(int steps){

  char command[256];
  memset(command,0,sizeof(char)*256);
  // sprintf(command,"G1 X%dY%dZ%d F%d",steps,steps,steps,rate);
  sprintf(command,"G1 X%d F%d",steps,rate);

  SendGcode(command);
}

void StepMotor::SendGcode(char *command){
  Wire.beginTransmission(STEPMOTOR_ADDRESS);
  while ((*command) != 0) {
        Wire.write(*command);
        command++;
    }
  Wire.write(0x0d);
  Wire.write(0x0a);
  Wire.endTransmission();
}
