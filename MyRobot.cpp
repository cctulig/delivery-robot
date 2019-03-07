#include "MyRobot.h"
#include "Arduino.h"


/**
   Called when the start button is pressed and the robot control begins
*/
void MyRobot::robotStartup() {
  Serial.println("Here is where I start up my robot code");

  sidePin = 28;
  pinMode(sidePin, INPUT_PULLUP);

  //Determines what side the robot is on based on a digital jumper wire
  if (digitalRead(sidePin)) {
    side = 0;
  }
  else {
    side = 1;
  }

  autoMode.initialize();
}
/**
   Called by the controller between communication with the wireless controller
   during autonomous mode
   @param time the amount of time remaining
   @param dfw instance of the DFW controller
*/
void MyRobot::autonomous( long time) {
  Serial.print("\r\nAuto time remaining: ");
  Serial.print(time);
  if (side == 1) {
    autoMode.doAutoMode1();
  }
  else {
    autoMode.doAutoMode2();
  }
}
/**
   Called by the controller between communication with the wireless controller
   during teleop mode
   @param time the amount of time remaining
   @param dfw instance of the DFW controller
*/
void MyRobot::teleop( long time) {
  if (dfw->getCompetitionState() != powerup) {
    autoMode.chassis.rightMotor.write(180 - dfw->joystickrv());   //DFW.joystick will return 0-180 as an int into rightmotor.write
    autoMode.chassis.leftMotor.write(dfw->joysticklv());      //DFW.joystick will return 0-180 as an int into leftmotor.write
    if (dfw->r1()) {
      //Serial.println("r1");
      autoMode.arm.armMotor.write(180); //Moves arm up
    }
    else if (dfw->r2()) {
      //Serial.println("r2");
      autoMode.arm.armMotor.write(70);  //Moves arm down
    }
    else {
      autoMode.arm.armMotor.write(90);  //Holds arm
    }
  }
  if (autoMode.arm.buttonPressed()) {
    autoMode.arm.deliverPizza(140);     //Runs pizza dilvery conveyor belt (based off arm button, not remote control)
  }
  else {
    autoMode.arm.deliverPizza(90);      //Stops converyor belt
  }
}
/**
   Called at the end of control to reset the objects for the next start
*/
void MyRobot::robotShutdown(void) {
  Serial.println("Here is where I shut down my robot code");

}
