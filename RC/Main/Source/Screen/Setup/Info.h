// Copyright 2009 Peter Stegemann

#pragma once

#include "Base.h"
#include "GUI/Setup/CheckBox.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Defines.h"

class Screen_Setup_Info : public Screen_Setup_Base
{
  private:
    GUI_Setup_Label ownerLabel;
    GUI_Setup_TextInput ownerInput;
    char owner[ SETUP_OWNER_SIZE + 1];

    virtual void display( void);
//		virtual void update( void);
    virtual bool processMenu( DoMenuResult Result);

    void doOwner( void);

  public:
    Screen_Setup_Info
    (
        Input_Service* InputService,
        Interrupt_Service* InterruptService,
        Screen_Status_Status* StatusScreen
    );
};
