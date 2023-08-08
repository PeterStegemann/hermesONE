// Copyright 2012 Peter Stegemann

#pragma once

#include "Label.h"

class GUI_Status_BatteryGauge
{
  private:
    LCD_DOG* statusDisplay;

    // Marks whether we have remembered values from last draw.
    bool remembered;

    // Remembered values.
    int8_t minimum, maximum, value;

  public:
    GUI_Status_BatteryGauge( LCD_DOG* StatusDisplay)
        : statusDisplay( StatusDisplay)
        , remembered( false)
        , minimum( 0)
        , maximum( 0)
        , value( 0)
    {
    }

    void Display( uint8_t Value)
    {
        Display( minimum, maximum, Value);
    }

    void Display( uint8_t Minimum, uint8_t Maximum, uint8_t Value)
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

    	statusDisplay->Write( Base++, Page, 0b11111110);
	    statusDisplay->Write( Base++, Page, 0b10000010);

    	uint8_t Width = statusDisplay->GetWidth() - ( 2 * Base);
	    uint8_t Mid = Base + (( uint16_t) Value * ( uint16_t) Width) / ( uint16_t) Range;
	    uint8_t End = Base + Width;

    	uint8_t X = Base;

    	for( ; X < Mid; X++)
	    {
    		statusDisplay->Write( X, Page, 0b10111010);
	    }

    	for( ; X < End; X++)
	    {
    		statusDisplay->Write( X, Page, 0b10000010);
	    }

    	statusDisplay->Write( X++, Page, 0b10000010);
	    statusDisplay->Write( X++, Page, 0b11111110);
    }
};
