// Copyright 2010 Peter Stegemann

#ifndef SCREEN_SETUP_SOURCE_FOLLOWER_H
#define SCREEN_SETUP_SOURCE_FOLLOWER_H

#include "Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
//#include "GUI/Setup/RangeGauge.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Source/Source.h"

class Screen_Setup_Source_Follower : public Screen_Setup_Source_Base
{
	private:
		Signal_Source_Follower* sourceFollower;

		GUI_Setup_Label targetNameLabel;
		char targetName[ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_Label targetVolumeLabel;

		GUI_Setup_Gauge targetGauge;

		GUI_Setup_Label stepNameLabel;
		char stepName[ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_Label stepVolumeLabel;

		GUI_Setup_Gauge stepGauge;

		GUI_Setup_Label triggerNameLabel;
		char triggerName[ SETUP_SOURCE_NAME_SIZE + 1];

		GUI_Setup_Gauge triggerGauge;

		GUI_Setup_Label triggerHighLimitLabel;
		GUI_Setup_Label triggerLowLimitLabel;

		virtual void display( void);
		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

		static void updateTargetVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);
		static void updateStepVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);
		static void updateTriggerLimit( void* Object, GUI_Setup_Label* Label, int16_t Value);
		static void updateVolume( GUI_Setup_Label* Label, int16_t Value, int16_t SignalPerValue);

	public:
		Screen_Setup_Source_Follower( uint8_t SignalSourceId);
};

#endif
