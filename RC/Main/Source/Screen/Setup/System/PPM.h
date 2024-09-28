// Copyright 2011 Peter Stegemann

#pragma once

#include "../BaseList.h"
#include "GUI/Setup/Checkbox.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/MillisecondLabel.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Defines.h"
#include "Setup/PPM.h"

class Screen_Setup_System_PPM : public Screen_Setup_BaseList
{
  private:
    GUI_Setup_Select select;

    uint8_t ppmId;
    Setup_PPM ppmSetup;

    GUI_Setup_Label ppmNameLabel;
    GUI_Setup_Label ppmNameValueLabel;
    char ppmName[ SETUP_PPM_NAME_SIZE + 1];
    GUI_Setup_TextInput ppmNameInput;

    GUI_Setup_CheckBox invertedCheckBox;
    GUI_Setup_MillisecondLabel centerLabel;

    GUI_Setup_Label targetChannelLabel[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];
    char targetChannelName[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES][ SETUP_CHANNEL_NAME_SIZE + 1];
    GUI_Setup_Label sourceChannelLabel[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];

    virtual void display( void);
    virtual bool processMenu( DoMenuResult Result);

    void reDisplay( void);
    void displayMarker( void);

    void updateSourceChannel( GUI_Setup_Label* Label, int8_t Value);
    static void updateSourceChannel( void* Object, GUI_Setup_Label* Label, int8_t Value);

    void doChanged( void);
    void doSelect( uint8_t LineId);

    void updateCenter( void);
    static void updateCenter( void* Object, GUI_Setup_Label* Label, int8_t Value);

    void doPPMName( void);
    void doInverted( void);
    void doCenter( void);

  public:
    Screen_Setup_System_PPM
    (
        Input_Service* InputService,
        Interrupt_Service* InterruptService,
        Screen_Status_Status* StatusScreen,
        uint8_t PPMId
    );
};
