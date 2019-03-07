//#pragma once

#include "Servo.h"

class Arm {
  private:
    Servo armMotor;
    Servo beltMotor;

    int pot;
    int ultrasonic;
    int button;
    int Kp;
    int currentFloor;
    int lastPotPos;

  public:
    void initialize (unsigned armMotorPin, unsigned beltMotorPin, unsigned potPin, unsigned ultrasonicPin, unsigned buttonPin);
    bool raiseArmToDeg (int deg);
    bool raiseArmToFloor (int targetFloor);
    void countFloor ();
    bool checkForFloor();
    void deliverPizza (int motorSpeed);
    bool buttonPressed ();
    
};
