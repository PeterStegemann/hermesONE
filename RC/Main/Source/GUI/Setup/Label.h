// Copyright 2007 Peter Stegemann

#pragma once

#include "Defines.h"

#include "Main/Global.h"

#include "AVR/Source/Types.h"
#include "AVR/Source/LCD/65K_RGB.h"

class GUI_Setup_Label
{
  public:
    enum Options
    {
        O_None				= 0b00000000,
        O_Fixed				= 0b00000000,
        O_Proportional		= 0b00000001
    };

  private:
    uint16_t left, top;

    Options options;

    avr::font::FontId fontId;

  protected:
    // Marks whether we have remembered values from last draw.
    bool remembered;

    // Remembered values.
    const char* text;
    const flash_char* text_P;
    int32_t value;
    bool inverted;

  public:
    LCD_65K_RGB::Color ForegroundColor;
    LCD_65K_RGB::Color BackgroundColor;

  public:
    GUI_Setup_Label( void)
        : left( 0)
        , top( 0)
        , options( O_Proportional)
        , fontId( GUI_SETUP_MAIN_FONT)
        , remembered( false)
        , text( NULL)
        , text_P( NULL)
        , value( 0)
        , inverted( false)
        , ForegroundColor( LCD_65K_RGB::C_White)
        , BackgroundColor( LCD_65K_RGB::C_Black)
    {
    }

    // Set the location of the label on the screen.
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

    void SetFont( avr::font::FontId UseFontId)
    {
	    fontId = UseFontId;
    }

    void SetInverted( bool Inverted)
    {
    	if( inverted == Inverted)
	    {
    		return;
	    }

    	inverted = Inverted;

    	remembered = false;
    }

    // Set text.
    void SetText( const char* Text)
    {
    	if( remembered == true)
	    {
    		// If we have the same values as last time, don't display anything.
	    	if( text == Text)
		    {
    			// No changes.
	    		return;
		    }
    	}

    	text = Text;
	    text_P = NULL;

    	remembered = false;
    }

    void SetText_P( const flash_char* Text)
    {
    	if( remembered == true)
	    {
    		// If we have the same values as last time, don't display anything.
	    	if( text_P == Text)
		    {
    			// No changes.
	    		return;
		    }
    	}

    	text_P = Text;
	    text = NULL;

    	remembered = false;
    }

    // Set value.
    void SetValue( int32_t Value)
    {
        if( remembered == true)
        {
            // If we have the same values as last time, don't display anything.
            if( value == Value)
            {
                // No changes.
                return;
            }
        }

        value = Value;

        remembered = false;
    }

    // Get guessed width of the label.
    uint16_t GetWidth( void)
    {
        if(( text == NULL) && ( text_P == NULL))
        {
            return( 0);
        }

        const avr::font::Type* Font = avr::font::Font::Get( fontId);

        if( text != NULL)
        {
            return( strlen( text) * Font->GetCellWidth());
        }
        else
        {
            return( strlen_P( text_P) * Font->GetCellWidth());
        }
    }

    // This will update the view to reflect the given values. Be careful, no copy will be made.
    void Display( const char* Text)
    {
        if( remembered == true)
        {
            // If we have the same values as last time, don't display anything.
            if( text == Text)
            {
                // No changes.
                return;
            }
        }

        // Remember values.
        text = Text;
        text_P = NULL;

        Display();
    }

    void Display_P( const flash_char* Text)
    {
        if( remembered == true)
        {
            // If we have the same values as last time, don't display anything.
            if( text_P == Text)
            {
                // No changes.
                return;
            }
        }

        // Remember values.
        text_P = Text;
        text = NULL;

        Display();
    }

    void Display( void)
    {
        if(( text != NULL) || ( text_P != NULL))
        {
            LCD_65K_RGB::PrintOptions PrintOptions;

            if( options & O_Proportional)
            {
                PrintOptions = LCD::PO_Proportional;
            }
            else
            {
                PrintOptions = LCD_65K_RGB::PO_Fixed;
            }

            if( inverted == false)
            {
                if( text != NULL)
                {
                    GLOBAL.SetupDisplay.PrintFormat
                    (
                        left, top, fontId, ForegroundColor, BackgroundColor, PrintOptions, text, value
                    );
                }
                else
                {
                    GLOBAL.SetupDisplay.PrintFormat_P
                    (
                        left, top, fontId, ForegroundColor, BackgroundColor, PrintOptions, text_P, value
                    );
                }
            }
            else
            {
                if( text != NULL)
                {
                    GLOBAL.SetupDisplay.PrintFormat
                    (
                        left, top, fontId, BackgroundColor, ForegroundColor, PrintOptions, text, value
                    );
                }
                else
                {
                    GLOBAL.SetupDisplay.PrintFormat_P
                    (
                        left, top, fontId, BackgroundColor, ForegroundColor, PrintOptions, text_P, value
                    );
                }
            }
        }

        remembered = true;
    }

    // Clear label content.
    void Clear( void)
    {
        LCD_65K_RGB::PrintOptions PrintOptions;

        if( options & O_Proportional)
        {
            PrintOptions = LCD::PO_Proportional;
        }
        else
        {
            PrintOptions = LCD_65K_RGB::PO_Fixed;
        }

        if( text != NULL)
        {
            GLOBAL.SetupDisplay.PrintFormat
            (
                left, top, fontId, BackgroundColor, BackgroundColor, PrintOptions, text, value
            );
        }
        else if( text_P != NULL)
        {
            GLOBAL.SetupDisplay.PrintFormat_P
            (
                left, top, fontId, BackgroundColor, BackgroundColor, PrintOptions, text_P, value
            );
        }

        remembered = false;
    }
};
