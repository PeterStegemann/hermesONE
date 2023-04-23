// Copyright 2011 Peter Stegemann

#include "Battery.h"

#include "GUI/Setup/Select.h"
#include "GUI/Setup/Select.h"
#include "Text/Text.h"

Screen_Setup_System_Battery::Screen_Setup_System_Battery( void)
						   : Screen_Setup_Base( 0b10110011001, Text::Battery)
{
	GLOBAL.SetupService.GetBattery( &batterySetup);

	calibrationVoltage = GLOBAL.StatusBattery.GetVoltage();
}

void Screen_Setup_System_Battery::display( void)
{
	// Adjust menu entries to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t SubMenuLeft = menuLeft + Font->CellWidth;
	uint16_t ContentLeft = SubMenuLeft + 16 * Font->CellWidth;
	uint16_t GaugeWidth = frameWidth - ( menuLeft - frameLeft) - 1;

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	Line += 2;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Warn);

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Low);
	warnLowVoltageLabel.SetDimensions( ContentLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	warnLowVoltageLabel.SetVoltage( batterySetup.WarnLowVoltage);

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Critical);
	warnCriticalVoltageLabel.SetDimensions( ContentLeft,
										    frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	warnCriticalVoltageLabel.SetVoltage( batterySetup.WarnCriticalVoltage);

	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Range);

	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Minimum);
	minimumVoltageLabel.SetDimensions( ContentLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	minimumVoltageLabel.SetVoltage( batterySetup.MinimumVoltage);
	
	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Maximum);
	maximumVoltageLabel.SetDimensions( ContentLeft,
									   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	maximumVoltageLabel.SetVoltage( batterySetup.MaximumVoltage);
	
	Line++;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Calibration);
	calibrationVoltageLabel.SetDimensions( ContentLeft,
									       frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	calibrationVoltageLabel.SetOptions( GUI_Setup_Label::O_Proportional);
	calibrationVoltageLabel.SetVoltage( calibrationVoltage);

	Line += 2;

	batteryLabel.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
	batteryLabel.SetFont( FONT::FI_Large);
	batteryLabel.SetOptions( GUI_Setup_Label::O_Proportional);
	batteryLabel.SetVoltage( calibrationVoltage);

	Line++;

	batteryGauge.SetDimensions( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								GaugeWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
}

bool Screen_Setup_System_Battery::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			bool IsUpdated = false;

			switch( currentMenuEntry)
			{
				case 0 : return( false);

				case 3 :
				{
					IsUpdated = GUI_Setup_Select::DoSelect(
						( int8_t*) &( batterySetup.WarnLowVoltage), batterySetup.MinimumVoltage,
						batterySetup.MaximumVoltage, SETUP_BATTERY_VOLTAGE_STEPS, &menuMarker,
						&warnLowVoltageLabel, this, &staticUpdate, &updateVoltage);
				}
				break;

				case 4 :
				{
					IsUpdated = GUI_Setup_Select::DoSelect(
						( int8_t*) &( batterySetup.WarnCriticalVoltage), batterySetup.MinimumVoltage,
						batterySetup.MaximumVoltage, SETUP_BATTERY_VOLTAGE_STEPS, &menuMarker,
						&warnCriticalVoltageLabel, this, &staticUpdate, &updateVoltage);
				}
				break;

				case 7 :
				{
					IsUpdated = GUI_Setup_Select::DoSelect(
						( int8_t*) &( batterySetup.MinimumVoltage), SETUP_BATTERY_VOLTAGE_MINIMUM,
						SETUP_BATTERY_VOLTAGE_MAXIMUM, SETUP_BATTERY_VOLTAGE_STEPS, &menuMarker,
						&minimumVoltageLabel, this, &staticUpdate, &updateVoltage);
				}
				break;

				case 8 :
				{
					IsUpdated = GUI_Setup_Select::DoSelect(
						( int8_t*) &( batterySetup.MaximumVoltage), SETUP_BATTERY_VOLTAGE_MINIMUM,
						SETUP_BATTERY_VOLTAGE_MAXIMUM, SETUP_BATTERY_VOLTAGE_STEPS, &menuMarker,
						&maximumVoltageLabel, this, &staticUpdate, &updateVoltage);
				}
				break;

				case 10 :
				{
					IsUpdated = GUI_Setup_Select::DoSelect(
						( int8_t*) &calibrationVoltage, batterySetup.MinimumVoltage,
						batterySetup.MaximumVoltage, SETUP_BATTERY_VOLTAGE_STEPS, &menuMarker,
						&calibrationVoltageLabel, this, &staticUpdate, &updateCalibrationVoltage);
				}
				break;

				default : break;
			}

			if( IsUpdated == true)
			{
				GLOBAL.SetupService.SetBattery( &batterySetup);
			}
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_System_Battery::update( void)
{
	Screen_Setup_Base::update();

	batteryLabel.SetVoltage( GLOBAL.StatusBattery.GetVoltage());

	batteryGauge.Display( GLOBAL.StatusBattery.GetMinimumVoltage(),
						  GLOBAL.StatusBattery.GetMaximumVoltage(),
						  GLOBAL.StatusBattery.GetVoltage());

/*
	Setup_Battery* BatterySetup = &((( Screen_Setup_System_Battery*) Object)->batterySetup);

	// RawVoltage == Value
	uint16_t RawVoltage = GLOBAL.StatusBattery.GetRawVoltage();
	uint16_t CalibrationVoltage = BatterySetup->CalibrationVoltage;
	uint16_t Voltage = GLOBAL.StatusBattery.GetVoltage();

	GLOBAL.SetupDisplay.PrintFormat( 10, 10, SCREEN_SETUP_BASE_MAIN_FONT,
									 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
									 "%5d %5d %5u", RawVoltage, CalibrationVoltage, Voltage);
*/
}

void Screen_Setup_System_Battery::updateVoltage( void* Object, GUI_Setup_Label* Label, int8_t Value)
{
	(( GUI_Setup_VoltageLabel*) Label)->SetVoltage(( uint8_t) Value);

	Setup_Battery* BatterySetup = &((( Screen_Setup_System_Battery*) Object)->batterySetup);

	GLOBAL.StatusBattery.SetBatterySetup( BatterySetup);
}

void Screen_Setup_System_Battery::updateCalibrationVoltage( void* Object, GUI_Setup_Label* Label,
														    int8_t Value)
{
	(( GUI_Setup_VoltageLabel*) Label)->SetVoltage(( uint8_t) Value);

	Setup_Battery* BatterySetup = &((( Screen_Setup_System_Battery*) Object)->batterySetup);

	// RawVoltage == Value
	uint16_t RawVoltage = GLOBAL.StatusBattery.GetRawVoltage();
	uint32_t NewCalibrationVoltage = Value;
	NewCalibrationVoltage *= 1023;
	NewCalibrationVoltage /= RawVoltage;

	BatterySetup->CalibrationVoltage = NewCalibrationVoltage;

	GLOBAL.StatusBattery.SetBatterySetup( BatterySetup);
}
