// Copyright 2010 Peter Stegemann

#include "Calibrations.h"

#include "Calibration.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"
#include "AVR/Source/LCD/LCD_DOG.h"

#define MENU_EXIT		0
#define MENU_START		1
#define MENU_COUNT		SIGNAL_PROCESSOR_ANALOG_INPUTS + MENU_START

Screen_Status_Menu_Calibrations::Screen_Status_Menu_Calibrations( void)
							   : Screen_Status_Menu_Base( MENU_COUNT, L_Two)
{
}

Screen_Status_Menu_Calibrations::~Screen_Status_Menu_Calibrations( void)
{
}

void Screen_Status_Menu_Calibrations::display( void)
{
	pickMenu( MENU_START);
}

void Screen_Status_Menu_Calibrations::update( void)
{
	if( currentMenuEntry >= MENU_START)
	{
		int32_t Value = GLOBAL.SignalProcessor.GetCalibratedAnalogValue( currentMenuEntry - MENU_START);
		Value *= 200;
		Value /= SIGNAL_VALUE_RANGE;

		char String[ 6];

		snprintf_P( String, sizeof( String), Text::FourDigitPercentFormat, Value);

		SetText(( Level)( menuLevel + 2), String);
	}
}

bool Screen_Status_Menu_Calibrations::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Changed :
		{
			switch( currentMenuEntry)
			{
				case MENU_EXIT :
				{
					SetText_P( Text::Exit);
					SetText_P(( Level)( menuLevel + 2), NULL);
				}
				break;

				default :
				{
					uint8_t Id = currentMenuEntry - MENU_START;

					char String[ 3];

					snprintf_P( String, sizeof( String), Text::TwinCharacterFormat,
								'A' + ( Id / 2), '0' + ( Id % 2));

					SetText( String);
				}
				break;
			}
		}
		break;

		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case MENU_EXIT : return( false);

				default :
				{
					SetText_P(( Level)( menuLevel + 2), NULL);

					Screen_Status_Menu_Calibration CalibrationScreen(
						currentMenuEntry - MENU_START);
					CalibrationScreen.Run();

					clearSubMenuLine();
				}
				break;
			}
		}
		break;

		default : break;
	}

	return( true);
}
