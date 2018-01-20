#include "lift.h"
#include "math.h"

// init variables to zero
float lift_angle = 0;
float lift_height = 0;
static float constant = 0;

// declare motor
Motor motor_lift;

// declare encoder
Encoder enc_lift;

// declare PID
Pid pid_lift;

// inits the lift lifter (motors, pid, etc.)
void liftInit() {

  // define lift encoder
  enc_lift = encoderInit(5, 6, false);

  // update sensors for accurate starting position
  liftUpdateSensors();

  // init motor
  motor_lift.init(MOTOR_LIFT, true, lift_angle);

  // init PID
  pid_lift.init(18.f, 20.f, 400.f, LIFT_HEIGHT_MIN, lift_angle);
  pid_lift.target_buffer = .2f;
  pid_lift.max_i = 70;
  pid_lift.i_factor = .05f;

  // calculate lift offset from ground when at 'neutral' position (all bars parallel)
  float rad = lift_angle * 0.0174533f; // convert to radians
  constant = -(-LIFT_HEIGHT_MIN + (sin(rad) * BEAM_LENGTH_BOTTOM) + (sin(rad) * BEAM_LENGTH_TOP)); // do the maths
}

// update all lift lifter motors
void liftUpdateMotors() {
  motor_lift.update(lift_angle, UPDATE_INTERVAL);
}

// update all lift lifter sensors
void liftUpdateSensors() {

  // angle (degrees) if the lift
  lift_angle = (.2f * encoderGet(enc_lift)) + LIFT_ANGLE_MIN;

  // height (inches) of the lift
  float rad = lift_angle * 0.0174533f; // convert to radians
  lift_height = (sin(rad) * BEAM_LENGTH_BOTTOM) + (sin(rad) * BEAM_LENGTH_TOP) + constant; // translate to height
}

// determines whether or not the lift has reached its target
bool liftAtTarget(bool vel) {
  return pid_lift.atTarget(vel, lift_angle, motor_lift.getVelocity());
}

// returns a recommended timeout for a PID
unsigned int liftGetTimeout(float target) {
  float delta = target - lift_height;
  if (delta > 0) return abs(delta * 69.f);
  else return abs(delta * 27.7f);
}

// sets the power of both lift motors
void liftSetPower(int power) {
  motor_lift.setPower(power, false);
}

void liftGoto(float height, bool wait, bool vel) {

  // setup timeout
  unsigned int timeout = 0;
  unsigned int max_timeout = liftGetTimeout(pid_lift.getTarget());

  // update PID target
  pid_lift.setTarget(height);

  // wait if applicable
  if (wait) {
    while (!liftAtTarget(vel) && timeout < max_timeout) {
      delay(1);
      timeout += 1;
    }
  }
}
