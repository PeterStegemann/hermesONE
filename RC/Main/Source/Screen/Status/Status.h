// Copyright 2009 Peter Stegemann

#pragma once

#include "../Base.h"

#include "GUI/Status/BatteryGauge.h"
#include "GUI/Status/Gauge.h"
#include "GUI/Status/Label.h"
#include "GUI/Status/TimeLabel.h"
#include "GUI/Status/VoltageLabel.h"
#include "Main/Base.h"
#include "Setup/Defines.h"

#include "AVR/Source/Types.h"

class Screen_Status_Status : public Screen_Base
{
  private:
    char modelName[ SETUP_MODEL_NAME_SIZE + 1];
    GUI_Status_Label modelNameLabel;

    char typeName[ SETUP_MODEL_NAME_SIZE + 1];
    GUI_Status_Label typeNameLabel;

    GUI_Status_BatteryGauge batteryGauge;
    GUI_Status_VoltageLabel batteryLabel;
    GUI_Status_TimeLabel timeLabel;

    uint8_t source[ SETUP_MODEL_STATUS_SOURCES];
    GUI_Status_Gauge statusGauge[ SETUP_MODEL_STATUS_SOURCES];

    // Return false if the screen should exit.
    virtual bool processMenu( DoMenuResult Result);

  protected:
    // Initial display.
    virtual void display( void);

    // Regular display update.
    virtual void update( void);

  public:
    Screen_Status_Status( void);

    // Show RfOff.
    void ShowRfOff( void);

    // Intro is an alternative to run, won't work with Update()!
    void ShowIntro( void);

    // Set a single source.
//		void SetSource( uint8_t Source, uint8_t SignalSourceId);

    // Display screen (full update).
    void Display( void);

    // Update screen.
    void Update( void);
};
