// Copyright 2011 Peter Stegemann

#include "VoltageLabel.h"

#include "Text/Text.h"

void GUI_Setup_VoltageLabel::SetVoltage( uint8_t Voltage)
{
	if( remembered == true)
	{
		// If we have the same values as last time, don't display anything.
		if( value == Voltage)
		{
			// No changes.
			return;
		}
	}

	value = Voltage;

	remembered = false;

	snprintf_P( voltageLabelText, sizeof( voltageLabelText), Text::VoltageFormat,
			    Voltage / 10, Voltage % 10);
	SetText( voltageLabelText);

	Display();
}
