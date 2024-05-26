// Copyright 2007 Peter Stegemann

#include "Base.h"

#include "Ports.h"
#include "GUI/Setup/Label.h"
#include "Main/Global.h"
#include "Setup/Defines.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

#include <string.h>

Screen_Setup_Base::Screen_Setup_Base( uint32_t MenuPattern, const flash_char* Title, bool ManageMenuMarker)
				 : Screen_Base( MenuPattern)
				 , manageMenuMarker( ManageMenuMarker)
				 , title( Title)
				 , lastSequence( 0)
				 , frameLeft( 0)
				 , frameTop( 0)
				 , frameWidth( GLOBAL.SetupDisplay.GetWidth())
				 , frameHeight( GLOBAL.SetupDisplay.GetHeight())
{
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	menuMarker.SetOptions( GUI_Setup_Marker::O_Block);
	menuMarker.SetSize( SCREEN_SETUP_BASE_MENU_MARKER_WIDTH, Font->CellHeight);
	menuMarker.ForegroundColor = LCD_65K_RGB::C_WarmYellow;
}

Screen_Setup_Base::~Screen_Setup_Base( void)
{
}

extern unsigned int __data_start;
extern unsigned int __data_end;
extern unsigned int __bss_start;
extern unsigned int __bss_end;
extern unsigned int __noinit_start;
extern unsigned int __noinit_end;
extern unsigned int __heap_start;
extern unsigned int __heap_end;

void Screen_Setup_Base::update( void)
{
	GLOBAL.StatusScreen.Update();

    if( GLOBAL.GetDebug() == false)
    {
        return;
    }

	uint16_t Sequence = GLOBAL.SignalService.GetPPM( 0)->GetSequence();

	if( Sequence != lastSequence)
	{
    	lastSequence = Sequence;

    	uint16_t Left = frameLeft + frameWidth - 60;
    	uint16_t Top = frameTop + 1;

		GLOBAL.SetupDisplay.PrintFormat
		(
		    Left, Top, FONT::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "Stack: %04x", RAMEND - SP
        );
		GLOBAL.SetupDisplay.PrintFormat
		(
		    Left, Top += 10, FONT::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "Free:  %04x", SP - __data_start
        );
/*
		GLOBAL.SetupDisplay.PrintFormat
		(
		    Left, Top, FONT::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
		    "data:   %04x %04x", __data_start, __data_end
        );
		GLOBAL.SetupDisplay.PrintFormat
		(
    		Left, Top += 10, FONT::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "bss:    %04x %04x", __bss_start, __bss_end
        );
		GLOBAL.SetupDisplay.PrintFormat
		(
    		Left, Top += 10, FONT::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "noinit: %04x %04x", __noinit_start, __noinit_end
        );
		GLOBAL.SetupDisplay.PrintFormat
		(
    		Left, Top += 10, FONT::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "heap:   %04x %04x", __heap_start, __heap_end
        );
		GLOBAL.SetupDisplay.PrintFormat
		(
		    Left, Top += 10, FONT::FI_Mini, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "RAMEND: %04x SP: %04x size: %04x", RAMEND, SP, RAMEND - SP
        );
*/
/*
		//  	uint8_t Diff = Sequence - lastSequence;
        //		uint8_t Frames = 450 / Diff;

        GLOBAL.SetupDisplay.PrintFormat( frameLeft + 250, frameTop + 1, FONT::FI_Mini,
										 LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
										 "%u %d %d.%01d  ", Sequence,
										 GLOBAL.SignalService.GetPPM( 0)->GetWaited(), Frames / 10,
										 Frames % 10);
*/
		//		UTILITY::PrintByteBits( &( GLOBAL.SetupDisplay), frameLeft + 250, frameTop + 10, "A: ", PINA);
	}
}

void Screen_Setup_Base::staticUpdate( void* Object)
{
	(( Screen_Setup_Base*) Object)->update();
}

void Screen_Setup_Base::drawFrame( void)
{
	if( title == NULL)
	{
		return;
	}

	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_TITLE_FONT);

//	GLOBAL.SetupDisplay.DrawRect( 0, 0, GLOBAL.SetupDisplay.GetWidth(), GLOBAL.SetupDisplay.GetHeight(),
//						   LCD_65K_RGB::C_White, LCD::RO_Rounded);

	uint16_t LabelLeft = GLOBAL.SetupDisplay.GetWidth() / 3;
	uint16_t LabelWidth = LabelLeft;

	GLOBAL.SetupDisplay.FillRect( LabelLeft, 0, LabelWidth, Font->CellHeight + 1, LCD_65K_RGB::C_White);
	GLOBAL.SetupDisplay.DrawHorizontalLine( LabelLeft + 1, Font->CellHeight + 1, LabelWidth - 2, LCD_65K_RGB::C_White);

	uint16_t TitleWidth = strlen_P( title) * Font->CellWidth;
	uint16_t TextLeft = ( GLOBAL.SetupDisplay.GetWidth() - TitleWidth) / 2;

	TitleWidth = GLOBAL.SetupDisplay.Print_P
	(
	    TextLeft, 1, SCREEN_SETUP_BASE_TITLE_FONT, LCD_65K_RGB::C_Black, LCD_65K_RGB::C_White,
        LCD::PO_Proportional, title
    );

//	GLOBAL.SetupDisplay.DrawVerticalLine( TextLeft - 1, 1, Font->CellHeight, LCD_65K_RGB::C_White);
/*
	GLOBAL.SetupDisplay.DrawHorizontalLine( TextLeft + 1, 1 + Font->CellHeight + 1, TitleWidth - 2,
									    LCD_65K_RGB::C_Red);
	GLOBAL.SetupDisplay.DrawVerticalLine( TextLeft + TitleWidth + 1, 1, Font->CellHeight - 1,
								   LCD_65K_RGB::C_Red);
*/
	frameLeft = 0;
//	frameTop = Font->CellHeight + 2;
	frameTop = 1;
	frameWidth = GLOBAL.SetupDisplay.GetWidth() - frameLeft;
	frameHeight = GLOBAL.SetupDisplay.GetHeight() - frameTop;

	markerLeft = frameLeft;
	markerTop = frameTop + ( SCREEN_SETUP_BASE_LINE_HEIGHT / 2) - 1;

	menuMarker.SetArea( markerLeft, frameTop, SCREEN_SETUP_BASE_MENU_MARKER_WIDTH, frameHeight);

	menuLeft = markerLeft + SCREEN_SETUP_BASE_MENU_MARKER_WIDTH + 2;
}

void Screen_Setup_Base::drawMenuMarker( void)
{
	if( manageMenuMarker)
	{
		menuMarker.Display( markerLeft, markerTop + ( currentMenuEntry * 
													  SCREEN_SETUP_BASE_LINE_HEIGHT));
	}
}

void Screen_Setup_Base::SetSourceLabel( GUI_Setup_Label* Label, char* Name, uint8_t SignalSourceId)
{
	Label->Clear();

	if( SignalSourceId == SIGNAL_SOURCE_NONE)
	{
		Label->SetText_P( Text::None);
	}
	else if( SignalSourceId == SIGNAL_SOURCE_FIXED)
	{
		Label->SetText_P( Text::Fixed);
	}
	else
	{
		const Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

		if( Source->GetType() == Signal_Source_Source::T_Empty)
		{
			Label->SetText_P( Text::Empty);
		}
		else
		{
			GLOBAL.SetupService.GetSourceName( Source->GetSetupSourceId(), Name, 
											   SETUP_SOURCE_NAME_SIZE + 1);
			Label->SetText( Name);
		}
	}

	Label->Display();
}

void Screen_Setup_Base::ReDisplay( void)
{
	GLOBAL.SetupDisplay.Clear( LCD_65K_RGB::C_Black);

	drawFrame();
	display();
	drawMenuMarker();
}
