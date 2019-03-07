#include "Chassis.h"
#include "Arduino.h"

Encoder leftEnc (22, 23);
Encoder rightEnc (24, 25);

void Chassis::initialize(int leftPin, int rightPin, int leftLinePin, int middleLinePin, int rightLinePin) {
  leftMotor.attach(leftPin, 1000, 2000);
  rightMotor.attach(rightPin, 1000, 2000);

  leftLine = leftLinePin;
  middleLine = middleLinePin;
  rightLine = rightLinePin;

  pinMode(leftLine, INPUT);
  pinMode(middleLine, INPUT);
  pinMode(rightLine, INPUT);

  leftEnc.write(0);
  rightEnc.write(0);
  leftEncPos = 0;
  rightEncPos = 0;
  boardColor = (zeroSensor (leftLine, 10) + zeroSensor (rightLine, 10)) / 2;
  whiteLine = zeroSensor (middleLine, 10);
  //colorLine = (boardColor * 123 + whiteLine * 456) / 2;
  colorThreshold = (boardColor - whiteLine) / 2;
  state = ON_LINE;
  currentSpeed = 30;
  targetLeftEncPos = 0;
  targetRightEncPos = 0;
  leftGoal = false;
  rightGoal = false;

  Serial.print("white line: ");
  Serial.print(whiteLine);
  Serial.print(", board color: ");
  Serial.println(boardColor);
}

int Chassis::zeroSensor (int sensorPin, int times) {
  int sum = 0;
  for (int i = 0; i < times; i++) {
    sum += analogRead(sensorPin);
  }
  return sum / times;
}

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

void Chassis::motors(int leftSpeed, int rightSpeed) {
  leftMotor.write(90 + leftSpeed);
  rightMotor.write(90 - rightSpeed);
}

void Chassis::drive(int driveSpeed, int dir) {
  motors(driveSpeed + dir, driveSpeed - dir);
}

void Chassis::turn(int dir, int deg) {

  leftEncPos = leftEnc.read();
  rightEncPos = rightEnc.read();

  int turnAmount = deg * 240 / 360; //240 = encoder value for a full rotation of the robot
  if (abs(leftEncPos) < turnAmount) {
    leftMotor.write(45 * dir);
  }
  else if (abs(leftEncPos) >= turnAmount) {
    leftGoal = true;
    leftMotor.write(90);
  }

  if (abs(rightEncPos) < turnAmount) {
    rightMotor.write(45 * dir);
  }
  else if (abs(rightEncPos) >= turnAmount) {
    rightGoal = true;
    rightMotor.write(90);
  }
}
