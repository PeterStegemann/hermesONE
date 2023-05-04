// Copyright 2012 Peter Stegemann

#pragma once

#include "Label.h"

#include "Text/Text.h"

class GUI_Setup_TimeLabel : public GUI_Setup_Label
{
  private:
    char timeLabelText[ 7];

  public:
    void SetTime( int16_t Time)
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
            snprintf_P( timeLabelText, sizeof( timeLabelText), Text::NegativeTimeFormat, Time / 60, Time % 60);
        }
        else
        {
            snprintf_P( timeLabelText, sizeof( timeLabelText), Text::TimeFormat, Time / 60, Time % 60);
        }

        SetText( timeLabelText);

        Display();
    }
};
