#pragma once

#include "Servo.h"
#include "Encoder.h"

class Chassis {
  private:
    Servo leftMotor;
    Servo rightMotor;

    int colorThreshold;
    enum lineStates {ON_LINE, LEFT_OF_LINE, RIGHT_OF_LINE} state;
    int leftEncPos = 0;
    int rightEncPos = 0;
    int targetLeftEncPos;
    int targetRightEncPos;

  public:
    int leftLine;
    int middleLine;
    int rightLine;

    int whiteLine;
    int colorLine;
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
    void turn(int dir, int deg);
    void driveWithEncoders(int driveSpeed, int dir);
    void setEncoders(int distance, int dirL, int dirR);
    bool checkEncoders(int dirL, int dirR);
};
