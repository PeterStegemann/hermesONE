// Copyright 2011 Peter Stegemann

#include "MillisecondLabel.h"

#include "Text/Text.h"

void GUI_Setup_MillisecondLabel::SetMillisecond( uint8_t Millisecond)
{
	if( remembered == true)
	{
		// If we have the same values as last time, don't display anything.
		if( value == Millisecond)
		{
			// No changes.
			return;
		}
	}

	value = Millisecond;

	remembered = false;

	snprintf_P( millisecondLabelText, sizeof( millisecondLabelText), Text::MillisecondFormat,
				Millisecond / 10, Millisecond % 10);
	SetText( millisecondLabelText);

	Display();
}
