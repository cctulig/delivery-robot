#include "Arm.h"
#include "Arduino.h"

void Arm::initialize(unsigned armMotorPin, unsigned beltMotorPin, unsigned potPin, unsigned ultrasonicPin, unsigned buttonPin) {
  armMotor.attach(armMotorPin, 1000, 2000);
  beltMotor.attach(beltMotorPin, 1000, 2000);

  pot = potPin;
  ultrasonic = ultrasonicPin;
  button = buttonPin;

  pinMode(pot, INPUT_PULLUP);
  pinMode(ultrasonic, INPUT);
  pinMode(buttonPin, INPUT);

  Kp = 5;
  currentFloor = 0;
  lastPotPos = analogRead(pot) * 360 / 1023;
  
}

bool Arm::raiseArmToDeg(int deg){
  int armPos = analogRead(pot) - 65;
  int desiredPos = deg * 120 / 100;
  int error =  constrain((desiredPos - armPos + 20) * Kp, -90, 90);

  armMotor.write(90 + error);

  return armPos > desiredPos;
}

bool Arm::raiseArmToFloor(int targetFloor) {
  int floorDifference = targetFloor - currentFloor;
  int error = constrain(30 * floorDifference, -90, 90);
  
  armMotor.write(90 + error);

  return currentFloor == targetFloor;
}

void Arm::countFloor() {
  int currentPotPos = analogRead(pot) * 360 / 1023;
  int potDifference = abs(currentPotPos - lastPotPos);
  
  if(potDifference > 5) {
    if (checkForFloor()) {
      currentFloor ++;
      lastPotPos = analogRead(pot) * 360 / 1023;
    }
  }
}

bool Arm::checkForFloor() {
  int sum = 0;
  for(int i = 0; i < 5; i++) {
    if (digitalRead(ultrasonic) == 1) {
      sum ++;
    }
  }
  
  float avg = sum / 5;

  return avg >= 0.8f;
}

void Arm::deliverPizza(int motorSpeed) {
  beltMotor.write(180);
}

bool Arm::buttonPressed() {
  return digitalRead(button) == 1;
}
