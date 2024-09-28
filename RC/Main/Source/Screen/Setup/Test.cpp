// Copyright 2007 Peter Stegemann

#include "Test.h"

#include "Input.h"
#include "Ports.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"
#include "AVR/Source/LCD/LCD.h"

using namespace Screen::Setup;

Test::Test( Screen_Status_Status* StatusScreen)
    : Screen_Setup_Base( InputService, StatusScreen)
{
}

void Test::Run( void)
{
	DrawFrame( "Test");
/*
	GUI_Setup_Marker menuMarker( lcd);
	GetMenuMarker()->SetArea( GetLeft() + 5, GetTop() + 5, GetWidth() - 10, GetHeight() - 10);

	markerLeft = GetLeft() + 5;
	markerTop = GetTop() + 5;

	lcd->DrawRect(( GetLeft() + 5) - 1, ( GetTop() + 5) - 1, ( GetWidth() - 10) + 2, ( GetHeight() - 10) + 2, LCD::Red);

	GetMenuMarker()->Display( markerLeft, markerTop);

	while( true)
	{
		uint8_t Changed;
		uint8_t Keys = inputKeys->GetKeys( &Changed);

//		if( Changed != 0)
		{
			if( !UTILITY_GetBit( Keys, INPUT_LEFT))
			{
				if( markerLeft > 0)
				{
					markerLeft--;

					GetMenuMarker()->Display( markerLeft, markerTop);
				}
			}
			
			if( !UTILITY_GetBit( Keys, INPUT_RIGHT))
			{
				if( markerLeft < lcd->GetWidth())
				{
					markerLeft++;

					GetMenuMarker()->Display( markerLeft, markerTop);
				}
			}

			if( !UTILITY_GetBit( Keys, INPUT_UP))
			{
				if( markerTop > 0)
				{
					markerTop--;
					
					GetMenuMarker()->Display( markerLeft, markerTop);
				}
			}
			
			if( !UTILITY_GetBit( Keys, INPUT_DOWN))
			{
				if( markerTop < lcd->GetHeight())
				{
					markerTop++;
					
					GetMenuMarker()->Display( markerLeft, markerTop);
				}
			}
		}

		avr::Utility::Pause( 100);
	}
*/
	uint16_t Top = GetTop();

	for( uint8_t UseFontID = 0; UseFontID < avr::font::Font::GetCount(); UseFontID++)
	{
		const avr::font::Type* Font = avr::font::Font::Get(( avr::font::FontId) UseFontId);

		// Print characters from 32 to 133.
		for( uint8_t Line = 0; Line < 4; Line++)
		{
			char String[ 24];

			for( uint8_t Character = 0; Character < 23; Character++)
			{
				String[ Character] = Line * 23 + Character + 32;
			}

			String[ 23] = 0;

			GetLCD()->Print( GetLeft(), Top, ( avr::font::FontId) UseFontID, LCD::White, LCD::Black,
							 LCD::PM_Proportional, String);

			Top += Font->GetCellHeight();
		}
	}

	while( true)
	{
		uint8_t Changed;
		uint8_t Keys = GetInputKeys()->GetKeys( &Changed);

		if( Changed != 0)
		{
			if( UTILITY_GetBit( Changed, INPUT_ENTER) && UTILITY_GetBit( Keys, INPUT_ENTER))
			{
				break;
			}
		}
		
		avr::Utility::Pause( 5);
	}
}
