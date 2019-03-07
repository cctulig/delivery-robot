#include "Auto.h"

//Initializes the chassis and arm objects as well as the autonomous state machine
void Auto::initialize () {

  chassis.initialize(10, 11, A9, A10, A11);
  arm.initialize(8, 7, A3, 27, 26);

  state = INITIAL_RAISE_ARM;
}

//Blue side autonomous state machine
void Auto::doAutoMode1 () {
  switch (state) {

    //Raises arm until it is at the level of the 3rd floor (45 deg)
    case INITIAL_RAISE_ARM:
      if (arm.raiseArmToDeg(45)) {
        state = LINE_FOLLOW_TO_FARADAY;
        Serial.println("LINE_FOLLOW_TO_FARADAY");
      }
      break;

    //Follows the white line until the right angle is detected at the left hand turn to Faraday
    case LINE_FOLLOW_TO_FARADAY:
      chassis.followLine();
      if (chassis.detectRightAngle(chassis.whiteLine, chassis.leftLine)) {
        state = TURN_TO_FARADAY;
        Serial.println("TURN_TO_FARADAY");
        chassis.motors(0, 0);
        chassis.resetEncoders();
      }
      break;

    //Turns left to Faraday until it has turned 90 deg (encoder value of 40)
    case TURN_TO_FARADAY:
      if (chassis.turn(1, 40)) {
        chassis.leftGoal = false;
        chassis.rightGoal = false;
        state = DRIVE_UP_TO_DORM;
        chassis.motors(0, 0);
      }
      break;

    /* If the custom circuit worked, arm would have been kept at ground level at the beginning and raised now.
      case RAISE_ARM:
      if (arm.raiseArmToFloor(3)) {
      state = DRIVE_UP_TO_DORM;
      }
      break;
    */

    //Line follows to Faraday until the button sensor at the end of the arm is triggered
    case DRIVE_UP_TO_DORM:
      chassis.followLine();
      if (arm.buttonPressed()) {
        state = DELIVER_PIZZA;
        chassis.motors(0, 0);
      }
      break;

    //Activates conveyor belt to deliver pizza into Faraday until the Autonomous program is terminated
    case DELIVER_PIZZA:
      arm.deliverPizza(140);
      break;
  }
}

//Red side autonomous state machine
//Essentially the same as the blue side but:
//the line sensors are looking for a 90 degree right turn
//the robot then turns 90 degrees to the right
void Auto::doAutoMode2 () {
  switch (state) {
    case INITIAL_RAISE_ARM:
      if (arm.raiseArmToDeg(45)) {
        state = LINE_FOLLOW_TO_FARADAY;
        Serial.println("LINE_FOLLOW_TO_FARADAY");
      }
      break;
    case LINE_FOLLOW_TO_FARADAY:
      chassis.followLine();
      if (chassis.detectRightAngle(chassis.whiteLine, chassis.rightLine)) {
        state = TURN_TO_FARADAY;
        Serial.println("TURN_TO_FARADAY");
        chassis.resetEncoders();
      }
      break;
    case TURN_TO_FARADAY:
      if (chassis.turn(1, 40)) {
        chassis.leftGoal = false;
        chassis.rightGoal = false;
        state = DRIVE_UP_TO_DORM;
      }
      break;
    /*
      case RAISE_ARM:
      if (arm.raiseArmToFloor(3)) {
      state = DRIVE_UP_TO_DORM;
      }
      break;
    */
    case DRIVE_UP_TO_DORM:
      chassis.followLine();
      if (arm.buttonPressed()) {
        state = DELIVER_PIZZA;
        chassis.motors(0, 0);
      }
      break;
    case DELIVER_PIZZA:
      arm.deliverPizza(140);
      break;
  }
}
