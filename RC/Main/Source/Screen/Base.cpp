// Copyright 2007 Peter Stegemann

#include "Base.h"

#include "Ports.h"
#include "Main/Global.h"
#include "Setup/Defines.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

#include <string.h>

Screen_Base::Screen_Base( Input_Service* InputService, uint32_t MenuPattern)
		   : menuPattern( MenuPattern)
		   , inputService( InputService)
		   , currentMenuEntry( 0)
{
}

Screen_Base::~Screen_Base( void)
{
}

void Screen_Base::drawMenuMarker( void)
{
}

void Screen_Base::display( void)
{
}

void Screen_Base::update( void)
{
}

bool Screen_Base::processMenu( Screen_Base::DoMenuResult Result)
{
	return( false);
}

Screen_Base::DoMenuResult Screen_Base::doMenu( uint32_t MenuPattern)
{
	// Find lower boundary.
	uint8_t MinimumEntry = 0;

	while( MinimumEntry < 31)
	{
		if( UTILITY_GetBit32( MenuPattern, MinimumEntry))
		{
			break;
		}

		MinimumEntry++;
	}

	if( currentMenuEntry == 0)
	{
		currentMenuEntry = MinimumEntry;
	}

	// Find upper boundary.
	uint8_t MaximumEntry = 31;

	while( MaximumEntry > MinimumEntry)
	{
		if( UTILITY_GetBit32( MenuPattern, MaximumEntry))
		{
			break;
		}

		MaximumEntry--;
	}

	// Try rotary.
	int8_t RotarySelect;
	uint8_t RotaryButton;
	bool RotaryCurrentButton;
	uint16_t RotaryButtonTime;

	inputService->GetRotary( &RotarySelect, &RotaryButton, &RotaryCurrentButton, &RotaryButtonTime);

	// Check for long press while the button is still pressed.
	if( RotaryCurrentButton == true)
	{
		if( RotaryButtonTime >= SYSTEM_LONG_PRESS_DURATION)
		{
			return( DMR_LongPress);
		}
	}

	if( RotaryButton > 0)
	{
		return( DMR_Selected);
	}

	// If no new rotary value, skip.
	if( RotarySelect == 0)
	{
		return( DMR_None);
	}

	if(( RotarySelect < 0) && ( currentMenuEntry == MinimumEntry))
	{
//		GLOBAL.StatusService.Beep();

		// Minimum reached, ignore.
		return( DMR_None);
	}

	if(( RotarySelect > 0) && ( currentMenuEntry == MaximumEntry))
	{
//		GLOBAL.StatusService.Beep();

		// Maximum reached, ignore.
		return( DMR_None);
	}

	while( RotarySelect < 0)
	{
		// Go next entry and skip empty slots.
		do
		{
			currentMenuEntry--;
		}
		while( UTILITY_GetBit32( MenuPattern, currentMenuEntry) == false);

		if( currentMenuEntry == MinimumEntry)
		{
			// Minimum reached, done.
			break;
		}

		RotarySelect++;
	}

	while( RotarySelect > 0)
	{
		do
		{
			currentMenuEntry++;
		}
		while( UTILITY_GetBit32( MenuPattern, currentMenuEntry) == false);

		if( currentMenuEntry == MaximumEntry)
		{
			// Maximum reached, ignore.
			break;
		}

		RotarySelect--;
	}

	drawMenuMarker();

	return( DMR_Changed);
}

void Screen_Base::ReDisplay( void)
{
	display();

	drawMenuMarker();
}

void Screen_Base::SetMenuPattern( uint32_t MenuPattern)
{
	menuPattern = MenuPattern;
}

void Screen_Base::Run( void)
{
	// Initial full display.
	ReDisplay();

	while( true)
	{
		DoMenuResult CurrentMenuResult = doMenu( menuPattern);

		if( CurrentMenuResult != DMR_None)
		{
			if( processMenu( CurrentMenuResult) == false)
			{
				return;
			}
		}

		update();

		// Main has some important jobs to do, too.
		GLOBAL.Update();
	}
}
