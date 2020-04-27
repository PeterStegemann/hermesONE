// Copyright 2011 Peter Stegemann

#ifndef SCREEN_SETUP_SOURCE_TIMER_H
#define SCREEN_SETUP_SOURCE_TIMER_H

#include "Base.h"
#include "GUI/Setup/CheckBox.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "GUI/Setup/TimeLabel.h"
#include "Setup/Source/Source.h"

class Screen_Setup_Source_Timer : public Screen_Setup_Source_Base
{
	private:
		Signal_Source_Timer* sourceTimer;

		GUI_Setup_TimeLabel initTimeLabel;

		GUI_Setup_Label storeLabel;
		GUI_Setup_CheckBox storeCheckBox;

		GUI_Setup_TimeLabel timeLabel;

		GUI_Setup_Label reverseLabel;
		GUI_Setup_CheckBox reverseCheckBox;

		GUI_Setup_Label triggerNameLabel;
		char triggerName[ SETUP_SOURCE_NAME_SIZE + 1];

		GUI_Setup_Gauge triggerGauge;

		GUI_Setup_Label triggerHighLimitLabel;
		GUI_Setup_Label triggerLowLimitLabel;

		GUI_Setup_TimeLabel warnLowTimeLabel;
		GUI_Setup_TimeLabel warnCriticalTimeLabel;

		GUI_Setup_TimeLabel warnPauseTimeLabel;

		virtual void display( void);
		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

		static void updateTime( void* Object, GUI_Setup_Label* Label, int16_t Value);
		static void updateTriggerLimit( void* Object, GUI_Setup_Label* Label, int16_t Value);
		static void updateVolume( GUI_Setup_Label* Label, int16_t Value, int16_t SignalPerValue);

	public:
		Screen_Setup_Source_Timer( uint8_t SignalSourceId);
};

#endif
