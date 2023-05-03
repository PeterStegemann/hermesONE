// Copyright 2008 Peter Stegemann

#pragma once

#include "Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/SourceTupleGraph.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Defines.h"
#include "Setup/Source/Source.h"

class Screen_Setup_Source_Map : public Screen_Setup_Source_Base
{
	private:
		Signal_Source_Map* sourceMap;

		GUI_Setup_Label sourceNameLabel;
		char sourceName[ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_Label sourceVolumeLabel;

		GUI_Setup_Label pointSourceNameLabel[ SETUP_SOURCE_MAP_POINTS];
		char pointSourceName[ SETUP_SOURCE_MAP_POINTS][ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_Label pointVolumeLabel[ SETUP_SOURCE_MAP_POINTS];

		GUI_Setup_SourceTupleGraph mapGraph;

		virtual void display( void);
		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

		static void updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);
		static void updatePointVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);

		// This is used for the GUI_Setup_Select callbacks.
		uint8_t currentPointId;

	public:
		Screen_Setup_Source_Map( uint8_t SignalSourceId);
};
