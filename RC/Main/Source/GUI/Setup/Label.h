// Copyright 2007 Peter Stegemann

#pragma once

#include "AVR/Source/Types.h"
#include "AVR/Source/LCD/LCD_65K_RGB.h"

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

		FONT::FontId fontId;

	protected:
		// Marks wether we have remembered values from last draw.
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
		GUI_Setup_Label( void);

		// Set the location of the label on the screen.
		void SetDimensions( uint16_t Left, uint16_t Top);

		// Set some display options.
		void SetOptions( Options UseOptions)
		{
			options = UseOptions;
		}

		void SetFont( FONT::FontId UseFontId);

		void SetInverted( bool Inverted);

		// Set text.
		void SetText( const char* Text);
		void SetText_P( const flash_char* Text);

		// Set value.
		void SetValue( int32_t Value);

		// Get guessed width of the label.
		uint16_t GetWidth( void);

		// This will update the view to reflect the given values. Be careful, no copy will be made.
		void Display( const char* Text);
		void Display_P( const flash_char* Text);
		void Display( void);

		// Clear label content.
		void Clear( void);
};
