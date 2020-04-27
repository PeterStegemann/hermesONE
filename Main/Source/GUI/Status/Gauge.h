// Copyright 2012 Peter Stegemann

#ifndef GUI_STATUS_GAUGE_H
#define GUI_STATUS_GAUGE_H

#include "AVR/Components/Types.h"

#define SCREEN_STATUS_STATUS_HORIZONTAL_GAUGE_WIDTH		44
#define SCREEN_STATUS_STATUS_VERTICAL_GAUGE_HEIGHT		40

#define SCREEN_STATUS_STATUS_HORIZONTAL_BUTTON_WIDTH	4
#define SCREEN_STATUS_STATUS_VERTICAL_BUTTON_HEIGHT		4

class GUI_Status_Gauge
{
	public:
		enum Options
		{
			O_None					= 0b00000000,
			O_Horizontal			= 0b00000000,
			O_Vertical				= 0b00000001
		};

	private:
		uint16_t left, top;

		Options options;

		FONT::FontId fontId;

		// Marks wether we have remembered values from last draw.
		bool remembered;
		// Remembered values.
		int16_t minimum, maximum, value;

		void clearVerticalGauge( void);
		void clearHorizontalGauge( void);
		void drawVerticalGauge( void);
		void drawHorizontalGauge( void);

	public:
		GUI_Status_Gauge( void);

		// Set the location of the gauge on the screen.
		void SetDimensions( uint16_t Left, uint16_t Top);

		// Set some display options.
		void SetOptions( Options UseOptions);

		// This will update the view to reflect the given values.
		void Display( int16_t Value);
		void Display( int16_t Minimum, int16_t Maximum, int16_t Value);

		// Clear gauge content.
		void Clear( void);
};

#endif
