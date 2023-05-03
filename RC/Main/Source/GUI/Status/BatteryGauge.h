// Copyright 2012 Peter Stegemann

#pragma once

#include "Label.h"

class GUI_Status_BatteryGauge
{
	private:
		// Marks whether we have remembered values from last draw.
		bool remembered;

		// Remembered values.
		int8_t minimum, maximum, value;

	public:
		GUI_Status_BatteryGauge( void);

		void Display( uint8_t Value);
		void Display( uint8_t Minimum, uint8_t Maximum, uint8_t Value);
};
