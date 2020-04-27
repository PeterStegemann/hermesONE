// Copyright 2008 Peter Stegemann

#include "GaugeSelect.h"

#include "Defines.h"
#include "Gauge.h"
#include "Label.h"
#include "Marker.h"

#include "Main/Setup.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

bool GUI_Setup_GaugeSelect::DoSelect( uint8_t* Value, uint8_t LowerLimit, uint8_t UpperLimit,
									  uint8_t StepWidth, GUI_Setup_Marker* Marker,
									  GUI_Setup_Gauge* Gauge, void ( *UpdateGauge)( uint8_t Value))
{
	// Save some eeprom write cycles with this flag.
	bool ValueChanged = false;

	// Select.
	Marker->ForegroundColor = LCD_65K_RGB::C_Red;
	Marker->Display();
	Gauge->SetDetailColor( LCD_65K_RGB::C_Red);

	bool Exit = false;

	while( Exit == false)
	{
		int16_t NewValue = *Value;

		// Align to stepwidth.
		NewValue -= NewValue % StepWidth;
		
		Gauge->Display( LowerLimit, UpperLimit, *Value);

		int8_t RotarySelect;
		uint8_t RotaryButton;

		GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

		if( RotaryButton > 0)
		{
			UpdateGauge( *Value);

			Exit = true;
		}

		if( RotarySelect != 0)
		{
			ValueChanged = true;

			NewValue += RotarySelect * StepWidth;

			if( NewValue < LowerLimit)
			{
				NewValue = LowerLimit;
			}
			else if( NewValue > UpperLimit)
			{
				NewValue = UpperLimit;
			}

			*Value = NewValue;

			UpdateGauge( NewValue);
		}
	}

	// Unselect.
	Marker->ForegroundColor = LCD_65K_RGB::C_WarmYellow;
	Marker->Display();
	Gauge->SetDetailColor( LCD_65K_RGB::C_WarmYellow);
	Gauge->Display( LowerLimit, UpperLimit, *Value);

	return( ValueChanged);
}
