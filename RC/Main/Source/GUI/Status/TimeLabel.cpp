// Copyright 2012 Peter Stegemann

#include "TimeLabel.h"

#include "Text/Text.h"

void GUI_Status_TimeLabel::SetTime( int16_t Time)
{
	if( remembered == true)
	{
		// If we have the same values as last time, don't display anything.
		if( value == Time)
		{
			// No changes.
			return;
		}
	}

	value = Time;

	remembered = false;

	if( Time < 0)
	{
		Time = -Time;
		snprintf_P( timeLabelText, sizeof( timeLabelText), Text::NegativeTimeFormat, Time / 60,
				    Time % 60);
	}
	else
	{
		snprintf_P( timeLabelText, sizeof( timeLabelText), Text::TimeFormat, Time / 60, Time % 60);
	}

	SetText( timeLabelText);

	Display();
}
