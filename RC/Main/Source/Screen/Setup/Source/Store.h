// Copyright 2009 Peter Stegemann

#pragma once

#include "Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Source/Source.h"

class Screen_Setup_Source_Store : public Screen_Setup_Source_Base
{
  private:
    GUI_Setup_Select select;

    Signal_Source_Store* sourceStore;

    GUI_Setup_Label sourceNameLabel;
    char sourceName[ SETUP_SOURCE_NAME_SIZE + 1];

    virtual void display( void);
    virtual bool processMenu( DoMenuResult Result);

  public:
    Screen_Setup_Source_Store
    (
        Input_Service* InputService,
        Interrupt_Service* InterruptService,
        Screen_Status_Status* StatusScreen,
        uint8_t SignalSourceId
    );
};
