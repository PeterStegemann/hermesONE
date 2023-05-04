// Copyright 2012 Peter Stegemann

#pragma once

#include "Gauge.h"

class GUI_Setup_RangeGauge : public GUI_Setup_Gauge
{
  private:
    GUI_Setup_Marker lowerMarker, upperMarker;

    // Remembered values.
    int16_t lowerLimit, upperLimit;

  public:
    GUI_Setup_RangeGauge( void)
        : lowerLimit( 0)
        , upperLimit( 0)
    {
    	lowerMarker.ForegroundColor = detailColor;
	    lowerMarker.BackgroundColor = backgroundColor;
	    upperMarker.ForegroundColor = detailColor;
	    upperMarker.BackgroundColor = backgroundColor;
    }

    void SetRange( int16_t LowerLimit, int16_t UpperLimit)
    {
        if( remembered == true)
        {
            // If we have the same values as last time, don't display anything.
            if(( lowerLimit == LowerLimit) && ( upperLimit == UpperLimit))
            {
                // No changes.
    //			return;
            }
        }

        lowerLimit = LowerLimit;
        upperLimit = UpperLimit;

    //	remembered = false;

        lowerMarker.Clear();
        upperMarker.Clear();

        lowerMarker.SetArea( fillLeft, fillTop, fillWidth, fillHeight);
        upperMarker.SetArea( fillLeft, fillTop, fillWidth, fillHeight);

        if( options & O_Vertical)
        {
            lowerMarker.SetSize( fillWidth - 3);
            lowerMarker.SetOptions( GUI_Setup_Marker::O_LookDown);
            upperMarker.SetSize( fillWidth - 3);
            upperMarker.SetOptions( GUI_Setup_Marker::O_LookUp);
        }
        else
        {
            lowerMarker.SetSize( fillHeight - 3);
            lowerMarker.SetOptions( GUI_Setup_Marker::O_LookRight);
            upperMarker.SetSize( fillHeight - 3);
            upperMarker.SetOptions( GUI_Setup_Marker::O_LookLeft);
        }

        Display( minimum, maximum, value);
    }

    virtual void Display( int16_t Minimum, int16_t Maximum, int16_t Value)
    {
        if( remembered == true)
        {
            GUI_Setup_Gauge::Display( Minimum, Maximum, Value);

            return;
        }
        else
        {
            GUI_Setup_Gauge::Display( Minimum, Maximum, Value);
        }

        // Normalise values.
        int32_t NormalizedLowerLimit = lowerLimit;
        NormalizedLowerLimit -= Minimum;
        int32_t NormalizedUpperLimit = upperLimit;
        NormalizedUpperLimit -= Minimum;
        int32_t NormalizedMaximum = Maximum;
        NormalizedMaximum -= Minimum;

        int32_t LowerValueSize = 0;
        int32_t UpperValueSize = 0;

        if( NormalizedMaximum != 0)
        {
            if( options & O_Vertical)
            {
                LowerValueSize = NormalizedLowerLimit * fillHeight / NormalizedMaximum;
                UpperValueSize = NormalizedUpperLimit * fillHeight / NormalizedMaximum;
            }
            else
            {
                LowerValueSize = NormalizedLowerLimit * fillWidth / NormalizedMaximum;
                UpperValueSize = NormalizedUpperLimit * fillWidth / NormalizedMaximum;
            }
        }

        if( options & O_Vertical)
        {
            lowerMarker.Display( fillLeft + 3, fillTop + LowerValueSize);
            upperMarker.Display( fillLeft + 3, fillTop + UpperValueSize - ( fillWidth - 3));
        }
        else
        {
            lowerMarker.Display( fillLeft + LowerValueSize, fillTop + 3);
            upperMarker.Display( fillLeft + UpperValueSize - ( fillHeight - 3), fillTop + 3);
        }
    }
};
