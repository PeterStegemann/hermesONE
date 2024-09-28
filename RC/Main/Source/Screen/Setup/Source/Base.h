// Copyright 2012 Peter Stegemann

#pragma once

#include "../Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"

class Screen_Setup_Source_Base : public Screen_Setup_Base
{
	private:
		GUI_Setup_Label currentNameLabel;
		GUI_Setup_Label currentNameValueLabel;
		char currentName[ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_TextInput currentNameInput;

		GUI_Setup_Gauge currentGauge;

		bool hasGauge;

		void doCurrentName( void);

	protected:
		uint8_t signalSourceId;
		uint16_t setupSourceId;

		Signal_Source_Source* source;

		virtual void display( void);
		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

		Screen_Setup_Source_Base
		(
            Input_Service* InputService,
            Interrupt_Service* InterruptService,
		    Screen_Status_Status* StatusScreen,
	    	uint8_t SignalSourceId,
	    	uint32_t MenuPattern,
            const flash_char* Title,
            bool hasGauge = true
        );
};
