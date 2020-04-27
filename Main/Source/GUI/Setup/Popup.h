// Copyright 2008 Peter Stegemann

#pragma once

#include "AVR/Components/Types.h"
#include "AVR/Components/LCD/LCD_65K_RGB.h"

class GUI_Setup_Popup
{
	private:
		FONT::FontId fontId;

		const char* text;
		const flash_char* text_P;
		const flash_char* okText_P;
		const flash_char* cancelText_P;

		uint16_t buttonTop, okLeft, cancelLeft;

	public:
		LCD_65K_RGB::Color ForegroundColor;
		LCD_65K_RGB::Color BackgroundColor;

	public:
		GUI_Setup_Popup( void);

		// Set text.
		void SetText( const char* Text);
		void SetText_P( const flash_char* Text);
		void SetOKText_P( const flash_char* OKText);
		void SetCancelText_P( const flash_char* CancelText);

		// This will show the popup.
		void Show( void);

		// This will get the user input.
		bool Ask( void);
};
