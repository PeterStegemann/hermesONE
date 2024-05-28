// Copyright 2007 Peter Stegemann

#pragma once

#include "Main/Global.h"

#include "AVR/Source/LCD/65K_RGB.h"

class GUI_Setup_Marker
{
  public:
    enum Options
    {
        O_None				= 0b00000000,
        O_LookRight			= 0b00000001,
        O_LookUp			= 0b00000010,
        O_LookLeft			= 0b00000100,
        O_LookDown			= 0b00001000,
        O_Block		    	= 0b00010000
    };

  private:
    uint16_t areaLeft, areaTop, areaRight, areaBottom;

    Options options;

    uint8_t width, height;

    // Marks whether we have remembered values from last draw.
    bool remembered;
    // Current marker position.
    uint16_t left, top;

    // Internal display with a "print" or "clear" mode.
    enum ClearOrPrintMode
    {
        COPM_Clear,
        COPM_Print
    };

    // With print, the marker is shown. With clear, the marker will be removed.
    void display( ClearOrPrintMode ClearOrPrint)
    {
        LCD_65K_RGB::Color DrawColor;

        if( ClearOrPrint == COPM_Print)
        {
            DrawColor = ForegroundColor;
        }
        else
        {
            DrawColor = BackgroundColor;
        }

        if( options & O_Block)
        {
            displayBlock( DrawColor);
        }
        else
        {
            displayArrow( DrawColor);
        }
    }

    void displayBlock( LCD_65K_RGB::Color DrawColor)
    {
        GLOBAL.SetupDisplay.DrawHorizontalLine( left, top - height / 2, width + 2, DrawColor);
        GLOBAL.SetupDisplay.FillRect( left, top - height / 2, width, height, DrawColor);
        GLOBAL.SetupDisplay.DrawHorizontalLine( left, top + height / 2, width + 2, DrawColor);
    }

    void displayArrow( LCD_65K_RGB::Color DrawColor)
    {
        uint16_t DrawLeft = left;
        uint16_t DrawTop = top;
        int16_t Direction;

        if(( options & O_LookUp) || ( options & O_LookLeft))
        {
            Direction = 1;
        }
        else
        {
            if( options & O_LookDown)
            {
                DrawTop += width - 1;
            }
            else
            {
                DrawLeft += width - 1;
            }

            Direction = -1;
        }

        uint16_t DrawSize = 1;

        for( uint8_t Line = 0; Line < width; Line++)
        {
            if(( options & O_LookUp) || ( options & O_LookDown))
            {
                GLOBAL.SetupDisplay.DrawHorizontalLine( DrawLeft, DrawTop, DrawSize, DrawColor);

                if( DrawLeft > areaLeft)
                {
                    DrawLeft--;
                    DrawSize++;
                }

                if(( DrawLeft + DrawSize - 1) < areaRight)
                {
                    DrawSize++;
                }

                DrawTop += Direction;
            }
            else
            {
                GLOBAL.SetupDisplay.DrawVerticalLine( DrawLeft, DrawTop, DrawSize, DrawColor);

                if( DrawTop > areaTop)
                {
                    DrawTop--;
                    DrawSize++;
                }

                if(( DrawTop + DrawSize - 1) < areaBottom)
                {
                    DrawSize++;
                }

                DrawLeft += Direction;
            }
        }
    }

  public:
    LCD_65K_RGB::Color ForegroundColor;
    LCD_65K_RGB::Color BackgroundColor;

  public:
    GUI_Setup_Marker( void)
        : areaLeft( 0)
        , areaTop( 0)
        , options( O_None)
        , width( 5)
        , height( 5)
        , remembered( false)
        , left( 0)
        , top( 0)
        , ForegroundColor( LCD_65K_RGB::C_White)
        , BackgroundColor( LCD_65K_RGB::C_Black)
    {
    	areaRight = GLOBAL.SetupDisplay.GetWidth() - 1;
    	areaBottom = GLOBAL.SetupDisplay.GetHeight() - 1;
    }

    // Set the location of the marker area on the screen.
    void SetArea( uint16_t Left, uint16_t Top, uint16_t Width, uint16_t Height)
    {
    	areaLeft = Left;
	    areaTop = Top;
	    areaRight = areaLeft + Width - 1;
	    areaBottom = areaTop + Height - 1;

    	remembered = false;
    }

    // Set some display options.
    void SetOptions( Options UseOptions)
    {
    	options = UseOptions;

    	remembered = false;
    }

    // Set the size of the marker.
    void SetSize( uint8_t Width)
    {
    	SetSize( Width, Width);
    }

    void SetSize( uint8_t Width, uint8_t Height)
    {
    	width = Width;
    	height = Height;

    	remembered = false;
    }

    // Set the position.
    void SetPosition( uint16_t Left, uint16_t Top)
    {
    	if( remembered)
	    {
    		if(( Left == left) && ( Top == top))
	    	{
    			// No change, no redraw.
	    		return;
		    }
    	}

	    left = Left;
    	top = Top;

    	remembered = false;
    }

    // This will draw the marker.
    void Display( uint16_t Left, uint16_t Top)
    {
        if( remembered == true)
        {
            if(( Left == left) && ( Top == top))
            {
                // No change, no redraw.
                return;
            }

            display( COPM_Clear);
        }

        left = Left;
        top = Top;

        remembered = true;

        display( COPM_Print);
    }

    void Display( void)
    {
        remembered = true;

        display( COPM_Print);
    }

    void Clear( void)
    {
        if( remembered)
        {
            display( COPM_Clear);

            remembered = false;
        }
    }
};
