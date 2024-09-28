// Copyright 2008 Peter Stegemann

#pragma once

#include "BaseList.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/Select.h"
#include "Setup/Defines.h"

class Screen_Setup_Channels : public Screen_Setup_BaseList
{
  private:
    Interrupt_Service* interruptService;

    GUI_Setup_Select select;

    uint8_t selectedModelId;
    uint8_t rfMode;

    GUI_Setup_Label rfModeLabel;
    char rfModeName[ SETUP_PPM_NAME_SIZE];

    GUI_Setup_Label channelLabel[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];
    char channelName[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES][ SETUP_CHANNEL_NAME_SIZE + 1];
    GUI_Setup_Gauge valueGauge[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];

    virtual void display( void);
    virtual void update( void);
    virtual bool processMenu( DoMenuResult Result);

    void reDisplay( void);
    void displayMarker( void);

    void doChanged( void);
    void doSelect( uint8_t LineId);

    void updateRFMode( GUI_Setup_Label* Label, int8_t Value);

    static void updateRFMode( void* Object, GUI_Setup_Label* Label, int8_t Value);

  public:
    Screen_Setup_Channels
    (
        Input_Service* InputService,
        Interrupt_Service* InterruptService,
        Screen_Status_Status* StatusScreen
    );
};
