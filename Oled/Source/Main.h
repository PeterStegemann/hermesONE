// Copyright 2010 Peter Stegemann

#pragma once

#include "Serial/Serial_GraphicConnection.h"

#include "AVR/Source/LCD/LCD_S6E63D6_Parallel.h"
//#include "AVR/Components/LCD/LCD_S6E63D6_SPI.h"

class MAIN
{
	public:
		SERIAL_GraphicConnection< SYSTEM_SERIAL_ID> SerialGraphicConnection;
		LCD_S6E63D6_Parallel Lcd;
//		LCD_S6E63D6_SPI Lcd;

	private:
		void doFly( void);
		void doFractal( void);

	public:
		MAIN( void);

		// Go.
		void Run( void);
};

extern MAIN Main;
