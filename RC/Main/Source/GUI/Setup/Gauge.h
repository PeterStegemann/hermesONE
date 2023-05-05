// Copyright 2007 Peter Stegemann

#pragma once

#include "Defines.h"
#include "Marker.h"

#include "Text/Text.h"

#include "AVR/Source/Types.h"

class GUI_Setup_Gauge
{
  public:
    enum Options
    {
        O_None					= 0b00000000,
        O_CenterLine			= 0b00000001,
        O_Percentage			= 0b00000010,
        O_SinglePercentage		= 0b00000000,		// Range 0..100%
        O_DualPercentage		= 0b00000100,		// Range -100%..0..100%
        O_Value					= 0b00001000,
        O_Fill					= 0b00000000,
        O_Marker				= 0b00010000,
        O_Horizontal			= 0b00000000,
        O_Vertical				= 0b00100000,
        O_Reverse				= 0b01000000,
        O_MarkerLookRight		= 0b00000000,
        O_MarkerLookUp			= O_MarkerLookRight,
        O_MarkerLookLeft		= 0b10000000,
        O_MarkerLookDown		= O_MarkerLookLeft
    };

  protected:
    GUI_Setup_Marker marker;

    uint16_t left, top, width, height;

    Options options;

    LCD_65K_RGB::Color foregroundColor, backgroundColor, detailColor;

    FONT::FontId fontId;

    // Marks whether we have remembered values from last draw.
    bool remembered;
    // Remembered values.
    int16_t minimum, maximum, value;

    // Cached dimension values for drawing.
    uint16_t percentageLeft, percentageWidth;
    uint16_t valueLeft, valueWidth;
    uint16_t gaugeLeft, gaugeWidth;
    uint16_t fillLeft, fillTop, fillWidth, fillHeight;
    uint16_t centerPosition;

    // Calculate inner dimensions.
    void calculateInnerDimensions( void)
    {
        const FONT_Type* Font = FONT::GetFont( fontId);

        int32_t Width = width;

        percentageLeft = left;

        // Display percentage?
        if( options & O_Percentage)
        {
            // Check if the text fits in.
    //		if( height > Font->CellHeight)
            {
                uint16_t TextWidth;

                if( options & O_DualPercentage)
                {
                    TextWidth = 5 * Font->CellWidth;
                }
                else
                {
                    TextWidth = 4 * Font->CellWidth;
                }

                if( Width > TextWidth)
                {
                    percentageWidth = TextWidth;
                }
            }
        }

        if( percentageWidth != 0)
        {
            Width -= percentageWidth + 2;
            valueLeft = percentageLeft + percentageWidth + 2;
        }
        else
        {
            // No extra spacing.
            valueLeft = percentageLeft;
        }

        // Display value?
        if( options & O_Value)
        {
            // Check if the text fits in.
    //		if( height > Font->CellHeight)
            {
                uint16_t TextWidth = 6 * Font->CellWidth;

                if( Width > TextWidth)
                {
                    valueWidth = TextWidth;
                }
            }
        }

        if( valueWidth != 0)
        {
            gaugeWidth = Width - ( valueWidth + 2);
            gaugeLeft = valueLeft + ( valueWidth + 2);
        }
        else
        {
            gaugeWidth = Width;
            gaugeLeft = valueLeft;
        }

        // We need a minimum space for display.
        if(( gaugeWidth < 7) || ( height < 7))
        {
            return;
        }

        // Do fill level.
        fillLeft = gaugeLeft + 3;
        fillWidth = gaugeWidth - 6;
        fillTop = top + 3;
        fillHeight = height - 6;

        // Display marker or normal?
        if( options & O_Marker)
        {
            marker.SetArea( fillLeft, fillTop, fillWidth, fillHeight);

            if( options & O_Vertical)
            {
                marker.SetSize( fillWidth - 1);
            }
            else
            {
                marker.SetSize( fillHeight - 1);
            }

            // Move the marker so it becomes an arrow looking from bottom to top.
            if( options & O_Vertical)
            {
                if( options & O_MarkerLookLeft)
                {
                    marker.SetOptions( GUI_Setup_Marker::O_LookLeft);
                }
                else
                {
                    marker.SetOptions( GUI_Setup_Marker::O_LookRight);
                }
            }
            else
            {
                if( options & O_MarkerLookDown)
                {
                    marker.SetOptions( GUI_Setup_Marker::O_LookDown);
                }
                else
                {
                    marker.SetOptions( GUI_Setup_Marker::O_LookUp);
                }
            }
        }

        // Calculate center line, if needed.
        if( options & O_CenterLine)
        {
            if( options & O_Vertical)
            {
                centerPosition = fillTop + ( fillHeight - 1) / 2;
            }
            else
            {
                centerPosition = fillLeft + ( fillWidth - 1) / 2;
            }
        }
    }

  public:
    GUI_Setup_Gauge( void)
       : left( 0)
       , top( 0)
       , width( 0)
       , height( 0)
       , options( O_None)
       , foregroundColor( LCD_65K_RGB::C_White)
       , backgroundColor( LCD_65K_RGB::C_Black)
       , detailColor( LCD_65K_RGB::C_WarmYellow)
       , fontId( GUI_SETUP_MAIN_FONT)
       , remembered( false)
       , minimum( 0)
       , maximum( 0)
       , value( 0)
       , percentageLeft( 0)
       , percentageWidth( 0)
       , valueLeft( 0)
       , valueWidth( 0)
       , gaugeLeft( 0)
       , gaugeWidth( 0)
       , fillLeft( 0)
       , fillTop( 0)
       , fillWidth( 0)
       , fillHeight( 0)
       , centerPosition( 0)
    {
        // Recalculate inner dimensions.
        calculateInnerDimensions();

        marker.ForegroundColor = detailColor;
        marker.BackgroundColor = backgroundColor;
    }

    // Set the location of the gauge on the screen.
    void SetDimensions( uint16_t Left, uint16_t Top, uint16_t Width, uint16_t Height)
    {
        left = Left;
        top = Top;
        width = Width;
        height = Height;

        // Recalculate inner dimensions.
        calculateInnerDimensions();
    }

    // Set some display options.
    void SetOptions( Options UseOptions)
    {
        options = UseOptions;

        // Recalculate inner dimensions.
        calculateInnerDimensions();
    }

    void SetColors
    (
        LCD_65K_RGB::Color ForegroundColor, LCD_65K_RGB::Color BackgroundColor, LCD_65K_RGB::Color DetailColor
    )
    {
        foregroundColor = ForegroundColor;
        backgroundColor = BackgroundColor;
        detailColor = DetailColor;

        marker.ForegroundColor = detailColor;
        marker.BackgroundColor = backgroundColor;

        remembered = false;
    }

    void SetBackgroundColor( LCD_65K_RGB::Color BackgroundColor)
    {
        backgroundColor = BackgroundColor;
        marker.BackgroundColor = BackgroundColor;

        remembered = false;
    }

    void SetDetailColor( LCD_65K_RGB::Color DetailColor)
    {
        detailColor = DetailColor;
        marker.ForegroundColor = DetailColor;

        remembered = false;
    }

    // This will update the view to reflect the given values.
    void Display( void)
    {
        Display( minimum, maximum, value);
    }

    void Display( int16_t Value)
    {
        Display( minimum, maximum, Value);
    }

    void Display( int16_t Minimum, int16_t Maximum, int16_t Current)
    {
        if( remembered == true)
        {
            // If we have the same values as last time, don't display anything.
            if(( minimum == Minimum) && ( maximum == Maximum) && ( value == Current))
            {
                // No changes.
                return;
            }
        }

        // Remember values.
        minimum = Minimum;
        maximum = Maximum;
        value = Current;
        remembered = true;

        if( Minimum > Maximum)
        {
            // Turn this around.
            int16_t Swap = Minimum;
            Minimum = Maximum;
            Maximum = Swap;

            // Mirror value.
            uint32_t Center = Maximum;
            Center += Minimum;
            Center /= 2;
            Current = Center - Current;
        }

        if( options & O_Reverse)
        {
            // Mirror value.
            uint32_t Center = Maximum;
            Center += Minimum;
            Center /= 2;
            Current = Center - Current;
        }

        if( Current < Minimum)
        {
            Current = Minimum;
        }
        else if( Current > Maximum)
        {
            Current = Maximum;
        }

        // Normalise values.
        int32_t NormalizedCurrent = Current;
        NormalizedCurrent -= Minimum;
        int32_t NormalizedMaximum = Maximum;
        NormalizedMaximum -= Minimum;

        // Draw.

        // Display percentage?
        if(( options & O_Percentage) && ( percentageWidth != 0))
        {
            if( NormalizedMaximum != 0)
            {
                if( options & O_DualPercentage)
                {
                    int32_t Value = NormalizedCurrent * 200;
                    Value /= NormalizedMaximum;
                    Value -= 100;

                    GLOBAL.SetupDisplay.PrintFormat_P( percentageLeft, top, fontId, foregroundColor,
                                                       backgroundColor, LCD_65K_RGB::PO_Fixed,
                                                       Text::FourDigitPercentFormat, ( int16_t) Value);
                }
                else
                {
                    int32_t Value = NormalizedCurrent * 100;
                    Value /= NormalizedMaximum;

                    GLOBAL.SetupDisplay.PrintFormat_P( percentageLeft, top, fontId, foregroundColor,
                                                       backgroundColor, LCD_65K_RGB::PO_Fixed,
                                                       Text::ThreeDigitPercentFormat, ( int16_t) Value);
                }
            }
            else
            {
                GLOBAL.SetupDisplay.Print_P( percentageLeft, top, fontId, foregroundColor,
                                             backgroundColor, LCD_65K_RGB::PO_Fixed,
                                             Text::EmptyDigitPercent);
            }
        }

        // Display value?
        if(( options & O_Value) && ( valueWidth != 0))
        {
            GLOBAL.SetupDisplay.PrintFormat_P( valueLeft, top, fontId, foregroundColor, backgroundColor,
                                               LCD_65K_RGB::PO_Fixed, Text::SixDigitInt16Format,
                                               Current);
        }

        // We need a minimum space for display.
        if(( gaugeWidth < 5) || ( height < 5))
        {
            return;
        }

        // Draw gauge frame.
        GLOBAL.SetupDisplay.DrawRect( gaugeLeft, top, gaugeWidth, height, foregroundColor,
                                      LCD_65K_RGB::RO_Rounded);
        GLOBAL.SetupDisplay.DrawRect( gaugeLeft + 1, top + 1, gaugeWidth - 2, height - 2,
                                      foregroundColor, LCD_65K_RGB::RO_Boxed);

        // Draw gauge inner frame.

        // Draw fill level.
        int32_t ValueSize = 0;

        if( NormalizedMaximum != 0)
        {
            if( options & O_Vertical)
            {
                ValueSize = NormalizedCurrent * ( fillHeight - 1) / NormalizedMaximum;
            }
            else
            {
                ValueSize = NormalizedCurrent * ( fillWidth - 1) / NormalizedMaximum;
            }
        }

        // Display marker or normal?
        if( options & O_Marker)
        {
            if( options & O_Vertical)
            {
                marker.Display( fillLeft + 1, fillTop + ValueSize);
            }
            else
            {
                marker.Display( fillLeft + ValueSize, fillTop + 1);
            }
        }
        else
        {
            if( options & O_Vertical)
            {
                GLOBAL.SetupDisplay.FillRect( fillLeft, fillTop, fillWidth, ValueSize, detailColor);
                GLOBAL.SetupDisplay.FillRect( fillLeft, fillTop + ValueSize, fillWidth,
                                              fillHeight - ValueSize, backgroundColor);
            }
            else
            {
                GLOBAL.SetupDisplay.FillRect( fillLeft, fillTop, ValueSize, fillHeight, detailColor);
                GLOBAL.SetupDisplay.FillRect( fillLeft + ValueSize, fillTop, fillWidth - ValueSize,
                                              fillHeight, backgroundColor);
            }
        }

        // Draw center line, if needed.
        if( options & O_CenterLine)
        {
            if( options & O_Vertical)
            {
                GLOBAL.SetupDisplay.DrawHorizontalLine( left + 1, centerPosition, width - 2, foregroundColor);
            }
            else
            {
                GLOBAL.SetupDisplay.DrawVerticalLine( centerPosition, top + 1, height - 2, foregroundColor);
            }
        }
    }

    // Clear gauge content.
    void Clear( void)
    {
        // Display marker or normal?
        if( options & O_Marker)
        {
            marker.Clear();
        }

        GLOBAL.SetupDisplay.FillRect( left, top, width, height, backgroundColor);

        remembered = false;
    /*
        // Draw.
        label.Display();

        // Display percentage?
        if(( options & O_Percentage) && ( percentageWidth != 0))
        {
            GLOBAL.SetupDisplay.Print_P( percentageLeft, top, fontId, foregroundColor, backgroundColor, LCD_65K_RGB::PO_Fixed,
                        "     ");
        }

        // Display value?
        if(( options & O_Value) && ( valueWidth != 0))
        {
            GLOBAL.SetupDisplay.Print_P( valueLeft, top, fontId, foregroundColor, backgroundColor, LCD_65K_RGB::PO_Fixed,
                        "      ");
        }

        // We need a minimum space for display.
        if(( gaugeWidth < 5) || ( height < 5))
        {
            return;
        }

        // Draw gauge frame.
        GLOBAL.SetupDisplay.DrawRect( gaugeLeft, top, gaugeWidth, height, foregroundColor, LCD_65K_RGB::RO_Rounded);

        // Draw gauge inner frame.
        GLOBAL.SetupDisplay.FillRect( gaugeLeft + 1, top + 1, gaugeWidth - 2, height - 2, backgroundColor);

        // Draw center line, if needed.
        if( options & O_CenterLine)
        {
            if( options & O_Vertical)
            {
                GLOBAL.SetupDisplay.DrawHorizontalLine( left + 1, centerPosition, width - 2, foregroundColor);
            }
            else
            {
                GLOBAL.SetupDisplay.DrawVerticalLine( centerPosition, top + 1, height - 2, foregroundColor);
            }
        }
    */
    }
};
