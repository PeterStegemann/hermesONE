// Copyright 2008 Peter Stegemann

#ifndef SCREEN_SETUP_SYSTEM_CALIBRATION_H
#define SCREEN_SETUP_SYSTEM_CALIBRATION_H

#include "../Base.h"
#include "Ports.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "Signal/Calibration.h"

class Screen_Setup_System_Calibration : public Screen_Setup_Base
{
	private:
		GUI_Setup_Label label[ SIGNAL_PROCESSOR_ANALOG_INPUTS];
		GUI_Setup_Label high[ SIGNAL_PROCESSOR_ANALOG_INPUTS];
		GUI_Setup_Label center[ SIGNAL_PROCESSOR_ANALOG_INPUTS];
		GUI_Setup_Label low[ SIGNAL_PROCESSOR_ANALOG_INPUTS];

		GUI_Setup_Label currentLabel;
		GUI_Setup_Gauge currentGauge;

		void selectColumn( uint8_t Index, uint8_t Column, bool Selected);

		void doCalibration( uint8_t Index);

		virtual void display( void);
		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

	public:
		Screen_Setup_System_Calibration( void);
};

#endif
