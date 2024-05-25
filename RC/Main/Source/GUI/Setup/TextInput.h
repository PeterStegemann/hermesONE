// Copyright 2008 Peter Stegemann

#pragma once

#include "Defines.h"

#include "AVR/Source/Types.h"
#include "AVR/Source/LCD/LCD_65K_RGB.h"

#include <ctype.h>

#define GUI_SETUP_TEXTINPUT_MAXIMUM_SIZE		30
#define GUI_SETUP_TEXTINPUT_STATE_COUNT		    100

// Character sets with room for special characters at the end.
static const flash_char GUI_Setup_NumericCharacterSet[] PROGMEM =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '+',
	FONT::C_SpecialExit, FONT::C_SpecialBackspace, FONT::C_SpecialDelete, FONT::C_SpecialInsert,
	FONT::C_SpecialLeft, FONT::C_SpecialRight
};

static const flash_char GUI_Setup_AlphaNumericCharacterSet[] PROGMEM =
{
	' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
	's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-',
	'+', '_', '/', '&', '%', '?', '=',
	FONT::C_SpecialExit, FONT::C_SpecialBackspace, FONT::C_SpecialDelete, FONT::C_SpecialInsert,
	FONT::C_SpecialLeft, FONT::C_SpecialRight
};

static const flash_char GUI_Setup_FullCharacterSet[] PROGMEM =
{
	' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2',
	'3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E',
	'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
	'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~',
	FONT::C_SpecialExit, FONT::C_SpecialBackspace, FONT::C_SpecialDelete, FONT::C_SpecialInsert,
	FONT::C_SpecialLeft, FONT::C_SpecialRight
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
    uint16_t left, top;

    Options options;

    FONT::FontId fontId;

    // The current version of the text.
    char text[ GUI_SETUP_TEXTINPUT_MAXIMUM_SIZE + 1];

    uint8_t size;
    uint8_t cursorPosition;
    char currentCharacter;

    bool blinkState;

    bool isSpecial( char Character)
    {
        return(( Character >= FONT::C_FirstSpecial) && ( Character <= FONT::C_LastSpecial));
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
        const FONT_Type* Font = FONT::GetFont( fontId);

        GLOBAL.SetupDisplay.PrintFormat_P( left + ( cursorPosition * Font->CellWidth), top, fontId,
            ForegroundColor, BackgroundColor, LCD_65K_RGB::PO_Fixed, Text::CharacterFormat, text[ cursorPosition]);
    }

    void displayCursor( void)
    {
        LCD_65K_RGB::Color UseForegroundColor = ForegroundColor;
        LCD_65K_RGB::Color UseBackgroundColor = BackgroundColor;

        if( isSpecial( currentCharacter))
        {
            // Set special color.
            UseForegroundColor = CursorColor;
        }

        if( blinkState)
        {
            // Switch colors.
            UseBackgroundColor = UseForegroundColor;
            UseForegroundColor = BackgroundColor;
        }

        const FONT_Type* Font = FONT::GetFont( fontId);

        GLOBAL.SetupDisplay.PrintFormat_P( left + ( cursorPosition * Font->CellWidth), top, fontId,
            UseForegroundColor, UseBackgroundColor, LCD_65K_RGB::PO_Fixed, Text::CharacterFormat, currentCharacter);
    }

  public:
    LCD_65K_RGB::Color ForegroundColor;
    LCD_65K_RGB::Color BackgroundColor;
    LCD_65K_RGB::Color CursorColor;

  public:
    GUI_Setup_TextInput( void)
        : left( 0)
        , top( 0)
        , options( O_None)
        , fontId( GUI_SETUP_MAIN_FONT)
        , size( sizeof( text) - 1)
        , cursorPosition( 0)
        , currentCharacter( 'A')
        , blinkState( false)
        , ForegroundColor( LCD_65K_RGB::C_White)
        , BackgroundColor( LCD_65K_RGB::C_Black)
        , CursorColor( LCD_65K_RGB::C_WarmYellow)
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
    void SetOptions( Options UseOptions)
    {
    	options = UseOptions;
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

        uint16_t BlinkCount = 0;
        blinkState = true;

        Display();

        uint8_t StateCount = GUI_SETUP_TEXTINPUT_STATE_COUNT;

        while( true)
        {
            // Try keys.
            int8_t RotarySelect;
            uint8_t RotaryButton;
            bool RotaryCurrentButton;

            GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton, &RotaryCurrentButton);

            if( RotarySelect != 0)
            {
                skipCharacter( RotarySelect);

                Display();
            }

            if( RotaryButton > 0)
            {
                // If this is a special character, act accordingly.
                switch( currentCharacter)
                {
                    case FONT::C_SpecialExit :
                    {
                        clearCursor();

                        return;
                    }
                    break;

                    case FONT::C_SpecialBackspace :
                    {
                        if( cursorPosition > 0)
                        {
                            cursorPosition--;

                            strncpy( &text[ cursorPosition], &text[ cursorPosition + 1], size + 1 - cursorPosition);
                        }
                    }
                    break;

                    case FONT::C_SpecialDelete :
                    {
                        if( cursorPosition < size)
                        {
                            strncpy( &text[ cursorPosition], &text[ cursorPosition + 1], size + 1 - cursorPosition);
                        }
                    }
                    break;

                    case FONT::C_SpecialInsert :
                    {
                        if(( cursorPosition + 1) < size)
                        {
                            memmove( &text[ cursorPosition + 1], &text[ cursorPosition], size - ( cursorPosition + 1));

                            text[ cursorPosition] = ' ';
                        }
                    }
                    break;

                    case FONT::C_SpecialLeft :
                    {
                        // Skip left.
                        // Don't go beyond the string beginning.
                        if( cursorPosition > 0)
                        {
                            cursorPosition--;
                        }
                    }
                    break;

                    case FONT::C_SpecialRight :
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

                Display();
            }

            if( RotaryCurrentButton)
            {
                if( StateCount > 0)
                {
                    StateCount--;
                }
                else
                {
                    // Long press, flip size.
                    StateCount = GUI_SETUP_TEXTINPUT_STATE_COUNT;

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
            else
            {
                StateCount = GUI_SETUP_TEXTINPUT_STATE_COUNT;
            }

            // Blink cursor.
            BlinkCount++;

            if( BlinkCount == ( GUI_SETUP_BLINK_DELAY / 2))
            {
                blinkState = false;

                displayCursor();
            }
            else if( BlinkCount == GUI_SETUP_BLINK_DELAY)
            {
                blinkState = true;

                displayCursor();

                BlinkCount = 0;
            }

            UTILITY::Pause( 5);
        }
    }

    // This will update the view to reflect the current value.
    void Display( void)
    {
        Clear();

        GLOBAL.SetupDisplay.Print( left, top, fontId, ForegroundColor, BackgroundColor, LCD_65K_RGB::PO_Fixed, text);

        displayCursor();
    }

    void Clear( void)
    {
        const FONT_Type* Font = FONT::GetFont( fontId);

        GLOBAL.SetupDisplay.FillRect( left, top, size * Font->CellWidth, Font->CellHeight, BackgroundColor);
    }
};
