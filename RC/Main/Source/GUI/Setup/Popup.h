// Copyright 2008 Peter Stegemann

#pragma once

#include "Defines.h"

#include "AVR/Source/Types.h"
#include "AVR/Source/LCD/65K_RGB.h"

class GUI_Setup_Popup
{
  private:
    avr::font::FontId fontId;

    const char* text;
    const flash_char* text_P;
    const flash_char* okText_P;
    const flash_char* cancelText_P;

    uint16_t buttonTop, okLeft, cancelLeft;

  public:
    LCD_65K_RGB::Color ForegroundColor;
    LCD_65K_RGB::Color BackgroundColor;

  public:
    GUI_Setup_Popup( void)
        : fontId( GUI_SETUP_MAIN_FONT)
        , text( NULL)
        , text_P( NULL)
        , okText_P( NULL)
	    , cancelText_P( NULL)
		, ForegroundColor( LCD_65K_RGB::C_White)
		, BackgroundColor( LCD_65K_RGB::C_Blue)
    {
    }

    // Set text.
    void SetText( const char* Text)
    {
        text = Text;
        text_P = NULL;
    }

    void SetText_P( const flash_char* Text)
    {
        text_P = Text;
        text = NULL;
    }

    void SetOKText_P( const flash_char* OKText)
    {
        okText_P = OKText;
    }

    void SetCancelText_P( const flash_char* CancelText)
    {
        cancelText_P = CancelText;
    }

    // This will show the popup.
    void Show( void)
    {
        if(( text == NULL) && ( text_P == NULL))
        {
            return;
        }

        const avr::font::Type* Font = avr::font::Font::Get( fontId);

        uint16_t TextWidth;

        if( text != NULL)
        {
        	TextWidth = strlen( text) * Font->GetCellWidth();
        }
        else
        {
            TextWidth = strlen_P( text_P) * Font->GetCellWidth();
        }

        uint16_t OKTextWidth = 0;
        uint16_t CancelTextWidth = 0;

        if( okText_P != NULL)
        {
            OKTextWidth = strlen_P( okText_P) * Font->GetCellWidth();
        }

        if( cancelText_P != NULL)
        {
        	CancelTextWidth = strlen_P( cancelText_P) * Font->GetCellWidth();
        }

        uint16_t Width;

        if( TextWidth > ( OKTextWidth + CancelTextWidth + Font->GetCellWidth()))
        {
            Width = TextWidth;
        }
        else
        {
            Width = OKTextWidth + CancelTextWidth + Font->GetCellWidth();
        }

        Width += 4;

        uint16_t Height;

        if(( okText_P == NULL) && ( cancelText_P == NULL))
        {
            Height = ( 1 * Font->GetCellHeight()) + 4;
        }
        else
        {
            Height = ( 3 * Font->GetCellHeight()) + 4;
        }

        uint16_t Left = ( GLOBAL.SetupDisplay.GetWidth() - Width) / 2;
        uint16_t Top = ( GLOBAL.SetupDisplay.GetHeight() - Height) / 2;

        // Clear area.
        GLOBAL.SetupDisplay.FillRect( Left + 1, Top + 1, Width - 2, Height - 2, BackgroundColor);
        GLOBAL.SetupDisplay.DrawRect( Left, Top, Width, Height, ForegroundColor, LCD_65K_RGB::RO_Rounded);

        Top += 2;
        Left += 1;

        if( text != NULL)
        {
            GLOBAL.SetupDisplay.Print( Left + (( Width - TextWidth) / 2), Top, fontId,
                ForegroundColor, BackgroundColor, LCD::PO_Proportional, text);
        }
        else
        {
            GLOBAL.SetupDisplay.Print_P( Left + (( Width - TextWidth) / 2), Top, fontId,
                ForegroundColor, BackgroundColor, LCD::PO_Proportional, text_P);
        }

        Top += 1;
        Left += 2;

        buttonTop = Top + 2 * Font->GetCellHeight();

        if(( okText_P != NULL) && ( cancelText_P != NULL))
        {
            okLeft = Left;
            cancelLeft = Left + Width - 4 - CancelTextWidth;

            GLOBAL.SetupDisplay.Print_P( okLeft, buttonTop, fontId,
                ForegroundColor, BackgroundColor, LCD::PO_Proportional, okText_P);
            GLOBAL.SetupDisplay.Print_P( cancelLeft, buttonTop, fontId,
                ForegroundColor, BackgroundColor, LCD::PO_Proportional, cancelText_P);
        }
        else
        {
            okLeft = cancelLeft = Left + (( Width - ( OKTextWidth + CancelTextWidth)) / 2);

            if( okText_P != NULL)
            {
                GLOBAL.SetupDisplay.Print_P( okLeft, buttonTop, fontId,
                    LCD_65K_RGB::C_Red, BackgroundColor, LCD::PO_Proportional, okText_P);
            }
            else if( cancelText_P != NULL)
            {
                GLOBAL.SetupDisplay.Print_P( cancelLeft, buttonTop, fontId,
                    LCD_65K_RGB::C_Red, BackgroundColor, LCD::PO_Proportional, cancelText_P);
            }
        }
    }

    // This will get the user input.
    bool Ask( void)
    {
        // Make sure button is released before we start.
        while( true)
        {
            bool RotaryCurrentButton;
            uint8_t RotaryButton;

            // Try rotary. Also get the RotaryButton to clear it. Otherwise, this popup would always be accepted on the
            // initial button release.
            GLOBAL.InputService.GetRotary( NULL, &RotaryButton, &RotaryCurrentButton);

            if( RotaryCurrentButton == true)
            {
                avr::Utility::Pause( 5);
            }
            else
            {
                break;
            }
        }

        // Paint initial select red.
        if( okText_P != NULL)
        {
            GLOBAL.SetupDisplay.Print_P( okLeft, buttonTop, fontId,
                LCD_65K_RGB::C_Red, BackgroundColor, LCD::PO_Proportional, okText_P);
        }
        else if( cancelText_P != NULL)
        {
            GLOBAL.SetupDisplay.Print_P( cancelLeft, buttonTop, fontId,
                LCD_65K_RGB::C_Red, BackgroundColor, LCD::PO_Proportional, cancelText_P);
        }

        bool Choice = true;
    /*
        if(( okText != NULL) && ( cancelText != NULL))
        {
            Choice = false;
        }
        else
        {
            Choice = true;
        }
    */
        while( true)
        {
            // Try rotary.
            int8_t RotarySelect;
            uint8_t RotaryButton;

            GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

            if(( okText_P != NULL) && ( cancelText_P != NULL) && ( RotarySelect != 0))
            {
                if( RotarySelect < 0)
                {
                    Choice = true;

                    GLOBAL.SetupDisplay.Print_P( okLeft, buttonTop, fontId,
                        LCD_65K_RGB::C_Red, BackgroundColor, LCD::PO_Proportional, okText_P);
                    GLOBAL.SetupDisplay.Print_P( cancelLeft, buttonTop, fontId,
                        ForegroundColor, BackgroundColor, LCD::PO_Proportional, cancelText_P);
                }
                else
                {
                    Choice = false;

                    GLOBAL.SetupDisplay.Print_P( okLeft, buttonTop, fontId,
                        ForegroundColor, BackgroundColor, LCD::PO_Proportional, okText_P);
                    GLOBAL.SetupDisplay.Print_P( cancelLeft, buttonTop, fontId,
                        LCD_65K_RGB::C_Red, BackgroundColor, LCD::PO_Proportional, cancelText_P);
                }
            }

            if( RotaryButton > 0)
            {
                return( Choice);
            }

            avr::Utility::Pause( 5);
        }
    }
};
