// Copyright 2007 Peter Stegemann

#pragma once

#include "AVR/Source/LCD/LCD_65K_RGB.h"

class GUI_Setup_Marker
{
  public:
    enum Options
    {
        O_None				= 0b00000000,
        O_LookRight			= 0b00000001,
        O_LookUp			= 0b00000010,
        O_LookLeft			= 0b00000100,
        O_LookDown			= 0b00001000
    };

  private:
    uint16_t areaLeft, areaTop, areaRight, areaBottom;

    Options options;

    uint8_t size;

    // Marks whether we have remembered values from last draw.
    bool remembered;
    // Current marker position.
    uint16_t left, top;

    // Internal display with a "print" or "clear" mode.
    enum ClearOrPrintMode
    {
        COPM_Clear,
        COPM_Print
    };

    // With print, the marker is shown. With clear, the marker will be removed.
    void display( ClearOrPrintMode ClearOrPrint);

  public:
    LCD_65K_RGB::Color ForegroundColor;
    LCD_65K_RGB::Color BackgroundColor;

  public:
    GUI_Setup_Marker( void);

    // Set the location of the marker area on the screen.
    void SetArea( uint16_t Left, uint16_t Top, uint16_t Width, uint16_t Height);

    // Set some display options.
    void SetOptions( Options UseOptions);

    // Set the size of the marker.
    void SetSize( uint8_t Size);

    // Set the position.
    void SetPosition( uint16_t Left, uint16_t Top);

    // This will draw the marker.
    void Display( uint16_t left, uint16_t top);
    void Display( void);

    void Clear( void);
};
