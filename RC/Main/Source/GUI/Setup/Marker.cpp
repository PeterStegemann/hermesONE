// Copyright 2007 Peter Stegemann

#include "Marker.h"

#include "Main/Setup.h"

#include <string.h>

GUI_Setup_Marker::GUI_Setup_Marker( void)
				: areaLeft( 0)
				, areaTop( 0)
				, options( O_None)
				, size( 5)
				, remembered( false)
				, left( 0)
				, top( 0)
				, ForegroundColor( LCD_65K_RGB::C_White)
				, BackgroundColor( LCD_65K_RGB::C_Black)
{
	areaRight = GLOBAL.SetupDisplay.GetWidth() - 1;
	areaBottom = GLOBAL.SetupDisplay.GetHeight() - 1;
}

void GUI_Setup_Marker::display( ClearOrPrintMode ClearOrPrint)
{
	LCD_65K_RGB::Color DrawColor;

	if( ClearOrPrint == COPM_Print)
	{
		DrawColor = ForegroundColor;
	}
	else
	{
		DrawColor = BackgroundColor;
	}
/*
	GLOBAL.SetupDisplay.FillRect( areaLeft, areaTop, areaRight - areaLeft + 1,
								  areaBottom - areaTop + 1, LCD_65K_RGB::Pink);
*/
	uint16_t DrawLeft = left;
	uint16_t DrawTop = top;
	int16_t Direction;

	if(( options & O_LookUp) || ( options & O_LookLeft))
	{
		Direction = 1;
	}
	else
	{
		if( options & O_LookDown)
		{
			DrawTop += size - 1;
		}
		else
		{
			DrawLeft += size - 1;
		}

		Direction = -1;
	}

	uint16_t DrawSize = 1;

	for( uint8_t Line = 0; Line < size; Line++)
	{
		if(( options & O_LookUp) || ( options & O_LookDown))
		{
			GLOBAL.SetupDisplay.DrawHorizontalLine( DrawLeft, DrawTop, DrawSize, DrawColor);

			if( DrawLeft > areaLeft)
			{
				DrawLeft--;
				DrawSize++;
			}

			if(( DrawLeft + DrawSize - 1) < areaRight)
			{
				DrawSize++;
			}

			DrawTop += Direction;
		}
		else
		{
			GLOBAL.SetupDisplay.DrawVerticalLine( DrawLeft, DrawTop, DrawSize, DrawColor);

			if( DrawTop > areaTop)
			{
				DrawTop--;
				DrawSize++;
			}

			if(( DrawTop + DrawSize - 1) < areaBottom)
			{
				DrawSize++;
			}

			DrawLeft += Direction;
		}
	}
}

void GUI_Setup_Marker::SetArea( uint16_t Left, uint16_t Top, uint16_t Width, uint16_t Height)
{	
	areaLeft = Left;
	areaTop = Top;
	areaRight = areaLeft + Width - 1;
	areaBottom = areaTop + Height - 1;

	remembered = false;
}

void GUI_Setup_Marker::SetOptions( Options UseOptions)
{
	options = UseOptions;

	remembered = false;
}

void GUI_Setup_Marker::SetSize( uint8_t Size)
{
	size = Size;

	remembered = false;
}

void GUI_Setup_Marker::SetPosition( uint16_t Left, uint16_t Top)
{
	if( remembered)
	{
		if(( Left == left) && ( Top == top))
		{
			// No change, no redraw.
			return;
		}
	}
	
	left = Left;
	top = Top;

	remembered = false;
}

void GUI_Setup_Marker::Display( uint16_t Left, uint16_t Top)
{
	if( remembered == true)
	{
		if(( Left == left) && ( Top == top))
		{
			// No change, no redraw.
			return;
		}

		display( COPM_Clear);
	}

	left = Left;
	top = Top;

	remembered = true;

	display( COPM_Print);
}

void GUI_Setup_Marker::Display( void)
{
	remembered = true;

	display( COPM_Print);
}

void GUI_Setup_Marker::Clear( void)
{
	if( remembered)
	{
		display( COPM_Clear);

		remembered = false;
	}
}
