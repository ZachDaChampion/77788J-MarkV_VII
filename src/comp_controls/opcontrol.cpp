#include "main.h"
#include "controller.h"
#include "io_control.h"
#include "recorder.h"
#include "macros.h"
#include "chassis.h"
#include "lift.h"
#include "claw.h"
#include "mogo.h"
#include "math.h"

void operatorControl() {

	// start the central controller
	startController();

	// start the timer
	startTimer();

	// start the macros listener
	macrosStartListener();

	while (true) {

		// only debug if not recording so console isn't cluttered
		if (!RECORDING_ENABLED) {
			print("CHASSIS\n");
			printf("\tLEFT\t%f\n", chassis_left);
			printf("\tRIGHT\t%f\n", chassis_right);
			printf("\tVELOCITY LEFT\t%f\n", motor_chassis_fl.getVelocity());
			printf("\tVELOCITY RIGHT\t%f\n", motor_chassis_fr.getVelocity());
			printf("\tORIENTATION\t%f\n", chassis_angle);
			printf("\tAP FILTER ANGLE\t%f\n", orientation_ap);
			print("\n");
			print("MOGO LIFTER\n");
			printf("\tLEFT\t%f\n", mogo_angle_l);
			printf("\tRIGHT\t%f\n", mogo_angle_r);
			printf("\tAVERAGE\t%f\n", mogo_angle);
			printf("\tTARGET\t%f\n", pid_mogo.getTarget());
			printf("\tPOWER\t%d\n", motor_mogo.getPower());
			print("LIFT\n");
			printf("\tANGLE\t%f\n", lift_angle);
			printf("\tHEIGHT\t%f\n", lift_height);
			print("CLAW\n");
			printf("\tANGLE\t%f\n", claw_angle);
			print("\n\n\n");
		}

		delay(1000);
	}
}
