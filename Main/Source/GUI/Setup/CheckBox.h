// Copyright 2008 Peter Stegemann

#pragma once

#include "AVR/Source/LCD/LCD_65K_RGB.h"

class GUI_Setup_CheckBox
{
	public:
		enum Options
		{
			O_None				= 0b00000000,
			O_Crossed			= 0b00000000,
			O_Ticked			= 0b00000001
		};

	private:
		uint16_t left, top, width, height;

		Options options;

		FONT::FontId fontId;

		// Marks wether we have remembered values from last draw.
		bool remembered;
		// Remembered values.
		bool ticked;

	public:
		LCD_65K_RGB::Color ForegroundColor;
		LCD_65K_RGB::Color BackgroundColor;
		LCD_65K_RGB::Color FillColor;

	public:
		GUI_Setup_CheckBox( void);

		// Set the location of the box on the screen.
		void SetDimensions( uint16_t Left, uint16_t Top, uint16_t Width, uint16_t Height);

		// Set some display options.
		void SetOptions( Options UseOptions);

		// Set value without displaying it.
		void SetValue( bool Ticked);

		// This will update the view to reflect the given values.
		void Display( bool Ticked);
		void Display( void);

		// Clear box content.
		void Clear( void);
};
