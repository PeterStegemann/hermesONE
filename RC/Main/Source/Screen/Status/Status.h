// Copyright 2009 Peter Stegemann

#pragma once

#include "../Base.h"

#include "GUI/Status/BatteryGauge.h"
#include "GUI/Status/Gauge.h"
#include "GUI/Status/Label.h"
#include "GUI/Status/TimeLabel.h"
#include "GUI/Status/VoltageLabel.h"
#include "Setup/Defines.h"
#include "Status/Battery.h"
#include "Status/Time.h"

#include "AVR/Source/LCD/DOG/DOG.h"
#include "AVR/Source/Types.h"

class Screen_Status_Status : public Screen_Base
{
  private:
    Status_Battery* statusBattery;
    Status_Time* statusTime;
    LCD_DOG* statusDisplay;

    char modelName[ SETUP_MODEL_NAME_SIZE + 1];
    GUI_Status_Label modelNameLabel;

    char typeName[ SETUP_MODEL_NAME_SIZE + 1];
    GUI_Status_Label typeNameLabel;

    GUI_Status_BatteryGauge batteryGauge;
    GUI_Status_VoltageLabel batteryLabel;
    GUI_Status_TimeLabel timeLabel;

	GUI_Status_Gauge leftSideStatusGauge;
	GUI_Status_Gauge rightSideStatusGauge;
	GUI_Status_Gauge leftBottomStatusGauge;
	GUI_Status_Gauge rightBottomStatusGauge;

    // Return false if the screen should exit.
    virtual bool processMenu( DoMenuResult Result)
    {
        switch( Result)
        {
            case DMR_Selected :	return( false);

            default : break;
        }

        return( true);
    }

    // Initial display.
    void displayGauge( GUI_Status_Gauge* Gauge, uint8_t SourceId);

  protected:
    // Initial display.
    virtual void display( void);

    // Regular display update.
    virtual void update( void);

  public:
    Screen_Status_Status
    (
        Input_Service* InputService,
        Status_Battery* StatusBattery,
        Status_Time* StatusTime,
        LCD_DOG* StatusDisplay
    )
        : Screen_Base( InputService)
        , statusBattery( StatusBattery)
        , statusTime( StatusTime)
        , statusDisplay( StatusDisplay)
        , modelNameLabel( StatusDisplay)
        , typeNameLabel( StatusDisplay)
        , batteryGauge( StatusDisplay)
        , batteryLabel( StatusDisplay)
        , timeLabel( StatusDisplay)
        , leftSideStatusGauge( StatusDisplay)
        , rightSideStatusGauge( StatusDisplay)
        , leftBottomStatusGauge( StatusDisplay)
        , rightBottomStatusGauge( StatusDisplay)
    {
        leftSideStatusGauge.SetOptions(( GUI_Status_Gauge::Options)( GUI_Status_Gauge::O_Vertical));
        rightSideStatusGauge.SetOptions(( GUI_Status_Gauge::Options)( GUI_Status_Gauge::O_Vertical));
        leftBottomStatusGauge.SetOptions(( GUI_Status_Gauge::Options)( GUI_Status_Gauge::O_Horizontal));
        rightBottomStatusGauge.SetOptions(( GUI_Status_Gauge::Options)( GUI_Status_Gauge::O_Horizontal));
    }

    // Show RfOff.
    void ShowRfOff( void)
    {
        statusDisplay->Clear();

        statusDisplay->Print_P( 5, 1, avr::font::FI_Large, Text::RFDisabled);
    }

    // Intro is an alternative to run, won't work with Update()!
    void ShowIntro( void)
    {
        statusDisplay->Clear();

        statusDisplay->Print( 15, 1, avr::font::FI_Large, "hermesONE");
    }

    // Set a single source.
//		void SetSource( uint8_t Source, uint8_t SignalSourceId);

    // Display screen (full update).
    void Display( void)
    {
        display();
    }

    // Update screen.
    void Update( void)
    {
        update();
    }
};
