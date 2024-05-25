// Copyright 2010 Peter Stegemann

#include "Base.h"

#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"
#include "AVR/Source/LCD/LCD_DOG.h"

Screen_Status_Menu_Base::Screen_Status_Menu_Base( uint8_t MenuEntries, Level MenuLevel)
					   : Screen_Base()
					   , menuEntries( MenuEntries)
					   , menuLevel( MenuLevel)
{
	uint32_t MenuPattern = 0;

	while( MenuEntries > 0)
	{
		MenuEntries--;

		UTILITY_SetBit32( MenuPattern, MenuEntries);
	}

	SetMenuPattern( MenuPattern);
}

Screen_Status_Menu_Base::~Screen_Status_Menu_Base( void)
{
}

void Screen_Status_Menu_Base::SetText( const char* Text)
{
	SetText( menuLevel, Text);
}

void Screen_Status_Menu_Base::SetText_P( const flash_char* Text)
{
	SetText_P( menuLevel, Text);
}

void Screen_Status_Menu_Base::SetText( Level UseLevel, const char* Text)
{
	uint8_t Column = 7;
	uint8_t Limit = GLOBAL.StatusDisplay.GetWidth() - Column;

	if( Text != NULL)
	{
		Column += GLOBAL.StatusDisplay.Print( Column, UseLevel * 2, Limit, FONT::FI_Large, Text);
	}

	// Fill trailing space.
	while( Column <= Limit)
	{
		GLOBAL.StatusDisplay.Write( Column, UseLevel * 2, 0x00);
		GLOBAL.StatusDisplay.Write( Column, UseLevel * 2 + 1, 0x00);

		Column++;
	}
}

void Screen_Status_Menu_Base::SetText_P( Level UseLevel, const flash_char* Text)
{
	uint8_t Column = 7;
	uint8_t Limit = GLOBAL.StatusDisplay.GetWidth() - Column;

	if( Text != NULL)
	{
		Column += GLOBAL.StatusDisplay.Print_P( Column, UseLevel * 2, Limit, FONT::FI_Large, Text);
	}

	// Fill trailing space.
	while( Column <= Limit)
	{
		GLOBAL.StatusDisplay.Write( Column, UseLevel * 2, 0x00);
		GLOBAL.StatusDisplay.Write( Column, UseLevel * 2 + 1, 0x00);

		Column++;
	}
}

void Screen_Status_Menu_Base::SetSource( uint16_t SetupSourceId)
{
	SetSource( menuLevel, SetupSourceId);
}

void Screen_Status_Menu_Base::SetSource( Level UseLevel, uint16_t SetupSourceId)
{
	if( SetupSourceId == SETUP_SOURCE_NONE)
	{
		SetText_P( UseLevel, Text::None);
	}
	else if( SetupSourceId == SETUP_SOURCE_FIXED)
	{
		SetText_P( UseLevel, Text::Fixed);
	}
	else
	{
		if( GLOBAL.SetupService.GetSourceType( SetupSourceId) == Signal_Source_Source::T_Empty)
		{
			SetText_P( UseLevel, Text::Empty);
		}
		else
		{
			char Name[ SETUP_SOURCE_NAME_SIZE + 1];

			GLOBAL.SetupService.GetSourceName( SetupSourceId, Name, sizeof( Name));
			SetText( UseLevel, Name);
		}
	}
}

void Screen_Status_Menu_Base::updateBoolean( bool Value)
{
	if( Value == true)
	{
		SetText_P(( Level)( menuLevel + 1), Text::Yes);
	}
	else
	{
		SetText_P(( Level)( menuLevel + 1), Text::No);
	}
}

bool Screen_Status_Menu_Base::selectValue( int8_t* Value, int8_t LowerLimit, int8_t UpperLimit,
										   int8_t StepWidth, void* Object,
										   void ( *UpdateValue)( void* Object, int8_t Value))
{
	int16_t NewValue = *Value;

	// Align to step width.
	NewValue -= NewValue % StepWidth;

	// Saves some write cycles with this flag.
	bool ValueChanged = false;

	drawMenuMarker( menuLevel + 1, true, true);

	bool Exit = false;

	while( Exit == false)
	{
		int8_t RotarySelect;
		uint8_t RotaryButton;

		GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

		if( RotaryButton > 0)
		{
			Exit = true;
		}

		// Set new position.
		if( RotarySelect != 0)
		{
			ValueChanged = true;

			NewValue += RotarySelect * StepWidth;

			if( NewValue > UpperLimit)
			{
				NewValue -= (( int16_t) UpperLimit - ( int16_t) LowerLimit) + StepWidth;
			}
			else if( NewValue < LowerLimit)
			{
				NewValue += (( int16_t) UpperLimit - ( int16_t) LowerLimit) + StepWidth;
			}

			*Value = NewValue;

			// Refresh label.
			UpdateValue( Object, ( int8_t) NewValue);
		}

		UTILITY::Pause( 5);
	}

	// Unselect.
	drawMenuMarker( menuLevel, true, true);

	return( ValueChanged);
}

bool Screen_Status_Menu_Base::selectValue( int16_t* Value, int16_t LowerLimit, int16_t UpperLimit,
										   int16_t StepWidth, void* Object,
										   void ( *UpdateValue)( void* Object, int16_t Value))
{
	int32_t NewValue = *Value;

	// Align to step width.
	NewValue -= NewValue % StepWidth;

	// Save some write cycles with this flag.
	bool ValueChanged = false;

	drawMenuMarker( menuLevel + 1, true, true);

	bool Exit = false;

	while( Exit == false)
	{
		int8_t RotarySelect;
		uint8_t RotaryButton;

		GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

		if( RotaryButton > 0)
		{
			Exit = true;
		}

		// Set new position.
		if( RotarySelect != 0)
		{
			ValueChanged = true;

			NewValue += RotarySelect * StepWidth;

			if( NewValue > UpperLimit)
			{
				NewValue -= (( int32_t) UpperLimit - ( int32_t) LowerLimit) + StepWidth;
			}
			else if( NewValue < LowerLimit)
			{
				NewValue += (( int32_t) UpperLimit - ( int32_t) LowerLimit) + StepWidth;
			}
			
			*Value = NewValue;

			// Refresh label.
			UpdateValue( Object, ( int16_t) NewValue);
		}

		UTILITY::Pause( 5);
	}

	// Unselect.
	drawMenuMarker();

	return( ValueChanged);
}

bool Screen_Status_Menu_Base::selectValue( uint16_t* Value, uint16_t LowerLimit,
										   uint16_t UpperLimit, uint16_t StepWidth, void* Object,
										   void ( *UpdateValue)( void* Object, uint16_t Value))
{
	int32_t NewValue = *Value;

	// Align to step width.
	NewValue -= NewValue % StepWidth;

	// Save some write cycles with this flag.
	bool ValueChanged = false;

	drawMenuMarker( menuLevel + 1, ( NewValue > LowerLimit), ( NewValue < UpperLimit));

	bool Exit = false;

	while( Exit == false)
	{
		int8_t RotarySelect;
		uint8_t RotaryButton;

		GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

		if( RotaryButton > 0)
		{
			Exit = true;
		}

		// Set new position.
		if( RotarySelect != 0)
		{
			ValueChanged = true;

			NewValue += RotarySelect * ( int32_t) StepWidth;

			if( NewValue > UpperLimit)
			{
				NewValue = UpperLimit;
			}
			else if( NewValue < LowerLimit)
			{
				NewValue = LowerLimit;
			}

			*Value = NewValue;

			drawMenuMarker( menuLevel + 1, ( NewValue > LowerLimit), ( NewValue < UpperLimit));

			// Refresh label.
			UpdateValue( Object, ( int16_t) NewValue);
		}

		UTILITY::Pause( 5);
	}

	// Unselect.
	drawMenuMarker();

	return( ValueChanged);
}

bool Screen_Status_Menu_Base::selectSource
(
    uint8_t* SignalSourceId, uint16_t* SetupSourceId,
    void* Object, void ( *UpdateSource)( void* Object, uint16_t SetupSourceId), Signal_Source_Source::Level Level
)
{
	// Save some write cycles with this flag.
	bool SourceChanged = false;

	drawMenuMarker( menuLevel + 1, true, true);

	bool Exit = false;

	while( Exit == false)
	{
		int8_t RotarySelect;
		uint8_t RotaryButton;

		GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

		if( RotaryButton > 0)
		{
			Exit = true;
		}

		// Set new position.
		if( RotarySelect != 0)
		{
			SourceChanged = true;

			*SignalSourceId = GLOBAL.SignalProcessor.FindNextSource( *SignalSourceId, RotarySelect, false, Level);

			if( *SignalSourceId == SIGNAL_SOURCE_NONE)
			{
				*SetupSourceId = SETUP_SOURCE_NONE;
			}
			else if( *SignalSourceId == SIGNAL_SOURCE_FIXED)
			{
				*SetupSourceId = SETUP_SOURCE_FIXED;
			}
			else
			{
				const Signal_Source_Source* SignalSource = GLOBAL.SignalProcessor.GetSource( *SignalSourceId);

				*SetupSourceId = SignalSource->GetSetupSourceId();
			}

			// Refresh label.
			UpdateSource( Object, *SetupSourceId);
		}

		UTILITY::Pause( 5);
	}

	// Unselect.
	drawMenuMarker();
	
	return( SourceChanged);
}

void Screen_Status_Menu_Base::drawMenuMarker( void)
{
	drawMenuMarker( menuLevel);
}

void Screen_Status_Menu_Base::drawMenuMarker( uint8_t MenuLevel)
{
	drawMenuMarker( MenuLevel, currentMenuEntry > 0, currentMenuEntry < ( menuEntries - 1));
}

void Screen_Status_Menu_Base::drawMenuMarker( uint8_t MenuLevel, bool ShowLeft, bool ShowRight)
{
	static uint8_t Arrow[] =
	{
		0b00000000, 0b01000000,
		0b00000000, 0b11100000,
		0b00000001, 0b11110000,
		0b00000011, 0b10111000,
		0b00000111, 0b00011100
	};

	for( uint8_t CurrentLine = 0; CurrentLine < 8; CurrentLine += 2)
	{
		for( uint8_t Count = 1; Count < 6; Count++)
		{
			// Left marker.
			GLOBAL.StatusDisplay.Write( Count, CurrentLine, 0x00);
			GLOBAL.StatusDisplay.Write( Count, CurrentLine + 1, 0x00);

			// Right marker.
			GLOBAL.StatusDisplay.Write( GLOBAL.StatusDisplay.GetWidth() - Count, CurrentLine, 0x00);
			GLOBAL.StatusDisplay.Write( GLOBAL.StatusDisplay.GetWidth() - Count, CurrentLine + 1, 0x00);
		}
	}

	uint8_t CurrentLine = MenuLevel * 2;

	for( uint8_t Count = 0; Count < 5; Count++)
	{
		uint8_t Index = Count * 2;

		if( ShowLeft == true)
		{
			// Left marker.
			GLOBAL.StatusDisplay.Write( Count + 1, CurrentLine, Arrow[ Index + 1]);
			GLOBAL.StatusDisplay.Write( Count + 1, CurrentLine + 1, Arrow[ Index]);
		}

		if( ShowRight == true)
		{
			// Right marker.
			GLOBAL.StatusDisplay.Write( GLOBAL.StatusDisplay.GetWidth() - ( Count + 1), CurrentLine, Arrow[ Index + 1]);
			GLOBAL.StatusDisplay.Write( GLOBAL.StatusDisplay.GetWidth() - ( Count + 1), CurrentLine + 1, Arrow[ Index]);
		}
	}
}

void Screen_Status_Menu_Base::clearSubMenuLine( void)
{
	SetText_P(( Level)( menuLevel + 1), NULL);
	drawMenuMarker();
}

void Screen_Status_Menu_Base::display( void)
{
}

void Screen_Status_Menu_Base::pickMenu( uint8_t MenuEntry)
{
	currentMenuEntry = MenuEntry;

	processMenu( DMR_Changed);
}

void Screen_Status_Menu_Base::update( void)
{
}

void Screen_Status_Menu_Base::blank( bool Blanked)
{
	GLOBAL.StatusDisplay.SetBlanked( Blanked);
}

bool Screen_Status_Menu_Base::processMenu( DoMenuResult Result)
{
	return( true);
}
