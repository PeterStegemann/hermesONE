// Copyright 2012 Peter Stegemann

#pragma once

#include "Defines.h"

#include "Signal/Defines.h"

#include "AVR/Source/Types.h"

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
    LCD_DOG* statusDisplay;

	uint16_t left, top;

    Options options;

    avr::font::FontId fontId;

	// Marks whether we have remembered values from last draw.
	bool remembered;
	// Remembered values.
	int16_t minimum, maximum, value;

	void clearVerticalGauge( void)
	{
        uint8_t Base = left;

        // Draw base lines.
        for( uint8_t Page = top; Page < ( top + 5); Page++)
        {
            statusDisplay->Write( Base + 0, Page, 0b00000000);
            statusDisplay->Write( Base + 1, Page, 0b00000000);
            statusDisplay->Write( Base + 2, Page, 0b00000000);
            statusDisplay->Write( Base + 3, Page, 0b00000000);
            statusDisplay->Write( Base + 4, Page, 0b00000000);
            statusDisplay->Write( Base + 5, Page, 0b00000000);
        }
    }

	void clearHorizontalGauge( void)
	{
        uint8_t Page = top;
        uint8_t Base = left;
        uint8_t End = Base + SCREEN_STATUS_STATUS_HORIZONTAL_GAUGE_WIDTH;

        for( uint8_t X = Base; X < End; X++)
        {
            statusDisplay->Write( X, Page, 0b00000000);
        }
    }

	void drawVerticalGauge( void)
    {
        int32_t LargeValue = value - ( int32_t) minimum;
        LargeValue *= ( SCREEN_STATUS_STATUS_VERTICAL_GAUGE_HEIGHT - SCREEN_STATUS_STATUS_VERTICAL_BUTTON_HEIGHT);
        uint8_t Value = LargeValue / SIGNAL_VALUE_RANGE;

        uint8_t Base = left;

        // Draw base lines.
        for( uint8_t Page = top; Page < ( top + 5); Page++)
        {
            statusDisplay->Write( Base + 0, Page, 0b00000000);
            statusDisplay->Write( Base + 1, Page, 0b00000000);
            statusDisplay->Write( Base + 2, Page, 0b11111111);
            statusDisplay->Write( Base + 3, Page, 0b11111111);
            statusDisplay->Write( Base + 4, Page, 0b00000000);
            statusDisplay->Write( Base + 5, Page, 0b00000000);
        }

        // Draw button.
        Value = ( SCREEN_STATUS_STATUS_VERTICAL_GAUGE_HEIGHT - SCREEN_STATUS_STATUS_VERTICAL_BUTTON_HEIGHT) - Value;

        int8_t Page = ( Value / 8) + 2;
        uint8_t Line = Value % 8;

        if( Page >= 0)
        {
            uint8_t Byte = 0b00001111 << Line;

            statusDisplay->Write( Base + 0, Page, Byte);
            statusDisplay->Write( Base + 5, Page, Byte);

            Byte = 0b00001001 << Line;
            statusDisplay->Write( Base + 1, Page, Byte);
            statusDisplay->Write( Base + 4, Page, Byte);

            Byte = ~( 0b00000110 << Line);
            statusDisplay->Write( Base + 2, Page, Byte);
            statusDisplay->Write( Base + 3, Page, Byte);
        }

        Page++;

        if( Page <= 6)
        {
            uint8_t Byte = 0b00001111 >> ( 8 - Line);
            statusDisplay->Write( Base + 0, Page, Byte);
            statusDisplay->Write( Base + 5, Page, Byte);

            Byte = 0b00001001 >> ( 8 - Line);
            statusDisplay->Write( Base + 1, Page, Byte);
            statusDisplay->Write( Base + 4, Page, Byte);

            Byte = ~( 0b00000110 >> ( 8 - Line));
            statusDisplay->Write( Base + 2, Page, Byte);
            statusDisplay->Write( Base + 3, Page, Byte);
        }
    }

	void drawHorizontalGauge( void)
    {
        int32_t LargeValue = value - ( int32_t) minimum;
        LargeValue *= ( SCREEN_STATUS_STATUS_HORIZONTAL_GAUGE_WIDTH - SCREEN_STATUS_STATUS_HORIZONTAL_BUTTON_WIDTH);
        uint8_t Value = LargeValue / SIGNAL_VALUE_RANGE;

        uint8_t Page = top;
        uint8_t Base = left;
        uint8_t End = Base + SCREEN_STATUS_STATUS_HORIZONTAL_GAUGE_WIDTH;

        for( uint8_t X = Base; X < End; X++)
        {
            statusDisplay->Write( X, Page, 0b00110000);
        }

        // Draw button.
        statusDisplay->Write( Value + Base++, Page, 0b11111100);
        statusDisplay->Write( Value + Base++, Page, 0b10000100);
        statusDisplay->Write( Value + Base++, Page, 0b10000100);
        statusDisplay->Write( Value + Base, Page, 0b11111100);
    }

  public:
    GUI_Status_Gauge( LCD_DOG* StatusDisplay)
        : statusDisplay( StatusDisplay)
        , left( 0)
        , top( 0)
        , options( O_None)
        , fontId( GUI_STATUS_MAIN_FONT)
        , remembered( false)
        , minimum( 0)
        , maximum( 0)
        , value( 0)
    {
    }

	// Set the location of the gauge on the screen.
	void SetDimensions( uint16_t Left, uint16_t Top)
    {
	    left = Left;
    	top = Top;
    }

	// Set some display options.
	void SetOptions( Options UseOptions)
    {
	    options = UseOptions;
    }

	// This will update the view to reflect the given values.
	void Display( int16_t Value)
    {
        Display( minimum, maximum, Value);
    }

	void Display( int16_t Minimum, int16_t Maximum, int16_t Value)
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

	// Clear gauge content.
	void Clear( void)
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
};
