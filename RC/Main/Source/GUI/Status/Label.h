// Copyright 2012 Peter Stegemann

#pragma once

#include "AVR/Source/Types.h"
#include "AVR/Source/LCD/LCD_DOG.h"

class GUI_Status_Label
{
	private:
		uint16_t left, top;

		FONT::FontId fontId;

	protected:
		// Marks whether we have remembered values from last draw.
		bool remembered;

		// Remembered values.
		const char* text;
		const flash_char* text_P;
		int32_t value;

	public:
		GUI_Status_Label( void);

		// Set the location of the label on the screen.
		void SetDimensions( uint16_t Left, uint16_t Top);

		void SetFont( FONT::FontId UseFontId);

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
