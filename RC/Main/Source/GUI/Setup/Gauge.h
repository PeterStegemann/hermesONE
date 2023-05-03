// Copyright 2007 Peter Stegemann

#pragma once

#include "GUI/Setup/Marker.h"

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
    void calculateInnerDimensions( void);

  public:
    GUI_Setup_Gauge( void);

    // Set the location of the gauge on the screen.
    void SetDimensions( uint16_t Left, uint16_t Top, uint16_t Width, uint16_t Height);

    // Set some display options.
    void SetOptions( Options UseOptions);

    // Set colors.
    void SetColors( LCD_65K_RGB::Color ForegroundColor, LCD_65K_RGB::Color BackgroundColor,
                    LCD_65K_RGB::Color DetailColor);
    void SetBackgroundColor( LCD_65K_RGB::Color BackgroundColor);
    void SetDetailColor( LCD_65K_RGB::Color DetailColor);

    // This will update the view to reflect the given values.
    void Display( void);
    void Display( int16_t Current);
    void Display( int16_t Minimum, int16_t Maximum, int16_t Value);
//		virtual void Display( int16_t Minimum, int16_t Maximum, int16_t Value);

    // Clear gauge content.
    void Clear( void);
};
