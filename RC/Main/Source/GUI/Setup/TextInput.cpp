// Copyright 2008 Peter Stegemann

#include "TextInput.h"

#include "Defines.h"
#include "Main/Setup.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"

#include <ctype.h>
#include <string.h>

#define STATE_COUNT		100

// Character sets with room for special characters at the end.
static const char GUI_Setup_NumericCharacterSet[] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '+',
	FONT::C_SpecialExit, FONT::C_SpecialBackspace, FONT::C_SpecialDelete, FONT::C_SpecialInsert,
	FONT::C_SpecialLeft, FONT::C_SpecialRight
};

static const char GUI_Setup_AlphaNumericCharacterSet[] =
{
	' ', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
	's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-',
	'+', '_', '/', '&', '%', '?', '=',
	FONT::C_SpecialExit, FONT::C_SpecialBackspace, FONT::C_SpecialDelete, FONT::C_SpecialInsert,
	FONT::C_SpecialLeft, FONT::C_SpecialRight
};

static const char GUI_Setup_FullCharacterSet[] =
{
	' ', '!', '"', '#', '$', '%', '&', '\'', '(', ')', '*', '+', ',', '-', '.', '/', '0', '1', '2',
	'3', '4', '5', '6', '7', '8', '9', ':', ';', '<', '=', '>', '?', '@', 'A', 'B', 'C', 'D', 'E',
	'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 
	'Y', 'Z', '[', '\\', ']', '^', '_', '`', 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k',
	'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '{', '|', '}', '~',
	FONT::C_SpecialExit, FONT::C_SpecialBackspace, FONT::C_SpecialDelete, FONT::C_SpecialInsert,
	FONT::C_SpecialLeft, FONT::C_SpecialRight
};

GUI_Setup_TextInput::GUI_Setup_TextInput( void)
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

bool GUI_Setup_TextInput::isSpecial( char Character)
{
	return(( Character >= FONT::C_FirstSpecial) && ( Character <= FONT::C_LastSpecial));
}

void GUI_Setup_TextInput::skipCharacter( int16_t Range)
{
	const char* CharacterSet = GUI_Setup_FullCharacterSet;
	int16_t CharacterSetSize = sizeof( GUI_Setup_FullCharacterSet);

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

	// Get position of current character.
	const char* Current = strchr( CharacterSet, currentCharacter);

	if( Current == NULL)
	{
		// Try gokal.
		Current = strchr( CharacterSet, tolower( currentCharacter));
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

	currentCharacter = CharacterSet[ Index];
}

void GUI_Setup_TextInput::clearCursor( void)
{
	const FONT_Type* Font = FONT::GetFont( fontId);

	GLOBAL.SetupDisplay.PrintFormat_P( left + ( cursorPosition * Font->CellWidth), top, fontId,
									   ForegroundColor, BackgroundColor, LCD_65K_RGB::PO_Fixed,
									   Text::CharacterFormat, text[ cursorPosition]);
}

void GUI_Setup_TextInput::displayCursor( void)
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
									   UseForegroundColor, UseBackgroundColor,
									   LCD_65K_RGB::PO_Fixed, Text::CharacterFormat,
									   currentCharacter);
}

void GUI_Setup_TextInput::SetDimensions( uint16_t Left, uint16_t Top)
{	
	left = Left;
	top = Top;
}

void GUI_Setup_TextInput::SetOptions( Options UseOptions)
{
	options = UseOptions;
}

const char* GUI_Setup_TextInput::GetText( void)
{
	return( text);
}

void GUI_Setup_TextInput::ReadText( const char* Text, uint8_t Size)
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

	uint8_t BlinkCount = 0;
	blinkState = true;

	Display();

	uint8_t StateCount = STATE_COUNT;

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

						strncpy( &text[ cursorPosition], &text[ cursorPosition + 1],
						         size + 1 - cursorPosition);
					}
				}
				break;

				case FONT::C_SpecialDelete :
				{
					if( cursorPosition < size)
					{
						strncpy( &text[ cursorPosition], &text[ cursorPosition + 1],
						         size + 1 - cursorPosition);
					}
				}
				break;

				case FONT::C_SpecialInsert :
				{
					if(( cursorPosition + 1) < size)
					{
						memmove( &text[ cursorPosition + 1], &text[ cursorPosition],
						         size - ( cursorPosition + 1));

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
				StateCount = STATE_COUNT;

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
			StateCount = STATE_COUNT;
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

void GUI_Setup_TextInput::Display( void)
{
	Clear();

	GLOBAL.SetupDisplay.Print( left, top, fontId, ForegroundColor, BackgroundColor,
							   LCD_65K_RGB::PO_Fixed, text);

	displayCursor();
}

void GUI_Setup_TextInput::Clear( void)
{
	const FONT_Type* Font = FONT::GetFont( fontId);
	
	GLOBAL.SetupDisplay.FillRect( left, top, size * Font->CellWidth, Font->CellHeight,
								  BackgroundColor);
}
