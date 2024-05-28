// Copyright 2010 Peter Stegemann

#include "Calibration.h"

#include "Main/Global.h"
#include "Setup/Calibration.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"
#include "AVR/Source/LCD/DOG/DOG.h"

#define MENU_EXIT		0
#define MENU_LOW		1
#define MENU_CENTER		2
#define MENU_HIGH		3
#define MENU_COUNT		4

Screen_Status_Menu_Calibration::Screen_Status_Menu_Calibration( uint8_t CalibrationId)
							  : Screen_Status_Menu_Base( MENU_COUNT, L_Three)
							  , calibrationId( CalibrationId)
{
	calibration = GLOBAL.SignalProcessor.GetCalibration( calibrationId);
}

Screen_Status_Menu_Calibration::~Screen_Status_Menu_Calibration( void)
{
}

void Screen_Status_Menu_Calibration::updateValue( uint8_t ValueId)
{
	int16_t Value = calibration->Setup.Value[ ValueId];

	char String[ 7];

	snprintf_P( String, sizeof( String), Text::SixDigitInt16Format, Value);

	SetText(( Level)( menuLevel + 1), String);
}

void Screen_Status_Menu_Calibration::display( void)
{
	pickMenu( MENU_LOW);
}

bool Screen_Status_Menu_Calibration::processMenu( DoMenuResult Result)
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
					SetText_P(( Level)( menuLevel + 1), NULL);
				}
				break;

				case MENU_LOW :
				{
					SetText_P( Text::Minus100Percent);

					updateValue( Setup_Calibration::V_Low);
				}
				break;

				case MENU_CENTER :
				{
					SetText_P( Text::ZeroPercent);

					updateValue( Setup_Calibration::V_Center);
				}
				break;

				case MENU_HIGH :
				{
					SetText_P( Text::Plus100Percent);

					updateValue( Setup_Calibration::V_High);
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
					calibration->Setup.Value[ currentMenuEntry - MENU_LOW] =
						GLOBAL.SignalProcessor.GetRawAnalogValue( calibrationId);

					updateValue( currentMenuEntry - MENU_LOW);

					GLOBAL.SetupService.SetCalibration( calibrationId, &( calibration->Setup));
				}
				break;
			}
		}
		break;

		default : break;
	}

	return( true);
}
