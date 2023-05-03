// Copyright 2008 Peter Stegemann

#pragma once

#include "Base.h"
#include "GUI/Setup/CheckBox.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Defines.h"
#include "Setup/Source/Source.h"

class Screen_Setup_Source_Input : public Screen_Setup_Source_Base
{
	private:
		Setup_Source_Input* sourceInput;

		uint8_t previousType;

		GUI_Setup_Label typeValueLabel;

		GUI_Setup_Label inputALabel;
		GUI_Setup_Label inputAValueLabel;
		char inputAValue[ 2 + 1];
		GUI_Setup_Label inputBLabel;
		GUI_Setup_Label inputBValueLabel;
		char inputBValue[ 2 + 1];

		GUI_Setup_Label storeLabel;
		GUI_Setup_CheckBox storeCheckBox;
		GUI_Setup_Label initLabel;
		GUI_Setup_Label initValueLabel;

		GUI_Setup_Label stepLabel;
		GUI_Setup_Label stepValueLabel;

		GUI_Setup_Label toggleLabel;
		GUI_Setup_CheckBox toggleCheckBox;

		GUI_Setup_Label topLabel;
		GUI_Setup_Label topValueLabel;
		GUI_Setup_Label bottomLabel;
		GUI_Setup_Label bottomValueLabel;

		virtual void display( void);
		virtual bool processMenu( DoMenuResult Result);

		void updateType( void);
		void clear( void);
		void draw( void);

		static void updateType( void* Object, GUI_Setup_Label* Label, int8_t Value);
		static void updateInputAnalog( void* Object, GUI_Setup_Label* Label, int8_t Value);
		static void updateInputDigitalA( void* Object, GUI_Setup_Label* Label, int8_t Value);
		static void updateInputDigitalB( void* Object, GUI_Setup_Label* Label, int8_t Value);
		static void updateInit( void* Object, GUI_Setup_Label* Label, int16_t Value);
		static void updateStep( void* Object, GUI_Setup_Label* Label, int16_t Value);
		static void updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);

	public:
		Screen_Setup_Source_Input( uint8_t SignalSourceId);
};
