// Copyright 2010 Peter Stegemann

#include "Main.h"

#include "Ports.h"
#include "System.h"

#include "AVR/Components/Types.h"
#include "AVR/Components/Utility.h"

#include <avr/interrupt.h>
#include <avr/wdt.h>

#define STATUS_USED_BITS			UTILITY_BitValue( STATUS_GREEN) | UTILITY_BitValue( STATUS_RED)

#define STATUS_SetBit( Bit)			UTILITY_SetBit( STATUS_PORT, Bit)
#define STATUS_ClearBit( Bit)		UTILITY_ClearBit( STATUS_PORT, Bit)

MAIN Main;

uint8_t ResetReason __attribute__ (( section ( ".noinit")));

void ResetWatchdog( void) __attribute__(( naked)) __attribute__(( section( ".init3")));

void ResetWatchdog( void)
{
	ResetReason = MCUSR;
	MCUSR = 0;
	wdt_disable();
}

int main( void)
{
	// Disable watchdog.
	wdt_disable();

	Main.Run();

	return( 0);
}

MAIN::MAIN( void)
{
}

void MAIN::Run( void)
{
	// All Ports to output
	STATUS_DDR |= STATUS_USED_BITS;

	STATUS_SetBit( STATUS_RED);
	STATUS_ClearBit( STATUS_GREEN);

	// Enable interrupts.
	sei();

	// Set up main lcd.
	Lcd.Initialize();

	STATUS_SetBit( STATUS_GREEN);

	Lcd.SetOrientation(( LCD::Orientation)( LCD::O_Landscape));

	// Set up serial graphic connection.
	SerialGraphicConnection.Initialize( &Lcd);

	// Wait a moment to get all services going.
	UTILITY::Pause( 5);

	STATUS_ClearBit( STATUS_RED);

	//doFly();
	//doFractal();

	// Listen to the serial master.
	SerialGraphicConnection.DoSerialConnection();
/*
	Main.Lcd.Print( 50, 50, FONT::FID_Medium, LCD_65K_RGB::White, LCD_65K_RGB::Black,
				    LCD::PO_Fixed, "Test");

	char* Byte = "A"; // FONT::C_FirstTotal;
	uint16_t Count = 0;

	while( true)
	{
//		uint8_t Byte = SerialGraphicConnection.ReceiveByte( SERIAL::TO_NonBlock);
//		SerialGraphicConnection.SendByte( Byte, SERIAL::TO_NonBlock);

//		uint8_t Byte = SerialGraphicConnection.ReceiveByte();
//		SerialGraphicConnection.SendByte( Byte);

		uint16_t X = 50; //( Count % 10) * 6;
		uint16_t Y = 50; //(( Count / 10) % 8) * 5;

		Main.Lcd.Print( X, Y, FONT::FID_Large, LCD_65K_RGB::White, LCD_65K_RGB::Black,
						LCD::PO_Fixed, Byte);

		Count++;

		if( Byte == FONT::C_LastTotal)
		{
			Byte = FONT::C_FirstTotal;
			Count = 0;
		}
		else
		{
			Byte++;
			Count++;
		}
	}
*/
/*
	// Loop forever.
	while( true)
	{
		//
		Main.Lcd.SetOrientation(( LCD::Orientation)( LCD::O_Landscape));

		Main.Lcd.FillRect( 20, 10, 10, 10, LCD_65K_RGB::White);

		Main.Lcd.Print( 50, 50, FONT::FID_Large, LCD_65K_RGB::White, LCD_65K_RGB::Black,
					    LCD::PO_Fixed, "Landscape");

//		Main.Lcd.Print( 50, 50, FONT::FID_Large, LCD_65K_RGB::Black, LCD_65K_RGB::Black,
//					    LCD::PO_Proportional, "Landscape");
		//
		Main.Lcd.SetOrientation(( LCD::Orientation)( LCD::O_Landscape |
													 LCD::O_HorizontalFlip));

		Main.Lcd.FillRect( 20, 10, 10, 10, LCD_65K_RGB::Red);

		Main.Lcd.Print( 50, 50, FONT::FID_Large, LCD_65K_RGB::White, LCD_65K_RGB::Black,
					    LCD::PO_Fixed, "Landscape hor flip");

//		Main.Lcd.Print( 50, 50, FONT::FID_Large, LCD_65K_RGB::Black, LCD_65K_RGB::Black,
//					   LCD::PO_Proportional, "Landscape hor flip");

		//
		Main.Lcd.SetOrientation(( LCD::Orientation)( LCD::O_Landscape |
													 LCD::O_VerticalFlip));

		Main.Lcd.FillRect( 20, 10, 10, 10, LCD_65K_RGB::Green);

		Main.Lcd.Print( 50, 50, FONT::FID_Large, LCD_65K_RGB::White, LCD_65K_RGB::Black,
						LCD::PO_Fixed, "Landscape vert flip");

//		Main.Lcd.Print( 50, 50, FONT::FID_Large, LCD_65K_RGB::Black, LCD_65K_RGB::Black,
//						LCD::PO_Proportional, "Landscape vert flip");

		//
		Main.Lcd.SetOrientation(( LCD::Orientation)( LCD::O_Landscape |
													 LCD::O_VerticalFlip | LCD::O_HorizontalFlip));

		Main.Lcd.FillRect( 20, 10, 10, 10, LCD_65K_RGB::Blue);

		Main.Lcd.Print( 50, 50, FONT::FID_Large, LCD_65K_RGB::White, LCD_65K_RGB::Black,
					    LCD::PO_Fixed, "Landscape vert hor flip");

//		Main.Lcd.Print( 50, 50, FONT::FID_Large, LCD_65K_RGB::Black, LCD_65K_RGB::Black,
//					   LCD::PO_Proportional, "Landscape vert hor flip");

		Main.Lcd.Clear( LCD_65K_RGB::Black);
	}
*/
/*
		for( uint8_t Grey = 0; Grey <= 0x1f; Grey++)
		{
			Main.Lcd.Clear( LCD_65K_RGB_GREY( Grey));
		}
*/
/*
		for( uint16_t HeightCount = 0; HeightCount <= Main.Lcd.GetHeight(); HeightCount++)
		{
			for( uint16_t WidthCount = 0; WidthCount <= Main.Lcd.GetWidth(); WidthCount++)
			{
				Main.Lcd.DrawPixel( WidthCount, HeightCount,
								    LCD_65K_RGB_COLOR( WidthCount / 9.97,
													   HeightCount / 3.74,
													   ( 239 - HeightCount) / 7.48));
			}
		}

//		continue;
*/
}

void MAIN::doFly( void)
{
	int16_t X = 24;
	int16_t XDir = -2;
	int16_t Y = 15;
	int16_t YDir = -1;

	int16_t X2 = 150;
	int16_t X2Dir = +1;
	int16_t Y2 = 64;
	int16_t Y2Dir = +2;

	uint16_t CurrentColor = 0;

	while( true)
	{
		//		UTILITY::Pause( 100);

		if(( X == (( int16_t) Main.Lcd.GetWidth() - 2)) || ( X == 0))
		{
			XDir *= -1;
		}

		if(( Y == (( int16_t) Main.Lcd.GetHeight() - 1)) || ( Y == 0))
		{
			YDir *= -1;
		}

		if(( X2 == (( int16_t) Main.Lcd.GetWidth() - 1)) || ( X2 == 0))
		{
			X2Dir *= -1;
		}
			
		if(( Y2 == (( int16_t) Main.Lcd.GetHeight() - 2)) || ( Y2 == 0))
		{
			Y2Dir *= -1;
		}

		//		Main.Lcd.FillRect( X, Y, -10, 1, LCD_65K_RGB::Black, LCD::RO_None);
		//		Main.Lcd.DrawLine( X, Y, X - 10, Y, LCD_65K_RGB::Black);

		//		Main.Lcd.DrawLine( X, Y, X2, Y2, LCD_65K_RGB::Black);

		X += XDir;
		Y += YDir;
		X2 += X2Dir;
		Y2 += Y2Dir;

		//		Main.Lcd.FillRect( X, Y, -10, 1, LCD_65K_RGB::Yellow, LCD::RO_None);
		//		Main.Lcd.DrawLine( X, Y, X - 10, Y, LCD_65K_RGB::Yellow);

		CurrentColor = LCD_65K_RGB_COLOR( X, Y, X2); //  X | Y << 6 | X2 << 11; 
		Main.Lcd.DrawLine( X, Y, X2, Y2, ( LCD_65K_RGB::Color) CurrentColor);

		//		CurrentColor++;
	}
}

void MAIN::doFractal( void)
{
	uint16_t resolutionWidth = 320;
	uint16_t resolutionHeight = 240;
	uint16_t maximumIteration = 256;

	float centerX = -0.74;
	float centerY = 0.15;
	float height = 1.1;

	// Set new dimensions.
	float width = height * resolutionWidth / resolutionHeight;

	float cornerX = centerX - width / 2;
	float cornerY = centerY - height / 2;

	uint16_t X = 0;

	while( X < resolutionWidth)
	{
		uint16_t Y = 0;

		while( Y < resolutionHeight)
		{
			// Calculate point.
			float Zr, Zi, Cr, Ci, ZrSqr, ZiSqr;
			Cr = Zr = cornerX + width * X / resolutionWidth;
			Ci = Zi = cornerY + height * Y / resolutionHeight;

			uint64_t CurrentIteration = 0;

			while( CurrentIteration != maximumIteration)
			{
				ZrSqr = Zr * Zr;
				ZiSqr = Zi * Zi;

				if(( ZrSqr + ZiSqr) >= 16.0)
				{
					break;
				}

				float ZrNew = ZrSqr - ZiSqr + Cr;
				Zi = 2 * Zr * Zi + Ci;
				Zr = ZrNew;

				CurrentIteration++;
			}
			
			Lcd.DrawPixel( X, Y, LCD_65K_RGB_COLOR( CurrentIteration & 7,
													CurrentIteration & 192,
													CurrentIteration & 63));

			Y++;
		}

		X++;
	}
}
