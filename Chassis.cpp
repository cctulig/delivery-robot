#include "Chassis.h"
#include "Arduino.h"

Encoder leftEnc (22, 23);
Encoder rightEnc (24, 25);

//Initializes all the necessary motors, sensors, and variables for a chassis
void Chassis::initialize(int leftPin, int rightPin, int leftLinePin, int middleLinePin, int rightLinePin) {
  
  //Drive motor related info
  leftMotor.attach(leftPin, 1000, 2000);
  rightMotor.attach(rightPin, 1000, 2000);
  currentSpeed = 50;

  //Encoder related info
  leftEnc.write(0);
  rightEnc.write(0);
  leftEncPos = 0;
  rightEncPos = 0;
  targetLeftEncPos = 0;
  targetRightEncPos = 0;
  leftGoal = false;
  rightGoal = false;

  //Line follor related info
  leftLine = leftLinePin;
  middleLine = middleLinePin;
  rightLine = rightLinePin;

  pinMode(leftLine, INPUT);
  pinMode(middleLine, INPUT);
  pinMode(rightLine, INPUT);

  //Code assumes the middle line sensors starts on the white line and the left and right line sensors start on the board color
  state = ON_LINE;
  boardColor = (zeroSensor (leftLine, 10) + zeroSensor (rightLine, 10)) / 2;  //board color averaged between the left and right sensor averages
  whiteLine = zeroSensor (middleLine, 10);  //white line averaged
  colorThreshold = (boardColor - whiteLine) / 6;  //color threshold to be added onto the white line to catch slightly higher values

  Serial.print("white line: ");
  Serial.print(whiteLine);
  Serial.print(", board color: ");
  Serial.println(boardColor);
}

//Zeros a sensor value by averaging it a given amount of times
int Chassis::zeroSensor (int sensorPin, int times) {
  int sum = 0;
  for (int i = 0; i < times; i++) {
    sum += analogRead(sensorPin);
  }
  return sum / times;
}

//Line follow function.
//If the middle line detects white, robot goes straight
//If the right line detects white, the robot turns right until the middle line detects white
//If the left line detects white, the robot turns left until the middle line detects white
void Chassis::followLine() {
  int leftColor = analogRead(leftLine);
  int rightColor = analogRead(rightLine);
  int middleColor = analogRead(middleLine);

  if (middleColor <= whiteLine + colorThreshold) {
    state = ON_LINE;
    drive(currentSpeed, 1);
  }
  else if ((leftColor <= whiteLine + colorThreshold) || (state == LEFT_OF_LINE)) {
    state = LEFT_OF_LINE;
    motors(currentSpeed / 2, currentSpeed);
  }
  else if ((rightColor <= whiteLine + colorThreshold) || (state == RIGHT_OF_LINE)) {
    state = RIGHT_OF_LINE;
    motors(currentSpeed, currentSpeed / 2);
  }
  else {
    drive(currentSpeed, 1);
  }
}

//Detects a right angle of a given color and on a given side
//Returns true if a right angle is detected consectively 3 times in a row to eliminate faulty sensor readings
bool Chassis::detectRightAngle(int color, int sideSensor) {
  int sum = 0;
  for (int i = 0; i < 3; i++) {
    int sideColor = analogRead(sideSensor);
    int middleColor = analogRead(middleLine);
    if ((middleColor <= color + colorThreshold) && (sideColor <= color + colorThreshold)) {
      sum++;
    }
  }
  return (sum == 3);
}

//Drives the left and right motors.
//Allows the value of 0 to actually stop the robot
void Chassis::motors(int leftSpeed, int rightSpeed) {
  leftMotor.write(90 + leftSpeed);
  rightMotor.write(90 - rightSpeed);
}

//Drives the robot forwards or backwards at a given
void Chassis::drive(int driveSpeed, int dir) {
  motors(driveSpeed + dir, driveSpeed - dir);
}

//Turns the robot a given direction until both encoders read the given deg value
bool Chassis::turn(int dir, int deg) {

  leftEncPos = leftEnc.read();
  rightEncPos = rightEnc.read();

  Serial.print(leftEncPos);
  Serial.print(", ");
  Serial.print(rightEncPos);


  int turnAmount = deg;
  if (abs(leftEncPos) < turnAmount) {
    leftMotor.write(120 * dir);
  }
  else if (abs(leftEncPos) >= turnAmount) {
    leftGoal = true;
    leftMotor.write(90);
  }

  if (abs(rightEncPos) < turnAmount) {
    rightMotor.write(40 * dir);
  }
  else if (abs(rightEncPos) >= turnAmount) {
    rightGoal = true;
    rightMotor.write(90);
  }

  return (rightGoal && leftGoal);
}

//Resests the encoder positions back to 0
void Chassis::resetEncoders() {
  leftEnc.write(0);
  rightEnc.write(0);
}
