// Copyright 2012 Peter Stegemann

#include "Label.h"

#include "Defines.h"

#include <string.h>

GUI_Status_Label::GUI_Status_Label( void)
			    : left( 0)
			    , top( 0)
			    , fontId( GUI_STATUS_MAIN_FONT)
			    , remembered( false)
			    , text( NULL)
			    , text_P( NULL)
			    , value( 0)
{
}

void GUI_Status_Label::SetDimensions( uint16_t Left, uint16_t Top)
{	
	left = Left;
	top = Top;
}

void GUI_Status_Label::SetFont( FONT::FontId UseFontId)
{
	fontId = UseFontId;
}

void GUI_Status_Label::SetText( const char* Text)
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

void GUI_Status_Label::SetText_P( const flash_char* Text)
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

void GUI_Status_Label::SetValue( int32_t Value)
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

uint16_t GUI_Status_Label::GetWidth( void)
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

void GUI_Status_Label::Display( const char* Text)
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

void GUI_Status_Label::Display_P( const flash_char* Text)
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

void GUI_Status_Label::Display( void)
{
	if(( text != NULL) || ( text_P != NULL))
	{
		if( text != NULL)
		{
			GLOBAL.StatusDisplay.PrintFormat( left, top, fontId, text, value);
		}
		else
		{
			GLOBAL.StatusDisplay.PrintFormat_P( left, top, fontId, text_P, value);
		}
	}

	remembered = true;
}

void GUI_Status_Label::Clear( void)
{
	if( text != NULL)
	{
		GLOBAL.StatusDisplay.PrintFormat( left, top, fontId, text, value);
	}
	else if( text_P != NULL)
	{
		GLOBAL.StatusDisplay.PrintFormat_P( left, top, fontId, text_P, value);
	}

	remembered = false;
}
