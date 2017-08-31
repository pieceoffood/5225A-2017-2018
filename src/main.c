#pragma config(Sensor, in1,    autoPoti,       sensorPotentiometer)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#define FORCE_AUTO

#include "Vex_Competition_Includes_Custom.c" // Custom competition includes for field control

/* Headers */
#include "logging.h"
#include "utilities.h"
#include "async.h"
#include "motors.h"
#include "sensors.h"
#include "cycle.h"
#include "pid.h"
#include "lcd.h"
#include "state.h"

/* Setup */
#define LOG_LEVEL LOG_DATA
#define LCD_MENU_ITEMS 2

/* Code */
#include "logging.c"
#include "utilities.c"
#include "async.c"
#include "motors.c"
#include "sensors.c"
#include "cycle.c"
#include "pid.c"
#include "lcd.c"
#include "state.c"

/* Year-dependent */
#include "general.h"
#include "joystick.h"
#include "auto_runs.h"
#include "auto.h"
#include "lcd_control.h"

#include "general.c"
#include "joystick.c"
#include "auto_runs.c"
#include "auto.c"
#include "lcd_control.c"

/* Main */
// This function get's called 2s after the cortex is powered on
void startup()
{
	clearDebugStream();
	datalogClear();
	bLCDBacklight = true;
	setupMotors();
	setupSensors();

	startTask(statusMonitor);
}

// This function get's called every 25ms while the robot is disabled
void disabled()
{
	updateSensorInputs();
	selectAuto();
	runLCD();
}

// This function get's called to handle the autonomus period
task autonomous()
{
	updateSensorInputs();
	selectAuto();
	joystickSetup(1);

	startTask(autoMotorSensorUpdateTask);
	startTask(autoSafetyTask);
	startTask(runLCDTask);

	gAutoStartTime = nPgmTime;
	S_LOG "Auto Start" E_LOG_INFO

	runAuto();

	gAutoTime = nPgmTime - gAutoStartTime;
	S_LOG "Auto End - took %d ms", gAutoTime E_LOG_INFO
}

// This function get's called to handle the user-control period
task usercontrol()
{
	joystickSetup(0);

	clearLCD();
	S_LOG "Driver Start" E_LOG_INFO

	//startTask(testing);

	sCycleData cycle;
	initCycle(cycle, 10);
	while (true)
	{
		updateSensorInputs();
		selectAuto();
		runLCD();

		handleJoysticks();

		updateSensorOutputs();
		updateSensorsLst();
		updateMotors();
		endCycle(cycle);
	}
}
