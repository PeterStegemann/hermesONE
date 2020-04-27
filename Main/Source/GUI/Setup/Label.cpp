// Copyright 2007 Peter Stegemann

#include "Label.h"

#include "Defines.h"

#include <string.h>

GUI_Setup_Label::GUI_Setup_Label( void)
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

void GUI_Setup_Label::SetDimensions( uint16_t Left, uint16_t Top)
{	
	left = Left;
	top = Top;
}

void GUI_Setup_Label::SetOptions( Options UseOptions)
{
	options = UseOptions;
}

void GUI_Setup_Label::SetFont( FONT::FontId UseFontId)
{
	fontId = UseFontId;
}

void GUI_Setup_Label::SetInverted( bool Inverted)
{
	if( inverted == Inverted)
	{
		return;
	}

	inverted = Inverted;

	remembered = false;
}

void GUI_Setup_Label::SetText( const char* Text)
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

void GUI_Setup_Label::SetText_P( const flash_char* Text)
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

void GUI_Setup_Label::SetValue( int32_t Value)
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

uint16_t GUI_Setup_Label::GetWidth( void)
{
	if(( text == NULL) && ( text_P == NULL))
	{
		return( 0);
	}

	const FONT_Type* Font = FONT::GetFont( fontId);

	if( text != NULL)
	{
		return( strlen( text) * Font->CellWidth);
	}
	else
	{
		return( strlen_P( text_P) * Font->CellWidth);
	}
}

void GUI_Setup_Label::Display( const char* Text)
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

void GUI_Setup_Label::Display_P( const flash_char* Text)
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

void GUI_Setup_Label::Display( void)
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
				GLOBAL.SetupDisplay.PrintFormat( left, top, fontId, ForegroundColor,
												 BackgroundColor, PrintOptions, text, value);
			}
			else
			{
				GLOBAL.SetupDisplay.PrintFormat_P( left, top, fontId, ForegroundColor,
												   BackgroundColor, PrintOptions, text_P, value);
			}
		}
		else
		{
			if( text != NULL)
			{
				GLOBAL.SetupDisplay.PrintFormat( left, top, fontId, BackgroundColor,
												 ForegroundColor, PrintOptions, text, value);
			}
			else
			{
				GLOBAL.SetupDisplay.PrintFormat_P( left, top, fontId, BackgroundColor,
											       ForegroundColor, PrintOptions, text_P, value);
			}
		}
	}

	remembered = true;
}

void GUI_Setup_Label::Clear( void)
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
		GLOBAL.SetupDisplay.PrintFormat( left, top, fontId, BackgroundColor, BackgroundColor,
										 PrintOptions, text, value);
	}
	else if( text_P != NULL)
	{
		GLOBAL.SetupDisplay.PrintFormat_P( left, top, fontId, BackgroundColor, BackgroundColor,
										   PrintOptions, text_P, value);
	}

	remembered = false;
}
