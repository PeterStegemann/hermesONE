// Copyright 2010 Peter Stegemann

#pragma once

#include "Ports.h"

#include "AVR/Source/Types.h"
#include "AVR/Source/Utility.h"
#include "AVR/Source/LCD/65K_RGB.h"
#include "AVR/Source/LCD/Serial/SerialOledProtocol.h"
#include "AVR/Source/Serial/Connection.h"

#define SERIAL_STRING_SIZE		5

#define STATUS_USED_BITS			UTILITY_BitValue( STATUS_GREEN) | UTILITY_BitValue( STATUS_RED)

#define STATUS_SetBit( Bit)			UTILITY_SetBit( STATUS_PORT, Bit)
#define STATUS_ClearBit( Bit)		UTILITY_ClearBit( STATUS_PORT, Bit)

template< uint8_t DeviceId>
class GraphicConnection : public avr::serial::Connection< DeviceId>
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

        STATUS_SetBit( STATUS_RED);

        lcd->SetBlanked( Blanked);

        STATUS_ClearBit( STATUS_RED);
    }

    void receiveClear( void)
    {
        uint16_t Color;

        STATUS_SetBit( STATUS_RED);

        this->ReceiveWord( &Color);

        lcd->Clear(( LCD_65K_RGB::Color) Color);

        STATUS_ClearBit( STATUS_RED);
    }

    void receiveDrawPixel( void)
    {
        uint16_t Left, Top, Color;

        this->ReceiveWord( &Left);
        this->ReceiveWord( &Top);
        this->ReceiveWord( &Color);

        STATUS_SetBit( STATUS_RED);

        lcd->DrawPixel( Left, Top, ( LCD_65K_RGB::Color) Color);

        STATUS_ClearBit( STATUS_RED);
    }

    void receiveDrawLine( void)
    {
        uint16_t StartX, StartY, StopX, StopY, Color;

        this->ReceiveWord( &StartX);
        this->ReceiveWord( &StartY);
        this->ReceiveWord( &StopX);
        this->ReceiveWord( &StopY);
        this->ReceiveWord( &Color);

        STATUS_SetBit( STATUS_RED);

        lcd->DrawLine( StartX, StartY, StopX, StopY, ( LCD_65K_RGB::Color) Color);

        STATUS_ClearBit( STATUS_RED);
    }

    void receiveDrawRect( void)
    {
        uint16_t Left, Top, Width, Height, Color;

        this->ReceiveWord( &Left);
        this->ReceiveWord( &Top);
        this->ReceiveWord( &Width);
        this->ReceiveWord( &Height);
        this->ReceiveWord( &Color);

        STATUS_SetBit( STATUS_RED);

        lcd->DrawRect( Left, Top, Width, Height, ( LCD_65K_RGB::Color) Color);

        STATUS_ClearBit( STATUS_RED);
    }

    void receiveFillRect( void)
    {
        uint16_t Left, Top, Width, Height, Color;

        this->ReceiveWord( &Left);
        this->ReceiveWord( &Top);
        this->ReceiveWord( &Width);
        this->ReceiveWord( &Height);
        this->ReceiveWord( &Color);

        STATUS_SetBit( STATUS_RED);

        lcd->FillRect( Left, Top, Width, Height, ( LCD_65K_RGB::Color) Color);

        STATUS_ClearBit( STATUS_RED);
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

            uint16_t Length = lcd->Print
            (
                Left, Top, ( avr::font::FontId) FontId,
                ( LCD_65K_RGB::Color) ForegroundColor, ( LCD_65K_RGB::Color) BackgroundColor,
                ( LCD::PrintOptions) UsePrintOptions, StringBuffer
            );

            Left += Length;
        }
        while( FetchMore);
    }

  public:
    virtual ~GraphicConnection( void)
    {
    }

    void Initialize( LCD_65K_RGB* lcd)
    {
        avr::serial::Connection< DeviceId>::Initialize();

        this->lcd = lcd;
    }

    void DoSerialConnection( void)
    {
        STATUS_ClearBit( STATUS_RED);

        while( true)
        {
            // Receive command.
//				if( ReadyToReceive())
            {
                uint8_t Command;

                this->ReceiveByte( &Command);

                switch( Command)
                {
                    case LCD_SerialOledProtocol::I_Ping : break;

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
        }
    }
};
