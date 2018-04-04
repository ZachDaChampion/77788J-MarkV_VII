#include "main.h"
#include "autos.h"
#include "controller.h"
#include "io_control.h"
#include "chassis.h"
#include "chainbar.h"
#include "lift.h"
#include "mogo.h"
#include "goliath.h"
#include "math.h"


void autoRunRightStationary5() {

  // raise lift and extend chainbar
  mogoGoto(MOGO_ANGLE_EXTENDED, false, false);
  liftGoto(LIFT_HEIGHT_STATIONARY, true, true);

  // move to stationary goal
  chassisMove(22.5f, 22.5f, true, true);
  delay(200);

  // lower lift slightly and open claw
  liftGoto(LIFT_HEIGHT_STATIONARY - 4.f, true, true);
  delay(300);

  // discharge and raise lift
  goliathDischarge(false);
  liftGoto(LIFT_HEIGHT_STATIONARY + 2.f, true, false);
  delay(100);

  // back up and retract chainbar
  chainbarGoto(CHAINBAR_RETRACTED, false, false);
  chassisMove(-21.391f, -21.391f, true, true);

  // lower lift, lower mogo, and rotate
  liftGoto(LIFT_HEIGHT_MIN, false, false);
  mogoGoto(MOGO_ANGLE_GRAB, false, false);
  chassisRotate(90.f, true, true);

  float b = pid_chassis_theta.target_buffer;
  pid_chassis_theta.target_buffer = 2.f;

  // move to mogo
  chassisMove(47.81f, 47.81f, true, true);

  // intake mogo
  mogoGoto(MOGO_ANGLE_EXTENDED, true, false);
  delay(750);

  if (auto_cone) {

  // begin to lower chainbar for cone
  chainbarGoto(CHAINBAR_GRAB, false, false);

    // switch goliath to intake mode
    goliath_timeout = -1;
    goliathIntake(false);

    // move backwards a bit
    chassisMove(-6.f, -6.f, true, false);

    // lower chainbar for cone
    chainbarGoto(CHAINBAR_GRAB, true, false);

    // move forwards until holding cone or timeout
    chassis_mode = CHASSIS_MODE_DIRECT;
    chassisSetPower(45);

    // wait for cone intake
    while (!goliath_holding && time < 9250) {
      delay(1);
    }

    if (time < 9250) {

      // stop chassis
      chassisSetPower(0);
      chassisMove(0, 0, false, false);
      chassis_mode = CHASSIS_MODE_POSITION;

      // stack cone
      chainbarGoto(CHAINBAR_STACK, true, false);
      delay(50);
      goliathDischarge(true);


      // move chainbar way back
      chainbarGoto(CHAINBAR_RETRACTED, false, false);
    }

    else {
      chainbarGoto(CHAINBAR_RETRACTED, false, false);
    }
  }

  // shut down goliath
  goliathDisable();

  // move back to line
  chassisMove(-49.78f, -49.78f, true, true);

  // rotate to 5 zone
  chassisRotate(-200.f, true, true);

  chassisMove(8.f, 8.f, true, false);

  // drop mogo
  mogoGoto(MOGO_ANGLE_GRAB, true, false);

  delay(min(500, 14675 - time));

  mogoGoto(MOGO_ANGLE_RETRACTED, false, false);

  // move out of zones
  chassisMove(-10.f, -10.f, true, false);

  mogoGoto(MOGO_ANGLE_GRAB, false, false);

  pid_chassis_theta.target_buffer = b;

}
