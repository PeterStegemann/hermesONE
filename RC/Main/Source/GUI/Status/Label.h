// Copyright 2012 Peter Stegemann

#pragma once

#include "Defines.h"

#include "AVR/Source/Types.h"
#include "AVR/Source/LCD/DOG/DOG.h"

class GUI_Status_Label
{
  private:
    LCD_DOG* statusDisplay;

    uint16_t left, top;

    avr::font::FontId fontId;

  protected:
    // Marks whether we have remembered values from last draw.
    bool remembered;

    // Remembered values.
    const char* text;
    const flash_char* text_P;
    int32_t value;

  public:
    GUI_Status_Label( LCD_DOG* StatusDisplay)
        : statusDisplay( StatusDisplay)
        , left( 0)
        , top( 0)
        , fontId( GUI_STATUS_MAIN_FONT)
        , remembered( false)
        , text( NULL)
        , text_P( NULL)
        , value( 0)
    {
    }

    // Set the location of the label on the screen.
    void SetDimensions( uint16_t Left, uint16_t Top)
    {
	    left = Left;
    	top = Top;
    }

    void SetFont( avr::font::FontId UseFontId)
    {
    	fontId = UseFontId;
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
    		if( text != NULL)
	    	{
		    	statusDisplay->PrintFormat( left, top, fontId, text, value);
		    }
    		else
	    	{
		    	statusDisplay->PrintFormat_P( left, top, fontId, text_P, value);
		    }
    	}

    	remembered = true;
    }

    // Clear label content.
	void Clear( void)
    {
        if( text != NULL)
        {
            statusDisplay->PrintFormat( left, top, fontId, text, value);
        }
        else if( text_P != NULL)
        {
            statusDisplay->PrintFormat_P( left, top, fontId, text_P, value);
        }

        remembered = false;
    }
};
