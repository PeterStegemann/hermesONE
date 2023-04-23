// Copyright 2011 Peter Stegemann

#ifndef SCREEN_STATUS_MENU_BATTERY_H
#define SCREEN_STATUS_MENU_BATTERY_H

#include "Base.h"

class Screen_Status_Menu_Battery : public Screen_Status_Menu_Base
{
	private:
		Setup_Battery batterySetup;
		uint8_t calibrationVoltage;

		void updateVoltage( int8_t Value);
		static void updateVoltage( void* Object, int8_t Value);
		void updateCalibrationVoltage( int8_t Value);
		static void updateCalibrationVoltage( void* Object, int8_t Value);

		// Return false if the screen should exit.
		virtual bool processMenu( DoMenuResult Result);

	protected:
		// Initial display.
		virtual void display( void);

	public:
		Screen_Status_Menu_Battery( void);
		virtual ~Screen_Status_Menu_Battery( void);
};

#endif
