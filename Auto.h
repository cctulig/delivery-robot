#pragma once

#include "Servo.h"
#include "Arm.h"
#include "Chassis.h"

//Arm class contains the state machines for the 2 different autonomoue programs and holds the instantiated Arm and Chassis objects 
class Auto {
  public:
    enum stateChoices { INITIAL_RAISE_ARM, LINE_FOLLOW_TO_FARADAY, TURN_TO_FARADAY, RAISE_ARM, DRIVE_UP_TO_DORM, DELIVER_PIZZA, STOP_AUTO} state;

    void initialize ();
    void doAutoMode1 ();
    void doAutoMode2 ();

    Chassis chassis;
    Arm arm;
};
