#pragma config(Sensor, in1,    poti,           sensorPotentiometer)
#pragma config(Sensor, dgtl1,  button,         sensorTouch)
#pragma config(Sensor, dgtl2,  top,            sensorTouch)
#pragma config(Sensor, dgtl3,  bottom,         sensorTouch)
#pragma config(Motor,  port2,           left1,         tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           right1,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           left2,         tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port5,           right2,        tmotorVex393HighSpeed_MC29, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	SensorValue[poti];
	unsigned long time, diff;
	clearDebugStream();
	while (true)
	{
		while (!SensorValue[button]) sleep(10);
		time = nPgmTime;
		motor[left1] = motor[right1] = motor[left2] = motor[right2] = 127;
		while (!SensorValue[top]) sleep(10);
		motor[left1] = motor[right1] = motor[left2] = motor[right2] = 10;
		diff = nPgmTime - time;
		writeDebugStreamLine("Up:   %d.%d s", diff / 1000, diff % 1000);
		while (SensorValue[button]) sleep(10);

		while (!SensorValue[button]) sleep(10);
		 time = nPgmTime;
		motor[left1] = motor[right1] = motor[left2] = motor[right2] = -127;
		while (!SensorValue[bottom]) sleep(10);
		motor[left1] = motor[right1] = motor[left2] = motor[right2] = -10;
		diff = nPgmTime - time;
		writeDebugStreamLine("Down: %d.%d s", diff / 1000, diff % 1000);
		while (SensorValue[button]) sleep(10);
	}
}
