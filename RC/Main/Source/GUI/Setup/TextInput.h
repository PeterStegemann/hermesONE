// Copyright 2008 Peter Stegemann

#pragma once

#include "Defines.h"
#include "Select.h"

#include "AVR/Source/Types.h"
#include "AVR/Source/LCD/65K_RGB.h"

#include <ctype.h>

#define GUI_SETUP_TEXTINPUT_MAXIMUM_SIZE		30
#define GUI_SETUP_TEXTINPUT_STATE_COUNT		    100

// Character sets with room for special characters at the end.
static const flash_char GUI_Setup_NumericCharacterSet[] PROGMEM =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '+',
	avr::font::C_SpecialExit, avr::font::C_SpecialBackspace, avr::font::C_SpecialDelete, avr::font::C_SpecialInsert,
	avr::font::C_SpecialLeft, avr::font::C_SpecialRight
};

static const flash_char GUI_Setup_AlphaNumericCharacterSet[] PROGMEM =
{
	' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '+', '_', '/', '&', '%', '?', '=',
	avr::font::C_SpecialExit, avr::font::C_SpecialBackspace, avr::font::C_SpecialDelete, avr::font::C_SpecialInsert,
	avr::font::C_SpecialLeft, avr::font::C_SpecialRight
};

static const flash_char GUI_Setup_FullCharacterSet[] PROGMEM =
{
	' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2', '3', '4', '5', '6',
	'7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M',
	'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd',
	'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{',
	'|', '}', '~',
	avr::font::C_SpecialExit, avr::font::C_SpecialBackspace, avr::font::C_SpecialDelete, avr::font::C_SpecialInsert,
	avr::font::C_SpecialLeft, avr::font::C_SpecialRight
};

class GUI_Setup_TextInput
{
  public:
    enum Options
    {
        O_None					= 0b00000000,
        O_LimitNumeric			= 0b00000001,
        O_LimitAlphaNumeric		= 0b00000010
    };

  private:
    Input_Service* inputService;

    GUI_Setup_Select select;

    uint16_t left, top;

    Options options;

    avr::font::FontId fontId;

    LCD_65K_RGB::Color foregroundColor;
    LCD_65K_RGB::Color backgroundColor;
    LCD_65K_RGB::Color cursorColor;

    // The current version of the text.
    char text[ GUI_SETUP_TEXTINPUT_MAXIMUM_SIZE + 1];

    uint8_t size;
    uint8_t cursorPosition;
    char currentCharacter;

    bool blinkState;
    uint8_t stateCount;

    bool isSpecial( char Character)
    {
        return(( Character >= avr::font::C_FirstSpecial) && ( Character <= avr::font::C_LastSpecial));
    }

    void skipCharacter( int16_t Range)
    {
        const flash_char* CharacterSet;
        int16_t CharacterSetSize;

        if( options & O_LimitNumeric)
        {
            CharacterSet = GUI_Setup_NumericCharacterSet;
            CharacterSetSize = sizeof( GUI_Setup_NumericCharacterSet);
        }
        else if( options & O_LimitAlphaNumeric)
        {
            CharacterSet = GUI_Setup_AlphaNumericCharacterSet;
            CharacterSetSize = sizeof( GUI_Setup_AlphaNumericCharacterSet);
        }
        else
        {
            CharacterSet = GUI_Setup_FullCharacterSet;
            CharacterSetSize = sizeof( GUI_Setup_FullCharacterSet);
        }

        // Get position of current character.
        const flash_char* Current = strchr_P( CharacterSet, currentCharacter);

        if( Current == NULL)
        {
            // Try gokal.
            Current = strchr_P( CharacterSet, tolower( currentCharacter));
        }

        int16_t Index = 0;

        if( Current != NULL)
        {
            Index = Current - CharacterSet;
        }

        Index += Range;

        // Move character into valid range.
        while( Index < 0)
        {
            Index += CharacterSetSize;
        }

        Index %= CharacterSetSize;

        currentCharacter = pgm_read_byte( &( CharacterSet[ Index]));
    }

    void clearCursor( void)
    {
        const avr::font::Type* Font = avr::font::Font::Get( fontId);

        GLOBAL.SetupDisplay.PrintFormat_P
        (
            left + ( cursorPosition * Font->GetCellWidth()), top, fontId, foregroundColor, backgroundColor,
            LCD_65K_RGB::PO_Fixed, Text::CharacterFormat, text[ cursorPosition]
        );
    }

    void displayCursor( void)
    {
        LCD_65K_RGB::Color ForegroundColor = foregroundColor;
        LCD_65K_RGB::Color BackgroundColor = backgroundColor;

        if( isSpecial( currentCharacter))
        {
            // Set special color.
            ForegroundColor = cursorColor;
        }

        if( blinkState)
        {
            // Switch colors.
            BackgroundColor = ForegroundColor;
            ForegroundColor = backgroundColor;
        }

        const avr::font::Type* Font = avr::font::Font::Get( fontId);

        GLOBAL.SetupDisplay.PrintFormat_P
        (
            left + ( cursorPosition * Font->GetCellWidth()), top, fontId, ForegroundColor, BackgroundColor,
            LCD_65K_RGB::PO_Fixed, Text::CharacterFormat, currentCharacter
        );
    }

    /**
     * Returns false if the user selected "Exit"
     */
    bool handleButton( void)
    {
        // If this is a special character, act accordingly.
        switch( currentCharacter)
        {
            case avr::font::C_SpecialExit :
            {
                clearCursor();

                return( false);
            }
            break;

            case avr::font::C_SpecialBackspace :
            {
                if( cursorPosition > 0)
                {
                    cursorPosition--;

                    strncpy( &text[ cursorPosition], &text[ cursorPosition + 1], size + 1 - cursorPosition);
                }
            }
            break;

            case avr::font::C_SpecialDelete :
            {
                if( cursorPosition < size)
                {
                    strncpy( &text[ cursorPosition], &text[ cursorPosition + 1], size + 1 - cursorPosition);
                }
            }
            break;

            case avr::font::C_SpecialInsert :
            {
                if(( cursorPosition + 1) < size)
                {
                    memmove( &text[ cursorPosition + 1], &text[ cursorPosition], size - ( cursorPosition + 1));

                    text[ cursorPosition] = ' ';
                }
            }
            break;

            case avr::font::C_SpecialLeft :
            {
                // Skip left.
                // Don't go beyond the string beginning.
                if( cursorPosition > 0)
                {
                    cursorPosition--;
                }
            }
            break;

            case avr::font::C_SpecialRight :
            {
                // Skip next.
                // Don't go beyond the string size.
                if( cursorPosition < ( size - 1))
                {
                    if( text[ cursorPosition + 1] != 0)
                    {
                        cursorPosition++;
                    }
                }
            }
            break;

            default :
            {
                text[ cursorPosition] = currentCharacter;

                // Skip next.
                // Don't go beyond the string size.
                if( cursorPosition < ( size - 1))
                {
                    cursorPosition++;

                    currentCharacter = text[ cursorPosition];
                }
            }
            break;
        }

        // Set initial character?
        if( currentCharacter == 0)
        {
            if( cursorPosition == 0)
            {
                currentCharacter = 'A';
            }
            else
            {
                currentCharacter = text[ cursorPosition - 1];
            }

            text[ cursorPosition + 1] = 0;
        }

        return( true);
    }

    void handleCurrentButton( void)
    {
        if( stateCount > 0)
        {
            stateCount--;
        }
        else
        {
            // Long press, flip size.
            stateCount = GUI_SETUP_TEXTINPUT_STATE_COUNT;

            if( isupper( currentCharacter))
            {
                currentCharacter = tolower( currentCharacter);
            }
            else
            {
                currentCharacter = toupper( currentCharacter);
            }

            displayCursor();
        }
    }

    void blink( uint16_t* Millis)
    {
        if( select.Blink( Millis, &blinkState))
        {
            displayCursor();
        }
    }

  public:
    GUI_Setup_TextInput( Input_Service* InputService, Interrupt_Service* InterruptService)
        : inputService( InputService)
        , select( InputService, InterruptService)
        , left( 0)
        , top( 0)
        , options( O_None)
        , fontId( GUI_SETUP_MAIN_FONT)
        , foregroundColor( LCD_65K_RGB::C_White)
        , backgroundColor( LCD_65K_RGB::C_Black)
        , cursorColor( LCD_65K_RGB::C_WarmYellow)
        , size( sizeof( text) - 1)
        , cursorPosition( 0)
        , currentCharacter( 'A')
        , blinkState( false)
    {
    	text[ 0] = 0;
    }

    // Set the location of the map on the screen.
    void SetDimensions( uint16_t Left, uint16_t Top)
    {
    	left = Left;
	    top = Top;
    }

    // Set some display options.
    void SetOptions( Options Options)
    {
    	options = Options;
    }

    // Get text.
    const char* GetText( void)
    {
	    return( text);
    }

    // Read in new text from user.
    void ReadText( const char* Text, uint8_t Size)
    {
        if( Size > ( sizeof( text) - 1))
        {
            size = sizeof( text) - 1;
        }
        else
        {
            size = Size;
        }

        if( Text != NULL)
        {
            strncpy( text, Text, size + 1);
            text[ size] = 0;
        }

        cursorPosition = 0;
        currentCharacter = text[ cursorPosition];

        if( currentCharacter == 0)
        {
            if( cursorPosition == 0)
            {
                currentCharacter = 'A';
            }
            else
            {
                currentCharacter = text[ cursorPosition - 1];
            }

            text[ cursorPosition + 1] = 0;
        }

        uint16_t Millis = 0;
        stateCount = GUI_SETUP_TEXTINPUT_STATE_COUNT;
        blinkState = true;

        Display();

        while( true)
        {
            // Try keys.
            int8_t RotarySelect;
            uint8_t RotaryButton;
            bool RotaryCurrentButton;

            inputService->GetRotary( &RotarySelect, &RotaryButton, &RotaryCurrentButton);

            if( RotarySelect != 0)
            {
                skipCharacter( RotarySelect);

                Display();
            }

            if( RotaryButton > 0)
            {
                if( handleButton() == false)
                {
                    break;
                }

                Display();
            }

            if( RotaryCurrentButton)
            {
                handleCurrentButton();
            }
            else
            {
                stateCount = GUI_SETUP_TEXTINPUT_STATE_COUNT;
            }

            blink( &Millis);

            avr::Utility::Pause( 5);
        }
    }

    // This will update the view to reflect the current value.
    void Display( void)
    {
        Clear();

        GLOBAL.SetupDisplay.Print( left, top, fontId, foregroundColor, backgroundColor, LCD_65K_RGB::PO_Fixed, text);

        displayCursor();
    }

    void Clear( void)
    {
        const avr::font::Type* Font = avr::font::Font::Get( fontId);

        GLOBAL.SetupDisplay.FillRect( left, top, size * Font->GetCellWidth(), Font->GetCellHeight(), backgroundColor);
    }
};
