// Copyright 2011 Peter Stegemann

#pragma once

#include "Label.h"

#include "Text/Text.h"

class GUI_Setup_VoltageLabel : public GUI_Setup_Label
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
