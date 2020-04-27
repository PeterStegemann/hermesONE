// Copyright 2013 Peter Stegemann

#ifndef SCREEN_SETUP_SOURCE_PROXY_H
#define SCREEN_SETUP_SOURCE_PROXY_H

#include "Base.h"
#include "GUI/Setup/CheckBox.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Graph.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Channel.h"
#include "Setup/Defines.h"

class Screen_Setup_Source_Proxy : public Screen_Setup_Source_Base
{
	private:
		Signal_Source_Proxy* sourceProxy;

		GUI_Setup_Label sourceNameLabel;
		char sourceName[ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_Label sourceVolumeLabel;

		virtual void display( void);
		virtual bool processMenu( DoMenuResult Result);

		static void updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);

	public:
		Screen_Setup_Source_Proxy( uint8_t SignalSourceId);
};

#endif
