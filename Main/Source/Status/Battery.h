// Copyright 2011 Peter Stegemann

#ifndef STATUS_BATTERY_CONTROL_H
#define STATUS_BATTERY_CONTROL_H

#include "AVR/Components/Types.h"
#include "Setup/Battery.h"

class Status_Battery
{
	private:
		Setup_Battery batterySetup;

		uint8_t batteryVoltage;

		uint16_t warnCount;

		void calculateVoltage( void);

	public:
		Status_Battery( void);

		// This is for the interrupt, not for you.
		void Process( void);

		void SetBatterySetup( Setup_Battery* BatterySetup);

		uint8_t GetWarnLowVoltage( void);
		uint8_t GetWarnCriticalVoltage( void);

		uint8_t GetMinimumVoltage( void);
		uint8_t GetMaximumVoltage( void);

		uint8_t GetCalibrationVoltage( void);

		// Raw voltage value.
		uint16_t GetRawVoltage( void);

		// The current, calibrated voltage value.
		uint8_t GetVoltage( void);
};

#endif
