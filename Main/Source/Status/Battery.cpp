// Copyright 2011 Peter Stegemann

#include "Battery.h"

#define BATTERY_WARN_CHECK_DELAY		10

#define BATTERY_WARN_LOW_PAUSE			50
#define BATTERY_WARN_CRITICAL_PAUSE		5
#define BATTERY_WARN_PAUSE_RANGE		( BATTERY_WARN_LOW_PAUSE - BATTERY_WARN_CRITICAL_PAUSE)

#define BATTERY_WARN_BEEP_LENGTH		50
#define BATTERY_WARN_BEEP_PAUSE			50
#define BATTERY_WARN_BEEP_REPEAT		2

Status_Battery::Status_Battery( void)
			  // Initialize this with a pause to avoid false alarms on startup.
			  : warnCount( BATTERY_WARN_LOW_PAUSE)
{
	GLOBAL.SetupService.GetBattery( &batterySetup);

	// Start with a reasonable value.
	batteryVoltage = batterySetup.WarnLowVoltage;
}

void Status_Battery::calculateVoltage( void)
{
	uint32_t BatteryValue = GetRawVoltage();

	BatteryValue *= batterySetup.CalibrationVoltage;
	BatteryValue /= 1023;

	BatteryValue = BatteryValue + batteryVoltage * 9;
	BatteryValue /= 10;

	batteryVoltage = BatteryValue;
}

void Status_Battery::Process( void)
{
	calculateVoltage();

	// Count down warning pause.
	if( warnCount != 0)
	{
		warnCount--;

		return;
	}

	// Check for current warning level.
	uint8_t Voltage = GetVoltage();
	
	if( Voltage > batterySetup.WarnLowVoltage)
	{
		// Check again in a second, no warning needed now.
		warnCount = BATTERY_WARN_CHECK_DELAY;

		return;
	}

	uint8_t WarnVoltageLevel;

	if( Voltage <= batterySetup.WarnCriticalVoltage)
	{
		// Full alarm.
		WarnVoltageLevel = 0;
	}
	else
	{
		WarnVoltageLevel = Voltage - batterySetup.WarnCriticalVoltage;
	}

	uint8_t WarnVoltageRange = batterySetup.WarnLowVoltage - batterySetup.WarnCriticalVoltage;

	uint32_t WarnPause = BATTERY_WARN_PAUSE_RANGE;
	WarnPause *= WarnVoltageLevel;
	WarnPause /= WarnVoltageRange;
	WarnPause += BATTERY_WARN_CRITICAL_PAUSE;

	warnCount = WarnPause;

	GLOBAL.StatusService.Buzz( BATTERY_WARN_BEEP_LENGTH, BATTERY_WARN_BEEP_PAUSE, BATTERY_WARN_BEEP_REPEAT);
}

void Status_Battery::SetBatterySetup( Setup_Battery* BatterySetup)
{
	batterySetup = *BatterySetup;
}

uint8_t Status_Battery::GetWarnLowVoltage( void)
{
	return( batterySetup.WarnLowVoltage);
}

uint8_t Status_Battery::GetWarnCriticalVoltage( void)
{
	return( batterySetup.WarnCriticalVoltage);
}

uint8_t Status_Battery::GetMinimumVoltage( void)
{
	return( batterySetup.MinimumVoltage);
}

uint8_t Status_Battery::GetMaximumVoltage( void)
{
	return( batterySetup.MaximumVoltage);
}

uint8_t Status_Battery::GetCalibrationVoltage( void)
{
	return( batterySetup.CalibrationVoltage);
}

uint16_t Status_Battery::GetRawVoltage( void)
{
	return( GLOBAL.SignalProcessor.GetRawAnalogValue( INPUT_ANALOG_PORT_VOLTAGE));
}

uint8_t Status_Battery::GetVoltage( void)
{
	return( batteryVoltage);
}
