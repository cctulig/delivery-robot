#include "Arm.h"
#include "Arduino.h"

//Initializes all the necessary motors, sensors, and variables for an arm
void Arm::initialize(unsigned armMotorPin, unsigned beltMotorPin, unsigned potPin, unsigned ultrasonicPin, unsigned buttonPin) {
  
  //Motor related info
  armMotor.attach(armMotorPin, 1000, 2000);
  beltMotor.attach(beltMotorPin, 1000, 2000);

  //Sensor related info
  pot = potPin;
  ultrasonic = ultrasonicPin;
  button = buttonPin;

  pinMode(pot, INPUT);
  pinMode(ultrasonic, INPUT);
  pinMode(buttonPin, INPUT_PULLUP);

  //Variables
  Kp = 14;  //proportional control value
  currentFloor = 0;
  lastPotPos = analogRead(pot) * 360 / 1023;
  
}

//Raises an arm to a specified "degree"
//Arm degrees are from ground position (0) to fully raised (100)
//Returns true when the arm has reached the target position
bool Arm::raiseArmToDeg(int deg){
  //translates arm positions to 0 - 100 scale
  int armPos = analogRead(pot) - 65;
  int desiredPos = deg * 120 / 100;

  //Proportional Control Error
  int error =  constrain((desiredPos - armPos) * Kp, -90, 90);

  armMotor.write(90 + error);

  return armPos > desiredPos;
}

// *** The next couple functions are using the custom circuit if had worked. Note that because of this the code is untested. ***

//Raises arm to target floor
//Returns true if the arm has reach the target floor
bool Arm::raiseArmToFloor(int targetFloor) {

  //Porportional Control Error, based on floor difference
  int floorDifference = targetFloor - currentFloor;
  int error = constrain(30 * floorDifference, -90, 90);
  
  armMotor.write(90 + error);

  return currentFloor == targetFloor;
}

//Counts the floors as the arm passes them.
//If a floor is detected, another floor will not be looked for until the potentiometer has moved 5 deg
//Note: potentiometer was not adjusted to the 0 - 100 scale
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

//Reads the ultrasonic circuit to see if there is a floor
//Floor readings are averaged 5 at a time to eliminate minor misreadings in the sensor
//Returns true if the sensor read a floor at least 4 out of 5 times
bool Arm::checkForFloor() {
  int sum = 0;
  for(int i = 0; i < 5; i++) {
    if (digitalRead(ultrasonic) == 1) {
      sum ++;
    }
  }
  
  float avg = sum / 5;

  return avg >= 0.8f;
} // *** end of custom circuit functions ***

//Runs the converyor velt to deliver the pizza off the arm.
void Arm::deliverPizza(int motorSpeed) {
  beltMotor.write(motorSpeed);
}

//Checks if the button at the front of the arm is pressed
//Returns true if it has been
bool Arm::buttonPressed() {
  return (digitalRead(button) == 0);
}
