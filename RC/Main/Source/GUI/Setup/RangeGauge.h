// Copyright 2012 Peter Stegemann

#pragma once

#include "Gauge.h"

class GUI_Setup_RangeGauge : public GUI_Setup_Gauge
{
	private:
		GUI_Setup_Marker lowerMarker, upperMarker;

		// Remembered values.
		int16_t lowerLimit, upperLimit;

	public:
		GUI_Setup_RangeGauge( void);

		void SetRange( int16_t LowerLimit, int16_t UpperLimit);

		virtual void Display( int16_t Minimum, int16_t Maximum, int16_t Value);
};
