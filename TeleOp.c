#pragma config(Hubs,  S1, HTMotor,  HTServo,  HTMotor,  HTMotor)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Motor,  motorA,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorB,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  motorC,           ,             tmotorNXT, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_1,     motorD,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     motorE,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_1,     motorF,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C3_2,     motorG,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_1,     motorH,        tmotorTetrix, openLoop, encoder)
#pragma config(Motor,  mtr_S1_C4_2,     motorI,        tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S1_C2_1,    servo1,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_2,    servo2,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_3,    servo1,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_4,    servo2,               tServoStandard)
#pragma config(Servo,  srvo_S1_C2_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S1_C2_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
//TIM yu sucks lollipop
#include "JoystickDriver.c"

#pragma platform(Tetrix)

void omniDrive(int joyx, int joyy, float scale);
void latchrelease();

const float NORMAL_SCALE = 1.f;
const float SLOW_SCALE = 0.5f;
float scale = NORMAL_SCALE;

task main() {
while(true) {
        getJoystickSettings(joystick);
		//see if btn 8 is depressed, if so sets a scale factor for all movement calculations in omnidrive function
		if(joy1_btn(10)){
		//Release Latch
		latchrelease();
		}
		if(joy1_btn(8)){
		scale = SLOW_SCALE;
		}
        omniDrive(joystick.joy1_x1, joystick.joy1_y1, scale);
        if(joy1Btn(5)==1) {
            motor[motorH] = 50;
        }
        if (joy1Btn(7)==1) {
		motor[motorH]=-50;
	}

        switch(joystick.joy1_TopHat) {
        case 0:
        case 1:
        case 7:
            servo[servo1] = 40;
            break;

        case 5:
        case 4:
        case 3:
            servo[servo1] = -40;
            break;

        default:
            servo[servo1] = 0;
        }
    }
}

//int scale: multiplies by the scale factor to receive new speed
void omniDrive(int joyx, int joyy, float scale) {
    int upRightSpeed = (((100.0/128)*(float)joyx +
                        (100.0/128)*(float)joyy)
                       /sqrt(2))
					   *scale;
    int upLeftSpeed  = ((-(100.0/128)*(float)joyx +
                        (100.0/128)*(float)joyy)
                       /sqrt(2))
					   *scale;
    motor[motorE] = upRightSpeed;
    motor[motorD] = upRightSpeed;
    motor[motorG] = upLeftSpeed;
    motor[motorF] = upLeftSpeed;
}

void latchrelease() {
//write code when given a latch release mechanism design from mech
writedebugstreanline("Latch Release initiated");
}
