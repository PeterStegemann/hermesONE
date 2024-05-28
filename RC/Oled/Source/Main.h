// Copyright 2010 Peter Stegemann

#pragma once

#include "GraphicConnection.h"

#include "AVR/Source/LCD/S6E63D6_Parallel.h"
//#include "AVR/Components/LCD/S6E63D6_SPI.h"

class MAIN
{
  private:
    GraphicConnection< SYSTEM_SERIAL_ID> graphicConnection;
    LCD_S6E63D6_Parallel lcd;
//	LCD_S6E63D6_SPI Lcd;

    void doFly( void);
    void doFractal( void);
    void doIntro( void);

  public:
    MAIN( void);

    // Go.
    void Run( void);
};

extern MAIN Main;
