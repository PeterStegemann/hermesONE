// Copyright 2008 Peter Stegemann

#ifndef SCREEN_SETUP_STATUS_H
#define SCREEN_SETUP_STATUS_H

#include "Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TimeLabel.h"
#include "GUI/Setup/VoltageLabel.h"
#include "Setup/Defines.h"

class Screen_Setup_Status : public Screen_Setup_Base
{
	private:
		uint8_t statusSource[ SETUP_MODEL_STATUS_SOURCES];
		GUI_Setup_Gauge statusGauge[ SETUP_MODEL_STATUS_SOURCES];

		GUI_Setup_VoltageLabel batteryLabel;
		GUI_Setup_Gauge batteryGauge;

		uint8_t timerSource[ SETUP_MODEL_STATUS_TIMERS];
		GUI_Setup_TimeLabel timerLabel[ SETUP_MODEL_STATUS_TIMERS];

		virtual void display( void);
		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

	public:
		Screen_Setup_Status( void);
};

#endif
