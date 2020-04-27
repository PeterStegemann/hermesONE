// Copyright 2008 Peter Stegemann

#ifndef SCREEN_SETUP_CHANNEL_H
#define SCREEN_SETUP_CHANNEL_H

#include "Base.h"
#include "GUI/Setup/CheckBox.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/PointGraph.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Channel.h"
#include "Setup/Defines.h"

class Screen_Setup_Channel : public Screen_Setup_Base
{
	private:
		uint8_t channelId;
		Signal_Channel* channel;

		// Point array including modifications by trim and limit for graph display.
		int16_t points[ Setup_Channel::PV_PointVolumeCount];

		GUI_Setup_Label channelNameLabel;
		GUI_Setup_Label channelNameValueLabel;
		char channelName[ SETUP_CHANNEL_NAME_SIZE + 1];
		GUI_Setup_TextInput channelNameInput;

		GUI_Setup_Label sourceNameLabel;
		char sourceName[ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_Label sourceVolumeLabel;

		GUI_Setup_Label trimNameLabel;
		char trimName[ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_Label trimVolumeLabel;

		GUI_Setup_Label limitNameLabel;
		char limitName[ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_Label limitVolumeLabel;

		GUI_Setup_CheckBox reverseCheckBox;
		bool reverse;

		GUI_Setup_Label modeLabel;
	
		GUI_Setup_Label pointVolumeLabel[ Setup_Channel::PV_PointVolumeCount];

		GUI_Setup_PointGraph channelGraph;

		GUI_Setup_Gauge currentGauge;

		virtual void display( void);
		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

		void doChannelName( void);
		bool doSource( uint8_t* SignalSourceId, Setup_Source_Tupel* SourceTupel,
					   char SourceName[ SETUP_SOURCE_NAME_SIZE + 1],
					   GUI_Setup_Label* SourceNameLabel, GUI_Setup_Label* SourceVolumeLabel);

		void updatePointVolume( uint8_t CurrentPointId, int16_t Value);
		void updateMode( GUI_Setup_Label* Label, int8_t Value);

		static void updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);
		static void updatePointVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);
		static void updateMode( void* Object, GUI_Setup_Label* Label, int8_t Value);

		// This is used for the GUI_Setup_Select callbacks.
		uint8_t currentPointId;
		GUI_Setup_Label* currentVolumeLabel;

	public:
		Screen_Setup_Channel( uint8_t ChannelId);
};

#endif
