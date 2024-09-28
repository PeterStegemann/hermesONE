// Copyright 2009 Peter Stegemann

#include "Info.h"

#include "Main/Global.h"
#include "Setup/Struct.h"
#include "Text/Text.h"

#include "AVR/Source/EEPROM.h"

Screen_Setup_Info::Screen_Setup_Info
(
    Input_Service* InputService,
    Interrupt_Service* InterruptService,
    Screen_Status_Status* StatusScreen
)
    : Screen_Setup_Base( InputService, StatusScreen, 0b101, Text::Info)
    , ownerInput( InputService, InterruptService)
{
	GLOBAL.SetupService.GetOwner( owner, sizeof( owner));

	ownerInput.SetOptions(( GUI_Setup_TextInput::Options)( GUI_Setup_TextInput::O_LimitAlphaNumeric));
}

void Screen_Setup_Info::display( void)
{
	// Adjust menu entries to frame and set them up.
	const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_MAIN_FONT);
	
	uint16_t ContentLeft = frameLeft + 12 * Font->GetCellWidth();

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	Line += 2;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Owner);

	ownerLabel.SetDimensions( ContentLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
	ownerInput.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));

	ownerLabel.Display( owner);

	Line += 5;

	GLOBAL.SetupDisplay.PrintFormat_P( menuLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									   SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
									   LCD_65K_RGB::C_Black, LCD::PO_Proportional,
									   Text::ModelsCount,
									   GLOBAL.SetupService.CountModels( Setup_Service::CMO_Used),
									   SETUP_MODELS, sizeof( Setup_Model));
	GLOBAL.SetupDisplay.PrintFormat_P( menuLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									   SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
									   LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::TypesCount,
									   GLOBAL.SetupService.CountTypes( Setup_Service::CTO_Used),
									   SETUP_MODEL_TYPES, sizeof( Setup_Type));
	GLOBAL.SetupDisplay.PrintFormat_P( menuLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									   SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
									   LCD_65K_RGB::C_Black, LCD::PO_Proportional,
									   Text::SourcesCount,
									   GLOBAL.SetupService.CountSources( Setup_Service::CSO_Used),
									   SETUP_SOURCES, sizeof( Setup_Source_Source));

	Line++;

	GLOBAL.SetupDisplay.PrintFormat_P( menuLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									   SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
									   LCD_65K_RGB::C_Black, LCD::PO_Proportional,
									   Text::InternalEEPROMSize, sizeof( Setup_Struct), 4096);
	GLOBAL.SetupDisplay.PrintFormat_P( menuLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									   SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
									   LCD_65K_RGB::C_Black, LCD::PO_Proportional,
									   Text::ExternalEEPROMSize,
									   SETUP_EXTENSIONSTRUCT_SIZE, EEPROM_SIZE);

	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::CPUType);
}
/*
void Screen_Setup_Info::update( void)
{
	Screen_Setup_Base::update();

	uint8_t Line = 2;

	GLOBAL.SetupDisplay.PrintFormat( 10, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_Green,
									 LCD_65K_RGB::C_Black, LCD::PO_Proportional, "sync %ld ",
									 signalService->GetPPM(0)->GetSync());

	uint8_t ChannelId = 0;

	while( ChannelId < SIGNAL_PPM_CHANNELS)
	{
		GLOBAL.SetupDisplay.PrintFormat( 10, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
										 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_Green,
										 LCD_65K_RGB::C_Black, LCD::PO_Proportional, "%d %d ",
										 ChannelId, signalService->GetPPM(0)->GetChannel( ChannelId));

		ChannelId++;
	}
}
*/
bool Screen_Setup_Info::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case 0 : return( false);

				case 2 : doOwner();		break;

				default : break;
			}
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Info::doOwner( void)
{
	ownerLabel.Clear();

	ownerInput.ReadText( owner, sizeof( owner) - 1);

	strncpy( owner, ownerInput.GetText(), sizeof( owner));
	owner[ sizeof( owner) - 1] = 0;

	GLOBAL.SetupService.SetOwner( owner);

	ownerInput.Clear();
	ownerLabel.Display();
}
