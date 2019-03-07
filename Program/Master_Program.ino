#include "Servo.h"
#include "Arm.h"
#include "Chassis.h"

Chassis chassis;
Arm arm;

static enum stateChoices { INITIAL_RAISE_ARM, LINE_FOLLOW_TO_FARADAY, TURN_TO_FARADAY, RAISE_ARM, DRIVE_UP_TO_DORM, DELIVER_PIZZA, STOP_AUTO} state;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  chassis.initialize(10, 11, A9, A10, A11);
  arm.initialize(7, 8, A3, 4, 5);

  state = INITIAL_RAISE_ARM;
  delay(30);
}

void loop() {
  doAutoMode();
  
}

void doAutoMode () {
  switch (state) {
    case INITIAL_RAISE_ARM:
      if(arm.raiseArmToDeg(50)) {
        state = LINE_FOLLOW_TO_FARADAY;
        Serial.println("LINE_FOLLOW_TO_FARADAY");
      }
      break;
    case LINE_FOLLOW_TO_FARADAY:
      chassis.followLine();
      if (chassis.detectRightAngle(chassis.whiteLine, chassis.leftLine)) {
        state = TURN_TO_FARADAY;
        Serial.println("TURN_TO_FARADAY");
      }
      break;
    case TURN_TO_FARADAY:
      chassis.turn(1, 200);
      if (chassis.leftGoal && chassis.rightGoal) {
        chassis.leftGoal = false;
        chassis.rightGoal = false;
        state = RAISE_ARM;
      }
      break;
      /*
    case RAISE_ARM:
      if (arm.raiseArmToFloor(3)) {
        state = DELIVER_PIZZA;
      }
      break;
    case DRIVE_UP_TO_DORM:
      chassis.followLine();
      if (arm.buttonPressed()) {
      state = DELIVER_PIZZA;
      chassis.motors(0, 0);
      }
      break;
    case DELIVER_PIZZA:
        arm.deliverPizza(180);
        if (millis() > 20000) {
        state = STOP_AUTO;
        arm.deliverPizza(0);
        }
    case STOP_AUTO:
        break;
        */
      }
}
