#include "main.h"
#include "autos.h"
#include "controller.h"
#include "io_control.h"
#include "chassis.h"
#include "lift.h"
#include "chainbar.h"
#include "mogo.h"
#include "goliath.h"
#include "math.h"


void autoRunRight203ConeMogo() {

  float b = pid_chassis_theta.target_buffer;
  pid_chassis_theta.target_buffer = 2.5f;

  // move to mogo
  chassisMove(55.81f, 55.81f, true, true);

  // intake mogo
  mogoGoto(MOGO_ANGLE_EXTENDED, true, false);
  delay(1000);

  // stack (?) preload
  goliathDischarge(true);

  if (auto_cone) {

    // switch goliath back to intake mode
    goliath_timeout = -1;
    goliathIntake(false);

    // move backwards a bit
    chassisMove(-8.f, -8.f, true, false);

    // lower chainbar for second cone
    chainbarGoto(CHAINBAR_GRAB, true, false);

    // move forwards until holding second cone
    chassis_mode = CHASSIS_MODE_DIRECT;
    chassisSetPower(45);

    // wait for cone intake
    while (!goliath_holding && time < 7100) {
      delay(1);
    }

    chassisSetPower(30);

    // stack second cone
    chainbarGoto(CHAINBAR_STACK, true, false);
    goliathDischarge(true);
  }

  // make sure there's enough time for a third cone
  if (time < 7100) {

    // switch goliath back to intake mode
    goliath_timeout = -1;
    goliathIntake(false);

    // lower chainbar for third cone
    chainbarGoto(CHAINBAR_GRAB, true, false);

    // move forwards until holding third cone
    chassis_mode = CHASSIS_MODE_DIRECT;
    chassisSetPower(84);

    // wait for cone intake
    while (!goliath_holding && time < 6950) {
      delay(1);
    }

    // stop chassis
    chassisSetPower(0);
    chassisMove(0, 0, false, false);
    chassis_mode = CHASSIS_MODE_POSITION;

    // raise lift a bit
    pid_lift_enabled = true;
    liftGoto(LIFT_HEIGHT_MIN + 3.f, false, true);

    // raise chainbar slightly
    chainbarGoto(35.f, false, false);

    // wait for cone to be above ground
    while (lift_height < LIFT_HEIGHT_MIN + 1.f && chainbar_angle < 10.f && time < 7250) {
      delay(1);
    }

    // begin to move back to line
    chassisMove(4.f - (chassis_left / CHASSIS_SCALE_DISTANCE), 4.f - (chassis_right / CHASSIS_SCALE_DISTANCE), false, false);

    delay(275);

    // stack third cone
    chainbarGoto(CHAINBAR_STACK, true, true);
    goliathDischarge(true);
  }

  // shut down goliath and move chainbar way back
  goliathDisable();
  chainbarGoto(CHAINBAR_RETRACTED, true, false);

  // lower lift
  liftGoto(LIFT_HEIGHT_MIN, false, false);

  // move back to line
  while (!chassisAtTarget(false, CHASSIS_MODE_POSITION)) {
    delay(1);
  }

  // rotate parallel to 20 zone
  chassisRotate(-136.4f, true, true);

  // move to center of line
  chassisMove(24.9f, 24.4f, true, true);

  // rotate towards 20 zone
  chassisRotate(-90.f, true, true);

  // go forwards to 20
  chassisMove(45.f, 45.f, true, false);

  // drop mogo
  mogoGoto(MOGO_ANGLE_DROP, false, false);

  // move out of zones
  chassisMove(-30.f, -30.f, true, false);

  // get mogo ready for match
  mogoGoto(MOGO_ANGLE_GRAB, false, false);

  pid_chassis_theta.target_buffer = b;
}
