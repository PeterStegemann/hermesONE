// Copyright 2010 Peter Stegemann

#ifndef SCREEN_STATUS_MENU_CALIBRATION_H
#define SCREEN_STATUS_MENU_CALIBRATION_H

#include "Base.h"

class Screen_Status_Menu_Calibration : public Screen_Status_Menu_Base
{
	private:
		uint8_t calibrationId;

		Signal_Calibration* calibration;

		void updateValue( uint8_t ValueId);

		// Return false if the screen should exit.
		virtual bool processMenu( DoMenuResult Result);

	protected:
		// Initial display.
		virtual void display( void);

	public:
		Screen_Status_Menu_Calibration( uint8_t CalibrationId);
		virtual ~Screen_Status_Menu_Calibration( void);
};

#endif
