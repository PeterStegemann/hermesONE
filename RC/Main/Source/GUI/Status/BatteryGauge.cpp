// Copyright 2012 Peter Stegemann

#include "TimeLabel.h"

#include "Text/Text.h"
GUI_Status_BatteryGauge::GUI_Status_BatteryGauge( void)
					   : remembered( false)
					   , minimum( 0)
					   , maximum( 0)
					   , value( 0)
{
}

void GUI_Status_BatteryGauge::Display( uint8_t Value)
{
	Display( minimum, maximum, Value);
}

void GUI_Status_BatteryGauge::Display( uint8_t Minimum, uint8_t Maximum, uint8_t Value)
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

	if( Value < Minimum)
	{
		Value = Minimum;
	}
	else if( Value > Maximum)
	{
		Value = Maximum;
	}

	uint8_t Range = Maximum - Minimum;
	Value -= Minimum;

	uint8_t Page = 3;
	uint8_t Base = 7;
	
	GLOBAL.StatusDisplay.Write( Base++, Page, 0b11111110);
	GLOBAL.StatusDisplay.Write( Base++, Page, 0b10000010);
	
	uint8_t Width = GLOBAL.StatusDisplay.GetWidth() - ( 2 * Base);
	uint8_t Mid = Base + (( uint16_t) Value * ( uint16_t) Width) / ( uint16_t) Range;
	uint8_t End = Base + Width;
	
	uint8_t X = Base;
	
	for( ; X < Mid; X++)
	{
		GLOBAL.StatusDisplay.Write( X, Page, 0b10111010);
	}
	
	for( ; X < End; X++)
	{
		GLOBAL.StatusDisplay.Write( X, Page, 0b10000010);
	}
	
	GLOBAL.StatusDisplay.Write( X++, Page, 0b10000010);
	GLOBAL.StatusDisplay.Write( X++, Page, 0b11111110);
}
