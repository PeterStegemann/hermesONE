// Copyright 2008 Peter Stegemann

#pragma once

#include "AVR/Source/Types.h"

struct Setup_Source_Input
{
	enum InputType
	{
		IT_Analog,
		IT_Button,
		IT_Switch,
		IT_Ticker,
		IT_Rotary,

		IT_InputTypeCount
	};

	uint8_t Type;

	uint8_t InputIdA;
	uint8_t InputIdB;

	bool Store;
	int16_t InitVolume;
	int16_t StepVolume;
	bool Toggle;

	int16_t TopVolume;
	int16_t BottomVolume;
};
