// Copyright 2008 Peter Stegemann

#pragma once

#include "Defines.h"

#include "AVR/Source/LCD/65K_RGB.h"

class GUI_Setup_CheckBox
{
  public:
    enum Options
    {
        O_None		= 0b00000000,
        O_Crossed	= 0b00000000,
        O_Ticked	= 0b00000001
    };

  private:
    uint16_t left, top, width, height;

    Options options;

    avr::font::FontId fontId;

    // Marks whether we have remembered values from last draw.
    bool remembered;
    // Remembered values.
    bool ticked;

  public:
    LCD_65K_RGB::Color ForegroundColor;
    LCD_65K_RGB::Color BackgroundColor;
    LCD_65K_RGB::Color FillColor;

  public:
    GUI_Setup_CheckBox( void)
        : left( 0)
        , top( 0)
        , width( 0)
        , height( 0)
        , options( O_None)
        , fontId( GUI_SETUP_MAIN_FONT)
        , remembered( false)
        , ticked( false)
        , ForegroundColor( LCD_65K_RGB::C_White)
        , BackgroundColor( LCD_65K_RGB::C_Black)
        , FillColor( LCD_65K_RGB::C_WarmYellow)
    {
    }

    // Set the location of the box on the screen.
    void SetDimensions( uint16_t Left, uint16_t Top, uint16_t Width, uint16_t Height)
    {
        left = Left;
        top = Top;
        width = Width;
        height = Height;

        remembered = false;
    }

    // Set some display options.
    void SetOptions( Options UseOptions)
    {
    	options = UseOptions;

    	remembered = false;
    }

    // Set value without displaying it.
    void SetValue( bool Ticked)
    {
    	if( remembered == true)
	    {
    		// If we have the same values as last time, don't display anything.
	    	if( ticked == Ticked)
		    {
    			// No changes.
	    		return;
		    }
    	}

    	ticked = Ticked;
	    remembered = false;
    }

    // This will update the view to reflect the given values.
    void Display( bool Ticked)
    {
    	if( remembered == true)
	    {
    		// If we have the same values as last time, don't change anything.
	    	if( ticked == Ticked)
		    {
    			// No changes.
	    		return;
		    }
	    }

    	// Remember values.
	    ticked = Ticked;
	    remembered = false;

    	Display();
    }

    void Display( void)
    {
        if( remembered == true)
        {
            // No changes.
            return;
        }

        // Draw.
        // We need a minimum space for display.
        if( height < 3)
        {
            return;
        }

        // Draw box frame.
        GLOBAL.SetupDisplay.DrawRect( left, top, height, height, ForegroundColor, LCD::RO_Rounded);
        GLOBAL.SetupDisplay.DrawRect( left + 1, top + 1, height - 2, height - 2, ForegroundColor, LCD::RO_Boxed);

        LCD_65K_RGB::Color Color;

        // Draw box inner frame.
        if( ticked)
        {
            Color = FillColor;
        }
        else
        {
            Color = BackgroundColor;
        }

        GLOBAL.SetupDisplay.DrawLine( left + 4, top + 3, left + height - 4, top + height - 5, Color);
        GLOBAL.SetupDisplay.DrawLine( left + 3, top + 3, left + height - 4, top + height - 4, Color);
        GLOBAL.SetupDisplay.DrawLine( left + 3, top + 4, left + height - 5, top + height - 4, Color);
        GLOBAL.SetupDisplay.DrawLine( left + height - 5, top + 3, left + 3, top + height - 5, Color);
        GLOBAL.SetupDisplay.DrawLine( left + height - 4, top + 3, left + 3, top + height - 4, Color);
        GLOBAL.SetupDisplay.DrawLine( left + height - 4, top + 4, left + 4, top + height - 4, Color);

        //	GLOBAL.SetupDisplay.FillRect( left + 3, top + 3, height - 6, height - 6, Color);

        remembered = true;
    }

    // Clear box content.
    void Clear( void)
    {
        //	GLOBAL.SetupDisplay.DrawRect( left, top, width, height, LCD_65K_RGB::Green);

        remembered = false;

        // Draw.
        // We need a minimum space for display.
        if( height < 7)
        {
            return;
        }

        // Draw box frame.
        GLOBAL.SetupDisplay.DrawRect( left, top, height, height, BackgroundColor,
                                      LCD_65K_RGB::RO_Rounded);

        // Draw box inner frame.
        GLOBAL.SetupDisplay.FillRect( left + 1, top + 1, height - 2, height - 2, BackgroundColor);
    }
};
