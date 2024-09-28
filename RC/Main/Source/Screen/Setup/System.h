// Copyright 2007 Peter Stegemann

#pragma once

#include "Base.h"
#include "GUI/Setup/CheckBox.h"
#include "GUI/Setup/MillisecondLabel.h"
#include "GUI/Setup/Popup.h"
#include "Serial/DesktopConnection.h"
#include "System/Battery.h"
#include "System/Calibration.h"
#include "System/Display.h"
#include "System/PPM.h"
#include "Text/Text.h"

class Screen_Setup_System : public Screen_Setup_Base
{
  private:
    Interrupt_Service* interruptService;
    Status_Battery* statusBattery;

    GUI_Setup_CheckBox debugCheckBox;

    virtual void display( void)
    {
        // Adjust menu entries to frame and set them up.
        const avr::font::Type* Font = avr::font::Font::Get( SCREEN_SETUP_BASE_MAIN_FONT);

    	uint16_t ContentLeft = frameLeft + 18 * Font->GetCellWidth();
    	uint16_t ContentWidth = frameWidth - ( ContentLeft - frameLeft) - 1;

        uint8_t Line = 0;

        GLOBAL.SetupDisplay.Print_P
        (
            menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Exit
        );

        Line += 2;

        GLOBAL.SetupDisplay.Print_P
        (
            menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT), SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Serial
        );

        Line++;

        GLOBAL.SetupDisplay.Print_P
        (
            menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT), SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Display
        );
        GLOBAL.SetupDisplay.Print_P
        (
            menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT), SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Battery
        );

        Line++;

        GLOBAL.SetupDisplay.Print_P
        (
            menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT), SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::PPM
        );

        uint16_t SubMenuLeft = menuLeft + Font->GetCellWidth();

        char ModuleName[ SETUP_PPM_NAME_SIZE];

        GLOBAL.SetupDisplay.Print_P
        (
            SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT), SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::RFModule0
        );

        GLOBAL.SetupService.GetPPMName( 0, ModuleName, sizeof( ModuleName));

        GLOBAL.SetupDisplay.Print
        (
            SubMenuLeft + ( 15 * Font->GetCellWidth()), frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
            SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, ModuleName
        );

        GLOBAL.SetupDisplay.Print_P
        (
            SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT), SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::RFModule1
        );

        GLOBAL.SetupService.GetPPMName( 1, ModuleName, sizeof( ModuleName));

        GLOBAL.SetupDisplay.Print
        (
            SubMenuLeft + ( 15 * Font->GetCellWidth()), frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
            SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, ModuleName
        );

        Line += 2;

        GLOBAL.SetupDisplay.Print_P
        (
            menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT), SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Calibration
        );

        Line++;

        GLOBAL.SetupDisplay.Print_P
        (
            menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT), SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Debug
        );

    	debugCheckBox.SetDimensions
    	(
    	    ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
            ContentWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS
        );

    	debugCheckBox.Clear();
	    debugCheckBox.Display( GLOBAL.GetDebug());

        GLOBAL.SetupDisplay.Print_P
        (
            menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT), SCREEN_SETUP_BASE_MAIN_FONT,
            LCD_65K_RGB::C_White, LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Reset
        );
    }

    virtual bool processMenu( DoMenuResult Result)
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

                    case 12 : doCalibration();		break;

                    case 14 : doDebug();		    break;
                    case 15 : doReset();			break;

                    default : break;
                }
            }
            break;

            default : break;
        }

        return( true);
    }

    void doSerial( void)
    {
        GUI_Setup_Popup Popup( inputService);

        // Set text.
        Popup.SetText_P( Text::SerialAsk);
        Popup.SetOKText_P( Text::Stop);

        Popup.Show();

        Serial_DesktopConnection< SYSTEM_SERIAL_ID> DesktopConnection( inputService);

        DesktopConnection.Initialize();
        DesktopConnection.DoSerialConnection();

        ReDisplay();
    }

    void doDisplay( void)
    {
        Screen_Setup_System_Display DisplayScreen( inputService, interruptService, statusScreen);
        DisplayScreen.Run();

        ReDisplay();
    }

    void doBattery( void)
    {
        Screen_Setup_System_Battery BatteryScreen
        (
            inputService, interruptService, statusBattery, statusScreen
        );
        BatteryScreen.Run();

        ReDisplay();
    }

    void doPPM( uint8_t Id)
    {
        Screen_Setup_System_PPM PPMScreen( inputService, interruptService, statusScreen, Id);
        PPMScreen.Run();

        ReDisplay();
    }

    void doCalibration( void)
    {
        Screen_Setup_System_Calibration CalibrationScreen( inputService, statusScreen);
        CalibrationScreen.Run();

        ReDisplay();
    }

    void doDebug( void)
    {
    	bool Debug = !GLOBAL.GetDebug();

    	GLOBAL.SetupService.SetDebug( Debug);
    	GLOBAL.SetDebug( Debug);

    	debugCheckBox.Display( Debug);

        ReDisplay();
    }

    void doReset( void)
    {
        GUI_Setup_Popup Popup( inputService);

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

//        GLOBAL.SignalProcessor.LoadModel();

        avr::Utility::HardReset();
        // No comin' back.
    }

  public:
    Screen_Setup_System
    (
        Input_Service* InputService,
        Interrupt_Service* InterruptService,
        Status_Battery* StatusBattery,
        Screen_Status_Status* StatusScreen
    )
        : Screen_Setup_Base( InputService, StatusScreen, 0b1101001100110101, Text::System)
        , interruptService( InterruptService)
        , statusBattery( StatusBattery)
    {
    }
};
