// Copyright 2009 Peter Stegemann

#pragma once

#include "Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/Select.h"
#include "Setup/Defines.h"

class Screen_Setup_SetupStatus : public Screen_Setup_Base
{
  private:
    GUI_Setup_Select select;

    uint8_t timerId[ SETUP_MODEL_STATUS_TIMERS];

    GUI_Setup_Label timerLabel[ SETUP_MODEL_STATUS_TIMERS];
    char timerName[ SETUP_MODEL_STATUS_TIMERS][ SETUP_SOURCE_NAME_SIZE + 1];

    uint8_t sourceId[ SETUP_MODEL_STATUS_SOURCES];

    GUI_Setup_Label sourceLabel[ SETUP_MODEL_STATUS_SOURCES];
    char sourceName[ SETUP_MODEL_STATUS_SOURCES][ SETUP_SOURCE_NAME_SIZE + 1];
    GUI_Setup_Gauge statusGauge[ SETUP_MODEL_STATUS_SOURCES];

    virtual void display( void);
    virtual void update( void);
    virtual bool processMenu( DoMenuResult Result);

    void reDisplay( void);

    // This will store the sourceId that is currently changed.
    uint8_t currentSource;

  public:
    Screen_Setup_SetupStatus
    (
        Input_Service* InputService,
        Interrupt_Service* InterruptService,
        Screen_Status_Status* StatusScreen
    );
};
