// Copyright 2008 Peter Stegemann

#pragma once

#include "AVR/Components/Types.h"

class Input_Rotary
{
  private:
	int8_t rotarySubValue;
	uint8_t lastRotarySelect;
	uint8_t triggerValue;

  public:
	// Pass in the idle values of the rotary here.
	void Initialize( bool InputA, bool InputB);

	int8_t CalculateDifference( bool InputA, bool InputB);
};
