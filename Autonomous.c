#pragma config(Sensor, S1,     light1,         sensorLightActive)
#pragma config(Sensor, S2,     light2,         sensorLightActive)
#include "JoystickDriver.c"

void initializeRobot()
{
  return;
}

task main()
{
  initializeRobot();
  waitForStart();
  while(true)
  	{

}