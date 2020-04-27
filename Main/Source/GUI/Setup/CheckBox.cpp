// Copyright 2008 Peter Stegemann

#include "CheckBox.h"

#include "Defines.h"

GUI_Setup_CheckBox::GUI_Setup_CheckBox( void)
				  : left( 0)
				  , top( 0)
				  , width( 0)
				  , height( 0)
				  , options( O_None)
				  , fontId( GUI_SETUP_MAIN_FONT)
				  , remembered( false)
				  , ticked( false)
				  , ForegroundColor( LCD_65K_RGB::C_White)
				  , BackgroundColor( LCD_65K_RGB::C_Black)
				  , FillColor( LCD_65K_RGB::C_WarmYellow)
{
}

void GUI_Setup_CheckBox::SetDimensions( uint16_t Left, uint16_t Top, uint16_t Width,
									    uint16_t Height)
{	
	left = Left;
	top = Top;
	width = Width;
	height = Height;

	remembered = false;
}

void GUI_Setup_CheckBox::SetOptions( Options UseOptions)
{
	options = UseOptions;

	remembered = false;
}

void GUI_Setup_CheckBox::SetValue( bool Ticked)
{
	if( remembered == true)
	{
		// If we have the same values as last time, don't display anything.
		if( ticked == Ticked)
		{
			// No changes.
			return;
		}
	}

	ticked = Ticked;
	remembered = false;
}

void GUI_Setup_CheckBox::Display( bool Ticked)
{
	if( remembered == true)
	{
		// If we have the same values as last time, don't display anything.
		if( ticked == Ticked)
		{
			// No changes.
			return;
		}
	}

	// Remember values.
	ticked = Ticked;
	remembered = false;

	Display();
}

void GUI_Setup_CheckBox::Display( void)
{
	if( remembered == true)
	{
		// No changes.
		return;
	}

	// Draw.
	// We need a minimum space for display.
	if( height < 3)
	{
		return;
	}

	// Draw box frame.
	GLOBAL.SetupDisplay.DrawRect( left, top, height, height, ForegroundColor, LCD::RO_Rounded);
	GLOBAL.SetupDisplay.DrawRect( left + 1, top + 1, height - 2, height - 2, ForegroundColor,
							  LCD::RO_Boxed);

	LCD_65K_RGB::Color Color;

	// Draw box inner frame.
	if( ticked)
	{
		Color = FillColor;
	}
	else
	{
		Color = BackgroundColor;
	}

	GLOBAL.SetupDisplay.DrawLine( left + 4, top + 3, left + height - 4, top + height - 5, Color);
	GLOBAL.SetupDisplay.DrawLine( left + 3, top + 3, left + height - 4, top + height - 4, Color);
	GLOBAL.SetupDisplay.DrawLine( left + 3, top + 4, left + height - 5, top + height - 4, Color);
	GLOBAL.SetupDisplay.DrawLine( left + height - 5, top + 3, left + 3, top + height - 5, Color);
	GLOBAL.SetupDisplay.DrawLine( left + height - 4, top + 3, left + 3, top + height - 4, Color);
	GLOBAL.SetupDisplay.DrawLine( left + height - 4, top + 4, left + 4, top + height - 4, Color);

//	GLOBAL.SetupDisplay.FillRect( left + 3, top + 3, height - 6, height - 6, Color);

	remembered = true;
}

void GUI_Setup_CheckBox::Clear( void)
{
//	GLOBAL.SetupDisplay.DrawRect( left, top, width, height, LCD_65K_RGB::Green);

	remembered = false;

	// Draw.
	// We need a minimum space for display.
	if( height < 7)
	{
		return;
	}

	// Draw box frame.
	GLOBAL.SetupDisplay.DrawRect( left, top, height, height, BackgroundColor,
								  LCD_65K_RGB::RO_Rounded);

	// Draw box inner frame.
	GLOBAL.SetupDisplay.FillRect( left + 1, top + 1, height - 2, height - 2, BackgroundColor);
}
