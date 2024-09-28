// Copyright 2008 Peter Stegemann

#pragma once

#include "BaseList.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "Setup/Defines.h"

class Screen_Setup_Types : public Screen_Setup_BaseList
{
  private:
    Input_Service* inputService;

    uint8_t selectedTypeId;
    uint8_t type[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];

    GUI_Setup_Label typeLabel[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];
    char typeName[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES][ SETUP_MODEL_NAME_SIZE + 1];

    virtual void display( void);
    virtual bool processMenu( DoMenuResult Result);

    void reDisplay( void);
    void displayMarker( void);

    void doChanged( void);
    void doAdd( void);
    void doSelect( uint8_t LineId);
    void doDelete( uint8_t LineId);

  public:
    Screen_Setup_Types( Input_Service* InputService, Screen_Status_Status* StatusScreen);
};
