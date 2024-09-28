// Copyright 2008 Peter Stegemann

#pragma once

#include "Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TimeLabel.h"
#include "GUI/Setup/VoltageLabel.h"
#include "Setup/Defines.h"

class Screen_Setup_Status : public Screen_Setup_Base
{
  private:
    Status_Battery* statusBattery;
    Status_Time* statusTime;

    uint8_t statusSource[ SETUP_MODEL_STATUS_SOURCES];
    GUI_Setup_Gauge statusGauge[ SETUP_MODEL_STATUS_SOURCES];

    GUI_Setup_VoltageLabel batteryLabel;
    GUI_Setup_Gauge batteryGauge;

    uint8_t timerSource[ SETUP_MODEL_STATUS_TIMERS];
    GUI_Setup_TimeLabel timerLabel[ SETUP_MODEL_STATUS_TIMERS];

    virtual void display( void);
    virtual void update( void);
    virtual bool processMenu( DoMenuResult Result);

  public:
    Screen_Setup_Status
    (
        Input_Service* InputService,
        Status_Battery* StatusBattery,
        Status_Time* StatusTime,
        Screen_Status_Status* StatusScreen
    );
};
