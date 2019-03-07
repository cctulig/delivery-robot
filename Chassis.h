#pragma once

#include "Servo.h"
#include "Encoder.h"

//Chassis class contains all functions, sensors, motors, and variables relating to the chassis
class Chassis {
  private:

    int colorThreshold;
    enum lineStates {ON_LINE, LEFT_OF_LINE, RIGHT_OF_LINE} state;
    int leftEncPos = 0;
    int rightEncPos = 0;
    int targetLeftEncPos;
    int targetRightEncPos;

  public:
  
    Servo leftMotor;
    Servo rightMotor;

    int leftLine;
    int middleLine;
    int rightLine;

    int whiteLine;
    int boardColor;

    int currentSpeed;

    bool leftGoal;
    bool rightGoal;

    void initialize (int leftPin, int rightPin, int leftLinePin, int middleLinePin, int rightLinePin);
    int zeroSensor(int sensorPin, int times);
    void followLine ();
    bool detectRightAngle(int color, int sideSensor);
    void motors(int leftSpeed, int rightSpeed);
    void drive(int driveSpeed, int dir);
    bool turn(int dir, int deg);
    void resetEncoders();
};
