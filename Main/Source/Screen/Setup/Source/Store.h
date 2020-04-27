// Copyright 2009 Peter Stegemann

#ifndef SCREEN_SETUP_SOURCE_STORE_H
#define SCREEN_SETUP_SOURCE_STORE_H

#include "Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Source/Source.h"

class Screen_Setup_Source_Store : public Screen_Setup_Source_Base
{
	private:
		Signal_Source_Store* sourceStore;

		GUI_Setup_Label sourceNameLabel;
		char sourceName[ SETUP_SOURCE_NAME_SIZE + 1];

		virtual void display( void);
		virtual bool processMenu( DoMenuResult Result);

	public:
		Screen_Setup_Source_Store( uint8_t SignalSourceId);
};

#endif
