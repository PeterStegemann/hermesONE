// Copyright 2011 Peter Stegemann

#ifndef SETUP_BATTERY_H
#define SETUP_BATTERY_H

#include "AVR/Components/Types.h"

#define SETUP_BATTERY_VOLTAGE_MINIMUM		0
#define SETUP_BATTERY_VOLTAGE_MAXIMUM		120
#define SETUP_BATTERY_VOLTAGE_STEPS			1

struct Setup_Battery
{
	uint8_t WarnLowVoltage;
	uint8_t WarnCriticalVoltage;

	uint8_t MinimumVoltage;
	uint8_t MaximumVoltage;

	uint8_t CalibrationVoltage;
};

#endif
