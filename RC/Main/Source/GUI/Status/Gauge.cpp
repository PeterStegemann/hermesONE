// Copyright 2012 Peter Stegemann

#include "Gauge.h"

#include "Defines.h"

GUI_Status_Gauge::GUI_Status_Gauge( void)
				: left( 0)
				, top( 0)
				, options( O_None)
				, fontId( GUI_STATUS_MAIN_FONT)
				, remembered( false)
				, minimum( 0)
				, maximum( 0)
				, value( 0)
{
}

void GUI_Status_Gauge::SetDimensions( uint16_t Left, uint16_t Top)
{
	left = Left;
	top = Top;
}

void GUI_Status_Gauge::SetOptions( Options UseOptions)
{
	options = UseOptions;
}

void GUI_Status_Gauge::Display( int16_t Value)
{
	Display( minimum, maximum, Value);
}

void GUI_Status_Gauge::Display( int16_t Minimum, int16_t Maximum, int16_t Value)
{
	if( remembered == true)
	{
		// If we have the same values as last time, don't display anything.
		if(( minimum == Minimum) && ( maximum == Maximum) && ( value == Value))
		{
			// No changes.
			return;
		}
	}

	// Remember values.
	minimum = Minimum;
	maximum = Maximum;
	value = Value;
	remembered = true;

	// Draw.
	if( options & O_Vertical)
	{
		drawVerticalGauge();
	}
	else
	{
		drawHorizontalGauge();
	}
}

void GUI_Status_Gauge::Clear( void)
{
	remembered = false;

	// Draw.
	if( options & O_Vertical)
	{
		clearVerticalGauge();
	}
	else
	{
		clearHorizontalGauge();
	}
}

void GUI_Status_Gauge::drawVerticalGauge( void)
{
	int32_t LargeValue = value - ( int32_t) minimum;
	LargeValue *= ( SCREEN_STATUS_STATUS_VERTICAL_GAUGE_HEIGHT -
				    SCREEN_STATUS_STATUS_VERTICAL_BUTTON_HEIGHT);
	uint8_t Value = LargeValue / SIGNAL_VALUE_RANGE;
	
	uint8_t Base = left;
	
	// Draw base lines.
	for( uint8_t Page = top; Page < ( top + 5); Page++)
	{
		GLOBAL.StatusDisplay.Write( Base + 0, Page, 0b00000000);
		GLOBAL.StatusDisplay.Write( Base + 1, Page, 0b00000000);
		GLOBAL.StatusDisplay.Write( Base + 2, Page, 0b11111111);
		GLOBAL.StatusDisplay.Write( Base + 3, Page, 0b11111111);
		GLOBAL.StatusDisplay.Write( Base + 4, Page, 0b00000000);
		GLOBAL.StatusDisplay.Write( Base + 5, Page, 0b00000000);
	}
	
	// Draw button.
	Value = ( SCREEN_STATUS_STATUS_VERTICAL_GAUGE_HEIGHT -
			  SCREEN_STATUS_STATUS_VERTICAL_BUTTON_HEIGHT) - Value;
	
	int8_t Page = ( Value / 8) + 2;
	uint8_t Line = Value % 8;
	
	if( Page >= 0)
	{
		uint8_t Byte = 0b00001111 << Line;
		
		GLOBAL.StatusDisplay.Write( Base + 0, Page, Byte);
		GLOBAL.StatusDisplay.Write( Base + 5, Page, Byte);
		
		Byte = 0b00001001 << Line;
		GLOBAL.StatusDisplay.Write( Base + 1, Page, Byte);
		GLOBAL.StatusDisplay.Write( Base + 4, Page, Byte);
		
		Byte = ~( 0b00000110 << Line);
		GLOBAL.StatusDisplay.Write( Base + 2, Page, Byte);
		GLOBAL.StatusDisplay.Write( Base + 3, Page, Byte);
	}

	Page++;

	if( Page <= 6)
	{
		uint8_t Byte = 0b00001111 >> ( 8 - Line);
		GLOBAL.StatusDisplay.Write( Base + 0, Page, Byte);
		GLOBAL.StatusDisplay.Write( Base + 5, Page, Byte);

		Byte = 0b00001001 >> ( 8 - Line);
		GLOBAL.StatusDisplay.Write( Base + 1, Page, Byte);
		GLOBAL.StatusDisplay.Write( Base + 4, Page, Byte);

		Byte = ~( 0b00000110 >> ( 8 - Line));
		GLOBAL.StatusDisplay.Write( Base + 2, Page, Byte);
		GLOBAL.StatusDisplay.Write( Base + 3, Page, Byte);
	}
}

void GUI_Status_Gauge::drawHorizontalGauge( void)
{
	int32_t LargeValue = value - ( int32_t) minimum;
	LargeValue *= ( SCREEN_STATUS_STATUS_HORIZONTAL_GAUGE_WIDTH -
				    SCREEN_STATUS_STATUS_HORIZONTAL_BUTTON_WIDTH);
	uint8_t Value = LargeValue / SIGNAL_VALUE_RANGE;

	uint8_t Page = top;
	uint8_t Base = left;
	uint8_t End = Base + SCREEN_STATUS_STATUS_HORIZONTAL_GAUGE_WIDTH;

	for( uint8_t X = Base; X < End; X++)
	{
		GLOBAL.StatusDisplay.Write( X, Page, 0b00110000);
	}

	// Draw button.
	GLOBAL.StatusDisplay.Write( Value + Base++, Page, 0b11111100);
	GLOBAL.StatusDisplay.Write( Value + Base++, Page, 0b10000100);
	GLOBAL.StatusDisplay.Write( Value + Base++, Page, 0b10000100);
	GLOBAL.StatusDisplay.Write( Value + Base, Page, 0b11111100);
}

void GUI_Status_Gauge::clearVerticalGauge( void)
{
	uint8_t Base = left;

	// Draw base lines.
	for( uint8_t Page = top; Page < ( top + 5); Page++)
	{
		GLOBAL.StatusDisplay.Write( Base + 0, Page, 0b00000000);
		GLOBAL.StatusDisplay.Write( Base + 1, Page, 0b00000000);
		GLOBAL.StatusDisplay.Write( Base + 2, Page, 0b00000000);
		GLOBAL.StatusDisplay.Write( Base + 3, Page, 0b00000000);
		GLOBAL.StatusDisplay.Write( Base + 4, Page, 0b00000000);
		GLOBAL.StatusDisplay.Write( Base + 5, Page, 0b00000000);
	}
}

void GUI_Status_Gauge::clearHorizontalGauge( void)
{
	uint8_t Page = top;
	uint8_t Base = left;
	uint8_t End = Base + SCREEN_STATUS_STATUS_HORIZONTAL_GAUGE_WIDTH;
	
	for( uint8_t X = Base; X < End; X++)
	{
		GLOBAL.StatusDisplay.Write( X, Page, 0b00000000);
	}
}
