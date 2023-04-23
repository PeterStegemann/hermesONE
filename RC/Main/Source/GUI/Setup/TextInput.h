// Copyright 2008 Peter Stegemann

#pragma once

#include "AVR/Source/Types.h"
#include "AVR/Source/LCD/LCD_65K_RGB.h"

#define GUI_SETUP_TEXTINPUT_MAXIMUM_SIZE		30

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

		bool isSpecial( char Character);
		void skipCharacter( int16_t Range);
		void clearCursor( void);
		void displayCursor( void);

	public:
		LCD_65K_RGB::Color ForegroundColor;
		LCD_65K_RGB::Color BackgroundColor;
		LCD_65K_RGB::Color CursorColor;

	public:
		GUI_Setup_TextInput( void);

		// Set the location of the map on the screen.
		void SetDimensions( uint16_t Left, uint16_t Top);

		// Set some display options.
		void SetOptions( Options UseOptions);

		// Get text.
		const char* GetText( void);

		// Read in new text from user.
		void ReadText( const char* Text, uint8_t Size);

		// This will update the view to reflect the current value.
		void Display( void);

		void Clear( void);
};
