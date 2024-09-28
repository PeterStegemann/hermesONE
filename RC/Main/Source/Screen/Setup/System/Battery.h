// Copyright 2011 Peter Stegemann

#pragma once

#include "../Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Select.h"
#include "GUI/Setup/VoltageLabel.h"
#include "Setup/Battery.h"

class Screen_Setup_System_Battery : public Screen_Setup_Base
{
  private:
    Setup_Battery* batterySetup;

    GUI_Setup_Select select;

    uint8_t voltage;

    GUI_Setup_VoltageLabel warnLowVoltageLabel;
    GUI_Setup_VoltageLabel warnCriticalVoltageLabel;

    GUI_Setup_VoltageLabel minimumVoltageLabel;
    GUI_Setup_VoltageLabel maximumVoltageLabel;

    GUI_Setup_VoltageLabel calibrationVoltageLabel;

    GUI_Setup_Gauge batteryGauge;

    virtual void display( void)
    {
    	// Adjust menu entries to frame and set them up.
    	const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_MAIN_FONT);

    	uint16_t SubMenuLeft = menuLeft + Font->GetCellWidth();
    	uint16_t ContentLeft = SubMenuLeft + 16 * Font->GetCellWidth();
    	uint16_t GaugeWidth = frameWidth - ( menuLeft - frameLeft) - 1;

    	uint8_t Line = 0;

    	GLOBAL.SetupDisplay.Print_P
    	(
    	    menuLeft, frameTop,
    	    SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
            LCD::PO_Proportional, Text::Exit
        );

    	Line += 2;

    	GLOBAL.SetupDisplay.Print_P
    	(
            menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
            SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
            LCD::PO_Proportional, Text::Warn
        );

    	GLOBAL.SetupDisplay.Print_P
    	(
    	    SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
            SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
            LCD::PO_Proportional, Text::Low
        );
    	warnLowVoltageLabel.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
    	warnLowVoltageLabel.SetVoltage( batterySetup->WarnLowVoltage);

    	GLOBAL.SetupDisplay.Print_P
    	(
    	    SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
            SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
            LCD::PO_Proportional, Text::Critical
        );
    	warnCriticalVoltageLabel.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
    	warnCriticalVoltageLabel.SetVoltage( batterySetup->WarnCriticalVoltage);

    	Line++;

    	GLOBAL.SetupDisplay.Print_P
    	(
    	    menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
            SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
            LCD::PO_Proportional, Text::Range
        );

    	GLOBAL.SetupDisplay.Print_P
    	(
    	    SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
            SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
            LCD::PO_Proportional, Text::Minimum
        );
    	minimumVoltageLabel.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
    	minimumVoltageLabel.SetVoltage( batterySetup->MinimumVoltage);

    	GLOBAL.SetupDisplay.Print_P
    	(
    	    SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
            SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
            LCD::PO_Proportional, Text::Maximum
        );
    	maximumVoltageLabel.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
    	maximumVoltageLabel.SetVoltage( batterySetup->MaximumVoltage);

    	Line++;

    	GLOBAL.SetupDisplay.Print_P
    	(
    	    menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
            SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black,
            LCD::PO_Proportional, Text::Calibration
        );
    	calibrationVoltageLabel.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
    	calibrationVoltageLabel.SetOptions( GUI_Setup_Label::O_Proportional);
    	calibrationVoltageLabel.SetVoltage( voltage);

    	Line += 4;

    	batteryGauge.SetDimensions
    	(
    	    menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
    	    GaugeWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS
        );
    }

    virtual void update( void)
    {
        Screen_Setup_Base::update();

        voltage = GLOBAL.StatusBattery.GetVoltage();

    	calibrationVoltageLabel.SetVoltage( voltage);
        batteryGauge.Display( batterySetup->MinimumVoltage, batterySetup->MaximumVoltage, voltage);
/*
        // RawVoltage == Value
        uint8_t CalibrationValue = batterySetup->CalibrationValue;
        uint16_t RawVoltage = GLOBAL.StatusBattery.GetRawVoltage();

        GLOBAL.SetupDisplay.PrintFormat
        (
            10, 10, SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_Red, LCD_65K_RGB::C_Black, LCD::PO_Fixed,
            "%5d %5d %5u", RawVoltage, CalibrationValue, voltage
        );
*/
    }

    virtual bool processMenu( DoMenuResult Result)
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
                        IsUpdated = select.DoSelect8
                        (
                            ( int8_t*) &( batterySetup->WarnLowVoltage),
                            batterySetup->MinimumVoltage, batterySetup->MaximumVoltage, SETUP_BATTERY_VOLTAGE_STEPS,
                            &menuMarker, &warnLowVoltageLabel, this, &staticUpdate, &updateVoltage
                        );
                    }
                    break;

                    case 4 :
                    {
                        IsUpdated = select.DoSelect8
                        (
                            ( int8_t*) &( batterySetup->WarnCriticalVoltage),
                            batterySetup->MinimumVoltage, batterySetup->MaximumVoltage, SETUP_BATTERY_VOLTAGE_STEPS,
                            &menuMarker, &warnCriticalVoltageLabel, this, &staticUpdate, &updateVoltage
                        );
                    }
                    break;

                    case 7 :
                    {
                        IsUpdated = select.DoSelect8
                        (
                            ( int8_t*) &( batterySetup->MinimumVoltage),
                            SETUP_BATTERY_VOLTAGE_MINIMUM, SETUP_BATTERY_VOLTAGE_MAXIMUM, SETUP_BATTERY_VOLTAGE_STEPS,
                            &menuMarker, &minimumVoltageLabel, this, &staticUpdate, &updateVoltage
                        );
                    }
                    break;

                    case 8 :
                    {
                        IsUpdated = select.DoSelect8
                        (
                            ( int8_t*) &( batterySetup->MaximumVoltage),
                            SETUP_BATTERY_VOLTAGE_MINIMUM, SETUP_BATTERY_VOLTAGE_MAXIMUM, SETUP_BATTERY_VOLTAGE_STEPS,
                            &menuMarker, &maximumVoltageLabel, this, &staticUpdate, &updateVoltage
                        );
                    }
                    break;

                    case 10 :
                    {
                        uint8_t Value = voltage;

                        IsUpdated = select.DoSelect8
                        (
                            ( int8_t*) &Value,
                            0, SETUP_BATTERY_VOLTAGE_MAXIMUM, SETUP_BATTERY_VOLTAGE_STEPS,
                            &menuMarker, &calibrationVoltageLabel, this, &staticUpdate, &updateCalibrationValue
                        );
                    }
                    break;

                    default : break;
                }

                if( IsUpdated == true)
                {
                    GLOBAL.SetupService.SetBattery( batterySetup);
                }
            }
            break;

            default : break;
        }

        return( true);
    }

    static void updateVoltage( void* Object, GUI_Setup_Label* Label, int8_t Value)
    {
    	(( GUI_Setup_VoltageLabel*) Label)->SetVoltage(( uint8_t) Value);
    }

    static void updateCalibrationValue( void* Object, GUI_Setup_Label* Label, int8_t Value)
    {
        (( GUI_Setup_VoltageLabel*) Label)->SetVoltage(( uint8_t) Value);

        GLOBAL.StatusBattery.UpdateCalibrationValue( Value);
    }

  public:
    Screen_Setup_System_Battery
    (
        Input_Service* InputService,
        Interrupt_Service* InterruptService,
        Screen_Status_Status* StatusScreen
    )
        : Screen_Setup_Base( InputService, StatusScreen, 0b10110011001, Text::Battery)
        , select( InputService, InterruptService)
    {
        batterySetup = GLOBAL.StatusBattery.GetBatterySetup();
        voltage = GLOBAL.StatusBattery.GetVoltage();
    }
};
