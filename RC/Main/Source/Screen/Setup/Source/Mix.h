// Copyright 2008 Peter Stegemann

#pragma once

#include "Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Defines.h"
#include "Setup/Source/Source.h"

class Screen_Setup_Source_Mix : public Screen_Setup_Source_Base
{
	private:
		Signal_Source_Mix* sourceMix;

		GUI_Setup_Label sourceLabel[ SETUP_SOURCE_MIX_INPUTS];
		char sourceName[ SETUP_SOURCE_MIX_INPUTS][ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_Label volumeLabel[ SETUP_SOURCE_MIX_INPUTS];
		GUI_Setup_Gauge valueGauge[ SETUP_SOURCE_MIX_INPUTS];

		virtual void display( void);
		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

		void doSource( uint8_t Index);
		void selectColumn( uint8_t Index, uint8_t Column, bool Selected);

		static void updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);

		// This is used for the GUI_Setup_Select callbacks.
		uint8_t currentVolumeIndex;

	public:
		Screen_Setup_Source_Mix( uint8_t SignalSourceId);
};
