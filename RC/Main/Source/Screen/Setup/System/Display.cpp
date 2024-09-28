// Copyright 2011 Peter Stegemann

#include "Display.h"

#include "GUI/Setup/GaugeSelect.h"
#include "GUI/Setup/Select.h"
#include "Text/Text.h"
#include "AVR/Source/Utility.h"

Screen_Setup_System_Display::Screen_Setup_System_Display
(
    Input_Service* InputService,
    Interrupt_Service* InterruptService,
    Screen_Status_Status* StatusScreen
)
   : Screen_Setup_Base( InputService, StatusScreen, 0b1010110010100001, Text::Display)
   , select( InputService, InterruptService)
   , gaugeSelect( InputService)
   , setupBacklight( GLOBAL.SetupService.GetSetupBacklight())
   , setupBlankTime( GLOBAL.SetupService.GetSetupBlankTime())
   , statusBacklight( GLOBAL.SetupService.GetStatusBacklight())
   , statusContrast( GLOBAL.SetupService.GetStatusContrast())
   , statusBlankTime( GLOBAL.SetupService.GetStatusBlankTime())
   , statusInverted( GLOBAL.SetupService.GetStatusInverted())
{
}

void Screen_Setup_System_Display::display( void)
{
	// Adjust menu entries to frame and set them up.
	const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t SubMenuLeft = menuLeft + Font->GetCellWidth();
	uint16_t ContentLeft = SubMenuLeft + 16 * Font->GetCellWidth();
	uint16_t ContentWidth = frameWidth - ( ContentLeft - frameLeft) - 1;

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	Line += 2;

	Line++;
	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Setup);

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Backlight);

	setupBacklightGauge.SetDimensions( ContentLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									   ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
	setupBacklightGauge.SetOptions( GUI_Setup_Gauge::O_Percentage);
	setupBacklightGauge.Display( LCD_65K_RGB_BACKLIGHT_OFF, LCD_65K_RGB_BACKLIGHT_FULL,
								 setupBacklight);

	Line++;

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::BlankTime);

	setupBlankTimeLabel.SetDimensions( ContentLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	setupBlankTimeLabel.SetOptions( GUI_Setup_Label::O_Fixed);
	setupBlankTimeLabel.SetText( setupBlankTimeLabelText);
	displaySetupBlankTime();

	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Status);

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Backlight);

	statusBacklightGauge.SetDimensions( ContentLeft,
									    frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									    ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
	statusBacklightGauge.SetOptions( GUI_Setup_Gauge::O_Percentage);
	statusBacklightGauge.Display( LCD_DOG_BACKLIGHT_OFF, LCD_DOG_BACKLIGHT_FULL, statusBacklight);

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Contrast);

	statusContrastGauge.SetDimensions( ContentLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									   ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
	statusContrastGauge.SetOptions( GUI_Setup_Gauge::O_Percentage);
	statusContrastGauge.Display( LCD_DOG_CONTRAST_OFF, LCD_DOG_CONTRAST_FULL, statusContrast);

	Line++;

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::BlankTime);

	statusBlankTimeLabel.SetDimensions( ContentLeft,
									    frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	statusBlankTimeLabel.SetOptions( GUI_Setup_Label::O_Fixed);
	statusBlankTimeLabel.SetText( statusBlankTimeLabelText);
	displayStatusBlankTime();

	Line++;

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::StatusInverted);

	statusInvertedCheckBox.SetDimensions( ContentLeft,
										  frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
										  ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
	statusInvertedCheckBox.Display( statusInverted);
}

bool Screen_Setup_System_Display::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case 0 : return( false);

				case 5 : doSetupBacklight();	break;

				case 7 : doSetupBlankTime();	break;

				case 10 : doStatusBacklight();	break;
				case 11 : doStatusContrast();	break;

				case 13 : doStatusBlankTime();	break;

				case 15 : doStatusInverted();	break;

				default : break;
			}
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_System_Display::doSetupBacklight( void)
{
    bool ValueChanged = gaugeSelect.DoSelect8
    (
        &setupBacklight, LCD_DOG_BACKLIGHT_OFF, LCD_DOG_BACKLIGHT_FULL, LCD_65K_RGB_BACKLIGHT_STEPS,
        &menuMarker, &setupBacklightGauge, &updateSetupBacklight
    );

    if( ValueChanged == true)
	{
		GLOBAL.SetupService.SetSetupBacklight( setupBacklight);
	}
}

void Screen_Setup_System_Display::updateSetupBacklight( uint8_t Value)
{
	GLOBAL.SetupDisplay.SetBacklight( Value);
}

void Screen_Setup_System_Display::doSetupBlankTime( void)
{
	if( select.DoSelectTime( &setupBlankTime, 0, 5 * 60, 5, &menuMarker,
									    &setupBlankTimeLabel, this, &updateSetupBlankTime))
	{
		GLOBAL.SetupService.SetSetupBlankTime( setupBlankTime);
	}
}

void Screen_Setup_System_Display::displaySetupBlankTime( void)
{
	if( setupBlankTime != 0)
	{
		snprintf_P( setupBlankTimeLabelText, sizeof( setupBlankTimeLabelText), Text::TimeFormat,
					setupBlankTime / 60, setupBlankTime % 60);
		setupBlankTimeLabel.SetText( setupBlankTimeLabelText);
	}
	else
	{
		setupBlankTimeLabel.SetText_P( Text::PaddedOff);
	}

	setupBlankTimeLabel.Display();
}

void Screen_Setup_System_Display::updateSetupBlankTime( void* Object, GUI_Setup_Label* Label,
													    int16_t Value)
{
	GLOBAL.SetSetupBlankTime( Value);

	(( Screen_Setup_System_Display*) Object)->displaySetupBlankTime();
}

void Screen_Setup_System_Display::doStatusBacklight( void)
{
    bool ValueChanged = gaugeSelect.DoSelect8
    (
        &statusBacklight, LCD_DOG_BACKLIGHT_OFF, LCD_DOG_BACKLIGHT_FULL, LCD_DOG_BACKLIGHT_STEPS,
        &menuMarker, &statusBacklightGauge, &updateStatusBacklight
    );

	if( ValueChanged == true)
	{
		GLOBAL.SetupService.SetStatusBacklight( statusBacklight);
	}
}

void Screen_Setup_System_Display::updateStatusBacklight( uint8_t Value)
{
	GLOBAL.StatusDisplay.SetBacklight( Value);
}

void Screen_Setup_System_Display::doStatusContrast( void)
{
    bool ValueChanged = gaugeSelect.DoSelect8
    (
        &statusContrast, LCD_DOG_CONTRAST_OFF, LCD_DOG_CONTRAST_FULL, LCD_DOG_CONTRAST_STEPS,
        &menuMarker, &statusContrastGauge, &updateStatusContrast
    );

    if( ValueChanged == true)
	{
		GLOBAL.SetupService.SetStatusContrast( statusContrast);
	}
}

void Screen_Setup_System_Display::updateStatusContrast( uint8_t Value)
{
	GLOBAL.StatusDisplay.SetContrast( Value);
}

void Screen_Setup_System_Display::doStatusBlankTime( void)
{
	if( select.DoSelectTime( &statusBlankTime, 0, 5 * 60, 5, &menuMarker,
                             &statusBlankTimeLabel, this, &updateStatusBlankTime))
	{
		GLOBAL.SetupService.SetStatusBlankTime( statusBlankTime);
	}
}

void Screen_Setup_System_Display::displayStatusBlankTime( void)
{
	if( statusBlankTime != 0)
	{
		snprintf_P( statusBlankTimeLabelText, sizeof( statusBlankTimeLabelText), Text::TimeFormat,
				    statusBlankTime / 60, statusBlankTime % 60);
		statusBlankTimeLabel.SetText( statusBlankTimeLabelText);
	}
	else
	{
		statusBlankTimeLabel.SetText_P( Text::PaddedOff);
	}

	statusBlankTimeLabel.Display();
}

void Screen_Setup_System_Display::updateStatusBlankTime( void* Object, GUI_Setup_Label* Label,
														 int16_t Value)
{
	GLOBAL.SetStatusBlankTime( Value);

	(( Screen_Setup_System_Display*) Object)->displayStatusBlankTime();
}

void Screen_Setup_System_Display::doStatusInverted( void)
{
	statusInverted = avr::Utility::Invert( statusInverted);
	GLOBAL.SetupService.SetStatusInverted( statusInverted);
	statusInvertedCheckBox.Display( statusInverted);
	GLOBAL.StatusDisplay.SetInverted( statusInverted);
}
