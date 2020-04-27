// Copyright 2011 Peter Stegemann

#ifndef SCREEN_SETUP_SYSTEM_BATTERY_H
#define SCREEN_SETUP_SYSTEM_BATTERY_H

#include "../Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/VoltageLabel.h"
#include "Setup/Battery.h"

class Screen_Setup_System_Battery : public Screen_Setup_Base
{
	private:
		Setup_Battery batterySetup;
		uint8_t calibrationVoltage;

		GUI_Setup_VoltageLabel warnLowVoltageLabel;
		GUI_Setup_VoltageLabel warnCriticalVoltageLabel;

		GUI_Setup_VoltageLabel minimumVoltageLabel;
		GUI_Setup_VoltageLabel maximumVoltageLabel;

		GUI_Setup_VoltageLabel calibrationVoltageLabel;

		GUI_Setup_VoltageLabel batteryLabel;
		GUI_Setup_Gauge batteryGauge;

		virtual void display( void);
		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

		static void updateVoltage( void* Object, GUI_Setup_Label* Label, int8_t Value);
		static void updateCalibrationVoltage( void* Object, GUI_Setup_Label* Label, int8_t Value);

	public:
		Screen_Setup_System_Battery( void);
};

#endif
