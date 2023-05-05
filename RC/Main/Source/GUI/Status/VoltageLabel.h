// Copyright 2012 Peter Stegemann

#pragma once

#include "Label.h"

#include "Text/Text.h"

class GUI_Status_VoltageLabel : public GUI_Status_Label
{
  private:
    char voltageLabelText[ 6];

  public:
    void SetVoltage( uint8_t Voltage)
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

    	snprintf_P( voltageLabelText, sizeof( voltageLabelText), Text::VoltageFormat, Voltage / 10, Voltage % 10);
    	SetText( voltageLabelText);

    	Display();
    }
};
