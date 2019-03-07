#pragma once

#include "Servo.h"

//Arm class contains all functions, sensors, motors, and variables relating to the arm
class Arm {
  private:

    int pot;
    int ultrasonic;
    int button;
    int Kp;
    int currentFloor;
    int lastPotPos;

  public:
    Servo armMotor;
    Servo beltMotor;
    void initialize (unsigned armMotorPin, unsigned beltMotorPin, unsigned potPin, unsigned ultrasonicPin, unsigned buttonPin);
    bool raiseArmToDeg (int deg);
    bool raiseArmToFloor (int targetFloor);
    void countFloor ();
    bool checkForFloor();
    void deliverPizza (int motorSpeed);
    bool buttonPressed ();

};
