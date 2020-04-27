// Copyright 2010 Peter Stegemann

#pragma once

#include "Ports.h"

#include "AVR/Components/Types.h"
#include "AVR/Components/Utility.h"
#include "AVR/Components/LCD/LCD_65K_RGB.h"
#include "AVR/Components/LCD/LCD_SerialOledProtocol.h"
#include "AVR/Components/Serial/Serial_Connection.h"

#define SERIAL_STRING_SIZE		5

#define STATUS_USED_BITS			UTILITY_BitValue( STATUS_GREEN) | UTILITY_BitValue( STATUS_RED)

#define STATUS_SetBit( Bit)			UTILITY_SetBit( STATUS_PORT, Bit)
#define STATUS_ClearBit( Bit)		UTILITY_ClearBit( STATUS_PORT, Bit)

template< uint8_t DeviceId>
class SERIAL_GraphicConnection : public SERIAL_Connection< DeviceId>
{
	private:
		LCD_65K_RGB* lcd;

		void receiveBacklight( void)
		{
			uint8_t Backlight;

			this->ReceiveByte( &Backlight);

			lcd->SetBacklight( Backlight);
		}

		void receiveBlank( void)
		{
			bool Blanked;

			this->ReceiveBoolean( &Blanked);

			lcd->SetBlanked( Blanked);
		}

		void receiveClear( void)
		{
			uint16_t Color;

			this->ReceiveWord( &Color);

			lcd->Clear(( LCD_65K_RGB::Color) Color);

		//	lcd->PrintFormat( 200, 0, FONT::FID_Mini, LCD_65K_RGB::White, LCD_65K_RGB::Black,
		//					  LCD::PO_Proportional, "Clear %x", Color);
		}

		void receiveDrawPixel( void)
		{
			uint16_t Left, Top, Color;

			this->ReceiveWord( &Left);
			this->ReceiveWord( &Top);
			this->ReceiveWord( &Color);

			lcd->DrawPixel( Left, Top, ( LCD_65K_RGB::Color) Color);
		}

		void receiveDrawLine( void)
		{
			uint16_t StartX, StartY, StopX, StopY, Color;

			this->ReceiveWord( &StartX);
			this->ReceiveWord( &StartY);
			this->ReceiveWord( &StopX);
			this->ReceiveWord( &StopY);
			this->ReceiveWord( &Color);

			lcd->DrawLine( StartX, StartY, StopX, StopY, ( LCD_65K_RGB::Color) Color);
		}

		void receiveDrawRect( void)
		{
			uint16_t Left, Top, Width, Height, Color;

			this->ReceiveWord( &Left);
			this->ReceiveWord( &Top);
			this->ReceiveWord( &Width);
			this->ReceiveWord( &Height);
			this->ReceiveWord( &Color);

			lcd->DrawRect( Left, Top, Width, Height, ( LCD_65K_RGB::Color) Color);
		}

		void receiveFillRect( void)
		{
			uint16_t Left, Top, Width, Height, Color;

			this->ReceiveWord( &Left);
			this->ReceiveWord( &Top);
			this->ReceiveWord( &Width);
			this->ReceiveWord( &Height);
			this->ReceiveWord( &Color);

			lcd->FillRect( Left, Top, Width, Height, ( LCD_65K_RGB::Color) Color);
		}

		void receivePrint( void)
		{
			char StringBuffer[ SERIAL_STRING_SIZE];

			uint16_t Left, Top, ForegroundColor, BackgroundColor;
			uint8_t FontId, UsePrintOptions;

			this->ReceiveWord( &Left);
			this->ReceiveWord( &Top);
			this->ReceiveByte( &FontId);
			this->ReceiveWord( &ForegroundColor);
			this->ReceiveWord( &BackgroundColor);
			this->ReceiveByte( &UsePrintOptions);

			bool FetchMore;

			do
			{
				FetchMore = this->ReceiveString( StringBuffer, SERIAL_STRING_SIZE);

				uint16_t Length =
					lcd->Print( Left, Top, ( FONT::FontId) FontId,
								( LCD_65K_RGB::Color) ForegroundColor,
								( LCD_65K_RGB::Color) BackgroundColor,
								( LCD::PrintOptions) UsePrintOptions,
								StringBuffer);

				Left += Length;
			}
			while( FetchMore);
		}

	public:
		virtual ~SERIAL_GraphicConnection( void)
		{
		}

		void Initialize( LCD_65K_RGB* lcd)
		{
			SERIAL_Connection< DeviceId>::Initialize();

			this->lcd = lcd;
		}

		void DoSerialConnection( void)
		{
			while( true)
			{
				// Receive command.
//				if( ReadyToReceive())
				{
					uint8_t Command;

					STATUS_ClearBit( STATUS_RED);

					this->ReceiveByte( &Command);

					STATUS_SetBit( STATUS_RED);

					switch( Command)
					{
						case LCD_SerialOledProtocol::I_Backlight :
						{
							receiveBacklight();
						}
						break;

						case LCD_SerialOledProtocol::I_Blank :
						{
							receiveBlank();
						}
						break;

						case LCD_SerialOledProtocol::I_Clear :
						{
							receiveClear();
						}
						break;

						case LCD_SerialOledProtocol::I_DrawPixel :
						{
							receiveDrawPixel();
						}
						break;

						case LCD_SerialOledProtocol::I_DrawLine :
						{
							receiveDrawLine();
						}
						break;

						case LCD_SerialOledProtocol::I_DrawRect :
						{
							receiveDrawRect();
						}
						break;

						case LCD_SerialOledProtocol::I_FillRect :
						{
							receiveFillRect();
						}
						break;

						case LCD_SerialOledProtocol::I_Print :
						{
							receivePrint();
						}
						break;

						default : break;
					}
				}

//				UTILITY::Pause( 5);
			}
		}
};
