// Copyright 2007 Peter Stegemann

#ifndef SCREEN_SETUP_SYSTEM_H
#define SCREEN_SETUP_SYSTEM_H

#include "Base.h"
#include "GUI/Setup/CheckBox.h"
#include "GUI/Setup/MillisecondLabel.h"

class Screen_Setup_System : public Screen_Setup_Base
{
	private:
		virtual void display( void);
		virtual bool processMenu( DoMenuResult Result);

		void doSerial( void);
		void doDisplay( void);
		void doBattery( void);
		void doPPM( uint8_t Id);
		void doCalibration( void);
		void doReset( void);

	public:
		Screen_Setup_System( void);
};

#endif
