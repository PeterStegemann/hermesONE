// Copyright 2011 Peter Stegemann

#include "Battery.h"

#include "Main/Global.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"
#include "AVR/Source/LCD/LCD_DOG.h"

#define MENU_EXIT						0
#define MENU_WARN_LOW_VOLTAGE			1
#define MENU_WARN_CRITICAL_VOLTAGE		2
#define MENU_MINIMUM_VOLTAGE			3
#define MENU_MAXIMUM_VOLTAGE			4
#define MENU_CALIBRATION_VOLTAGE		5
#define MENU_COUNT						6

Screen_Status_Menu_Battery::Screen_Status_Menu_Battery( void)
						  : Screen_Status_Menu_Base( MENU_COUNT, L_Three)
{
	GLOBAL.SetupService.GetBattery( &batterySetup);

	calibrationVoltage = GLOBAL.StatusBattery.GetVoltage();
}

Screen_Status_Menu_Battery::~Screen_Status_Menu_Battery( void)
{
}

void Screen_Status_Menu_Battery::display( void)
{
	pickMenu( MENU_WARN_LOW_VOLTAGE);
}

void Screen_Status_Menu_Battery::updateVoltage( int8_t Value)
{
	char String[ 6];

	snprintf_P( String, sizeof( String), Text::VoltageFormat, Value / 10, Value % 10);

	SetText(( Level)( menuLevel + 1), String);

	GLOBAL.StatusBattery.SetBatterySetup( &batterySetup);
}

void Screen_Status_Menu_Battery::updateVoltage( void* Object, int8_t Value)
{
	(( Screen_Status_Menu_Battery*) Object)->updateVoltage( Value);
}

void Screen_Status_Menu_Battery::updateCalibrationVoltage( int8_t Value)
{
	// RawVoltage == Value
	uint16_t RawVoltage = GLOBAL.StatusBattery.GetRawVoltage();
	uint32_t NewCalibrationVoltage = Value;
	NewCalibrationVoltage *= 1023;
	NewCalibrationVoltage /= RawVoltage;

	batterySetup.CalibrationVoltage = NewCalibrationVoltage;

	updateVoltage( Value);
}

void Screen_Status_Menu_Battery::updateCalibrationVoltage( void* Object, int8_t Value)
{
	(( Screen_Status_Menu_Battery*) Object)->updateCalibrationVoltage( Value);
}

bool Screen_Status_Menu_Battery::processMenu( DoMenuResult Result)
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

				case MENU_WARN_LOW_VOLTAGE :
				{
					SetText_P( Text::Low);

					updateVoltage( batterySetup.WarnLowVoltage);
				}
				break;

				case MENU_WARN_CRITICAL_VOLTAGE :
				{
					SetText_P( Text::Critical);

					updateVoltage( batterySetup.WarnCriticalVoltage);
				}
				break;

				case MENU_MINIMUM_VOLTAGE :
				{
					SetText_P( Text::Minimum);

					updateVoltage( batterySetup.MinimumVoltage);
				}
				break;

				case MENU_MAXIMUM_VOLTAGE :
				{
					SetText_P( Text::Maximum);

					updateVoltage( batterySetup.MaximumVoltage);
				}
				break;

				case MENU_CALIBRATION_VOLTAGE :
				{
					SetText_P( Text::Calibration);

					updateVoltage( calibrationVoltage);
				}
				break;
			}
		}
		break;

		case DMR_Selected :
		{
			bool IsUpdated = false;

			switch( currentMenuEntry)
			{
				case MENU_EXIT : return( false);

				case MENU_WARN_LOW_VOLTAGE :
				{
					IsUpdated =
						selectValue(( int8_t*) &( batterySetup.WarnLowVoltage),
									 SETUP_BATTERY_VOLTAGE_MINIMUM, SETUP_BATTERY_VOLTAGE_MAXIMUM,
									 SETUP_BATTERY_VOLTAGE_STEPS, this, &updateVoltage);
				}
				break;

				case MENU_WARN_CRITICAL_VOLTAGE :
				{
					IsUpdated =
						selectValue(( int8_t*) &( batterySetup.WarnCriticalVoltage),
									 SETUP_BATTERY_VOLTAGE_MINIMUM, SETUP_BATTERY_VOLTAGE_MAXIMUM,
									 SETUP_BATTERY_VOLTAGE_STEPS, this, &updateVoltage);
				}
				break;

				case MENU_MINIMUM_VOLTAGE :
				{
					IsUpdated =
						selectValue(( int8_t*) &( batterySetup.MinimumVoltage),
									 SETUP_BATTERY_VOLTAGE_MINIMUM, SETUP_BATTERY_VOLTAGE_MAXIMUM,
									 SETUP_BATTERY_VOLTAGE_STEPS, this, &updateVoltage);
				}
				break;

				case MENU_MAXIMUM_VOLTAGE :
				{
					IsUpdated =
						selectValue(( int8_t*) &( batterySetup.MaximumVoltage),
									 SETUP_BATTERY_VOLTAGE_MINIMUM, SETUP_BATTERY_VOLTAGE_MAXIMUM,
									 SETUP_BATTERY_VOLTAGE_STEPS, this, &updateVoltage);
				}
				break;

				case MENU_CALIBRATION_VOLTAGE :
				{
					IsUpdated =
						selectValue(( int8_t*) &calibrationVoltage, batterySetup.MinimumVoltage,
									 batterySetup.MaximumVoltage, SETUP_BATTERY_VOLTAGE_STEPS, this,
									 &updateCalibrationVoltage);
				}
				break;
			}

			if( IsUpdated == true)
			{
				GLOBAL.SetupService.SetBattery( &batterySetup);
			}

			drawMenuMarker();
		}
		break;

		default : break;
	}

	return( true);
}
