// Copyright 2007 Peter Stegemann

#include "Base.h"

#include "Ports.h"
#include "GUI/Setup/Label.h"
#include "Main/Global.h"
#include "Setup/Defines.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

#include <string.h>

Screen_Setup_Base::Screen_Setup_Base
(
    Input_Service* InputService,
    Screen_Status_Status* StatusScreen,
    uint32_t MenuPattern,
    const flash_char* Title,
    bool ManageMenuMarker
)
     : Screen_Base( InputService, MenuPattern)
     , manageMenuMarker( ManageMenuMarker)
     , title( Title)
     , statusScreen( StatusScreen)
     , frameLeft( 0)
     , frameTop( 0)
     , frameWidth( GLOBAL.SetupDisplay.GetWidth())
     , frameHeight( GLOBAL.SetupDisplay.GetHeight())
{
	const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_MAIN_FONT);

	menuMarker.SetOptions( GUI_Setup_Marker::O_Block);
	menuMarker.SetSize( SCREEN_SETUP_BASE_MENU_MARKER_WIDTH, Font->GetCellHeight());
	menuMarker.ForegroundColor = LCD_65K_RGB::C_WarmYellow;
}

Screen_Setup_Base::~Screen_Setup_Base( void)
{
}

void Screen_Setup_Base::update( void)
{
	statusScreen->Update();

    if( GLOBAL.GetDebug() == false)
    {
        return;
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

	const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_TITLE_FONT);

//	GLOBAL.SetupDisplay.DrawRect( 0, 0, GLOBAL.SetupDisplay.GetWidth(), GLOBAL.SetupDisplay.GetHeight(),
//						   LCD_65K_RGB::C_White, LCD::RO_Rounded);

	uint16_t LabelLeft = GLOBAL.SetupDisplay.GetWidth() / 3;
	uint16_t LabelWidth = LabelLeft;

	GLOBAL.SetupDisplay.FillRect( LabelLeft, 0, LabelWidth, Font->GetCellHeight() + 1, LCD_65K_RGB::C_White);
	GLOBAL.SetupDisplay.DrawHorizontalLine( LabelLeft + 1, Font->GetCellHeight() + 1, LabelWidth - 2, LCD_65K_RGB::C_White);

	uint16_t TitleWidth = strlen_P( title) * Font->GetCellWidth();
	uint16_t TextLeft = ( GLOBAL.SetupDisplay.GetWidth() - TitleWidth) / 2;

	TitleWidth = GLOBAL.SetupDisplay.Print_P
	(
	    TextLeft, 1, SCREEN_SETUP_BASE_TITLE_FONT, LCD_65K_RGB::C_Black, LCD_65K_RGB::C_White,
        LCD::PO_Proportional, title
    );

//	GLOBAL.SetupDisplay.DrawVerticalLine( TextLeft - 1, 1, Font->GetCellHeight(), LCD_65K_RGB::C_White);
/*
	GLOBAL.SetupDisplay.DrawHorizontalLine( TextLeft + 1, 1 + Font->GetCellHeight() + 1, TitleWidth - 2,
									    LCD_65K_RGB::C_Red);
	GLOBAL.SetupDisplay.DrawVerticalLine( TextLeft + TitleWidth + 1, 1, Font->GetCellHeight() - 1,
								   LCD_65K_RGB::C_Red);
*/
	frameLeft = 0;
//	frameTop = Font->GetCellHeight() + 2;
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
