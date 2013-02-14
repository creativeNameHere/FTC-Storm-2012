#pragma config(Hubs,  S1, HTMotor,  HTMotor,  HTServo,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop)
#pragma config(Motor,  mtr_S1_C1_1,     backRight,     tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     frontLeft,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_1,     frontRight,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C2_2,     backLeft,      tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C4_1,     armMotor,      tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     rampMotor,     tmotorTetrix, openLoop)
#pragma config(Servo,  srvo_S1_C3_1,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C3_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_3,    servo3,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_4,    servo4,               tServoNone)
#pragma config(Servo,  srvo_S1_C3_5,    clawServo,            tServoStandard)
#pragma config(Servo,  srvo_S1_C3_6,    wristServo,           tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
//revised 1/15/13
#include "JoystickDriver.c"
#include "ProportionalControl.h"
#include "range.h"

#pragma platform(Tetrix)

/* Motor Roles:
 * - MotorA           moves ramp
 * - frontLeftMotor   moves robot, front left wheel
 * - backRightMotor   moves robot, back right wheel
 * - frontRightMotor  moves robot, front right wheel
 * - backLeftMotor    moves robot, back left wheel
 * - MotorH           moves arm up and down
 * - wristServo        moves hand
 */

#define ENABLE_RAMP
#define ENABLE_ARM
#define ENABLE_CLAW
#define COMPETITION

void omniDrive(float x, float y, float scale, float spin);

#ifdef ENABLE_RAMP
#define RAMP_MAX_POWER  50
#define RAMP_MIN_ERROR  15
#define RAMP_KP         2
//#define RAMP_MIN        0
//#define RAMP_MAX        720
//#define RAMP_FINE_RANGE 90
//ProportionalSettings rampSettings;
//rampSettings.kP = RAMP_KP;
//rampSettings.maxError = RAMP_MAX_ERROR;
//rampSettings.minOutput = - RAMP_MAX_POWER;
//rampSettings.maxOutput = RAMP_MAX_POWER;
#endif

ProportionalSettings armSettings;

#define ARM_MAX_POWER  75
#define ARM_MIN        0
#define ARM_MAX        2211
#define ARM_KP         (2.0/(ARM_MAX - ARM_MIN))
#define ARM_MAX_ERROR  ((ARM_MAX - ARM_MIN)/20.0)
#define ARM_FINE_RANGE ((ARM_MAX - ARM_MIN)/5.0)

#define DRIVE_NORMAL_SCALE 1
#define DRIVE_SLOW_SCALE   0.5

#define WRIST_SERVO_SPEED 130

task main() {
#ifdef COMPETITION
 waitForStart(); //do not remove!!!!!!
#endif
	int ArmTarget = 0;
    armSettings.kP        = ARM_KP;
    armSettings.maxError  = ARM_MAX_ERROR;
    armSettings.minOutput = -ARM_MAX_POWER;
    armSettings.maxOutput = ARM_MAX_POWER;


    float armRoughSetpoint = 0,
          armFineSetpoint  = 0;

    float wristServoVal = 0;
    long wristTime = 0;
    while(true) {
      getJoystickSettings( joystick );
      //  } else {
      // 		writeDebugStreamLine("button 1 is off")
      //}
				//writeDebugStreamLine("%d", joystick.joy2_x1);
        //see if btn 8 is depressed.
        //if so set a scale factor for all movement calculations
        //in omnidrive function
        float vx = scaleJoystickValue(-1,1,joystick.joy1_x1),
              vy = scaleJoystickValue(-1,1,joystick.joy1_y1),
              scale = joy1Btn(8) ? DRIVE_SLOW_SCALE :
                                   DRIVE_NORMAL_SCALE;
        omniDrive(vx,vy,
                  sqrt(vx*vx+vy*vy) * scale,
                  scaleJoystickValue(-50,50,joystick.joy1_x2));

				if( joy1Btn( 6 ) ){
        		motor[armMotor] = scaleJoystickValue(-50,50,joystick.joy2_y1);
      	} else {
      			motor[armMotor] = scaleJoystickValue(-75,75,joystick.joy2_y1);
      	}

#ifdef ENABLE_ARM
		#define ARM_FIRST_LEVEL 2000
		#define ARM_SECOND_LEVEL 1460
		#define ARM_FULL_LEVEL 2370
		#define ARM_SPEED_CONST 40
		if(joy2btn(1)){
			ArmTarget = 0;
		} else if (joy2btn(2)) {
			ArmTarget = ARM_FIRST_LEVEL;
		} else if (joy2btn(3)){
			ArmTarget = ARM_SECOND_LEVEL;
		} else if (joy2btn(4)){
			ArmTarget = ARM_FULL_LEVEL;
		} else if (joystick.joy2_y1 != 0) {
			ArmTarget = -1;
		} else {
			ArmTarget = ArmTarget;
		}
		if(ArmTarget == -1){
			motor[armMotor] = (50/128) * joystick.joy2_y1;
		} else {
		if (abs(nmotorencoder[armMotor] - ArmTarget) <= 5){
				motor[armMotor] = 0;
			} else if (nmotorencoder[armMotor] - ArmTarget > 5) {
				motor[armMotor] = ARM_SPEED_CONST;
			} else if (nmotorencoder[armMotor] - ArmTarget < -5){
				motor[armMotor] = -ARM_SPEED_CONST;
			} else {
				motor[armMotor] = motor[armMotor];
			}
		}
			// if (joy1btn(6)){
				// motor[armMotor] = scaleJoystickValue(-50,50,joystick.joy2_y1);
			// } else {
				// motor[armMotor] = scaleJoystickValue(-75,75,joystick.joy2_y1);
			// }


        // if(joy2Btn(5)) {
            // armRoughSetpoint = scaleJoystickValue(ARM_MIN,
                                                  // ARM_MAX,
                                                  // joystick.joy2_y1);
            // armFineSetpoint = 0;
        // }
        // if(joy2Btn(6)) {
            // armFineSetpoint = scaleJoystickValue(-ARM_FINE_RANGE,
                                                 // ARM_FINE_RANGE,
                                                 // joystick.joy2_y2);
        // } else {
            // armRoughSetpoint += armFineSetpoint;
            // armFineSetpoint = 0;
        // }

        // proportionalControl(armMotor,
                            // armSettings,
                            // clamp(armRoughSetpoint+armFineSetpoint,
                                  // ARM_MIN,
                                  // ARM_MAX));

#endif

#ifdef ENABLE_CLAW

		    #define wristServoValVal 45
            if(!(joy2Btn(4) || joy2Btn(2))) {
                wristTime = 0;
            } else {
                wristTime += time1[T1];
            }
            ClearTimer(T1);
			if (joystick.joy2_TopHat == 0){
				wristServoVal = 180;
   			} else if (joystick.joy2_TopHat == 4){
				wristServoVal = 0;
			} else if (wristTime > 1000 / WRIST_SERVO_SPEED ){
				if (joy2Btn(4)){
					wristServoVal += 1;
				} else if (joy2Btn(2)){
					wristServoVal -= 1;
				}
				wristTime -= 1000/WRIST_SERVO_SPEED;
			}
            if (wristServoVal >= 180){
            	wristServoVal = 180;
      	    }else if (wristServoVal <= 20){
        	    wristServoVal = 0;
          	}
        	servo[wristServo] = wristServoVal;

            if(joy2Btn(1)) {
            	servo[clawServo] = 230;
        	} else if(joy2Btn(3)) {
            	servo[clawServo] = 130;
        	}
#endif


#ifdef ENABLE_RAMP

#define RAMP_SPEED 50

      if( joy2Btn( 10 ) ) {
					motor[rampMotor] = RAMP_SPEED;
			} else if( joy2Btn( 9 ) ) {

					motor[rampMotor] = - RAMP_SPEED;
			} else {
					motor[rampMotor] = 0;
				}
        //if(joy1Btn(10) && joy1Btn(9) && joy2Btn(10) && joy2Btn(9)) {
        //    break;
        //}

#endif
} // while
} //task main

void omniDrive(float x, float y, float scale, float spin) {
    int upRightSpeed = (x + y)  / sqrt(2);
    int upLeftSpeed  = (-x + y) / sqrt(2);

    int frontRightSpeed = clamp(upLeftSpeed  - spin,-100,100) * scale,
        frontLeftSpeed  = clamp(upRightSpeed + spin,-100,100) * scale,
        backRightSpeed  = clamp(upRightSpeed - spin,-100,100) * scale,
        backLeftSpeed   = clamp(upLeftSpeed  + spin,-100,100) * scale;

    motor[frontLeft]  = frontLeftSpeed;
    motor[backRight]  = backRightSpeed;
    motor[backLeft]   = backLeftSpeed;
    motor[frontRight] = frontRightSpeed;

    //writeDebugStreamLine("frontRight:%d,frontLeft:%d,backRight:%d,backLeft:%d", frontRightSpeed,frontLeftSpeed,backRightSpeed,backLeftSpeed);
}

#ifdef ENABLE_RAMP

	//void haltMotors(){
	//		writeDebugStreamLine("Halting motors...");
	//		motor[motorA] = 0;
	//		motor[frontLeft] = 0;
	//		motor[frontRight] = 0;
	//		motor[backLeft] = 0;
	//		writeDebugStreamLine("...");
	//		motor[backRight] = 0;
	//		motor[motorH] = 0;
	//		motor[rampMotor] = 0;
	//		writeDebugStreamLine("Complete!");
	//}

#endif
