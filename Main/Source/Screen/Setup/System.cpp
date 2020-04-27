// Copyright 2007 Peter Stegemann

#include "System.h"

#include "GUI/Setup/Popup.h"
#include "GUI/Setup/Select.h"
#include "Main/Setup.h"
#include "Serial/DesktopConnection.h"
#include "System/Battery.h"
#include "System/Calibration.h"
#include "System/Display.h"
#include "System/PPM.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_System::Screen_Setup_System( void)
				   : Screen_Setup_Base( 0b1010001100110101, Text::System)
{
}

void Screen_Setup_System::display( void)
{
	// Adjust menu entries to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	Line += 2;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Serial);

	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Display);
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Battery);

	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::PPM);

	uint16_t SubMenuLeft = menuLeft + Font->CellWidth;

	char ModuleName[ SETUP_PPM_NAME_SIZE];

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::RFModule0);

	GLOBAL.SetupService.GetPPMName( 0, ModuleName, sizeof( ModuleName));

	GLOBAL.SetupDisplay.Print( SubMenuLeft + ( 15 * Font->CellWidth),
							   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
							   SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
							   LCD_65K_RGB::C_Black, LCD::PO_Proportional, ModuleName);

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::RFModule1);

	GLOBAL.SetupService.GetPPMName( 1, ModuleName, sizeof( ModuleName));

	GLOBAL.SetupDisplay.Print( SubMenuLeft + ( 15 * Font->CellWidth),
							   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
							   SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
							   LCD_65K_RGB::C_Black, LCD::PO_Proportional, ModuleName);

	Line += 3;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Calibration);

	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Reset);
}

bool Screen_Setup_System::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case 0 : return( false);

				case 2 : doSerial();			break;

				case 4 : doDisplay();			break;
				case 5 : doBattery();			break;

				case 8 : doPPM(0);				break;
				case 9 : doPPM(1);				break;

				case 13 : doCalibration();		break;

				case 15 : doReset();			break;

				default : break;
			}
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_System::doSerial( void)
{
	GUI_Setup_Popup Popup;

	// Set text.
	Popup.SetText_P( Text::SerialAsk);
	Popup.SetOKText_P( Text::Stop);

	Popup.Show();

	Serial_DesktopConnection< SYSTEM_SERIAL_ID> UseConnection;
	// Set up serial.
	UseConnection.Initialize();

	UseConnection.DoSerialConnection();

	ReDisplay();
}

void Screen_Setup_System::doDisplay( void)
{
	Screen_Setup_System_Display DisplayScreen;
	DisplayScreen.Run();

	ReDisplay();
}

void Screen_Setup_System::doBattery( void)
{
	Screen_Setup_System_Battery BatteryScreen;
	BatteryScreen.Run();

	ReDisplay();
}

void Screen_Setup_System::doPPM( uint8_t Id)
{
	Screen_Setup_System_PPM PPMScreen( Id);
	PPMScreen.Run();

	ReDisplay();
}

void Screen_Setup_System::doCalibration( void)
{
	Screen_Setup_System_Calibration CalibrationScreen;
	CalibrationScreen.Run();

	ReDisplay();
}

void Screen_Setup_System::doReset( void)
{
	GUI_Setup_Popup Popup;

	// Set text.
	Popup.SetText_P( Text::ResetCheck);
	Popup.SetOKText_P( Text::Reset);
	Popup.SetCancelText_P( Text::Cancel);

	Popup.Show();

	if( Popup.Ask() != true)
	{
		ReDisplay();

		return;
	}

	GLOBAL.SetupService.Reset();

	GLOBAL.SignalProcessor.LoadModel();

	UTILITY::HardReset();
	// No comin' back.
}
