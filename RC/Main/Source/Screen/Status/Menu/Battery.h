// Copyright 2011 Peter Stegemann

#pragma once

#include "Base.h"

#include "Setup/Battery.h"
#include "Status/Battery.h"

class Screen_Status_Menu_Battery : public Screen_Status_Menu_Base
{
  private:
    Setup_Battery* batterySetup;
    Status_Battery* statusBattery;

    uint8_t voltage;

    void updateVoltage( int8_t Value);
    static void updateVoltage( void* Object, int8_t Value);
    void updateCalibrationVoltage( int8_t Value);
    static void updateCalibrationVoltage( void* Object, int8_t Value);

    // Return false if the screen should exit.
    virtual bool processMenu( DoMenuResult Result);

  protected:
    // Initial display.
    virtual void display( void);

  public:
    Screen_Status_Menu_Battery( Input_Service* InputService, Status_Battery* StatusBattery);
    virtual ~Screen_Status_Menu_Battery( void);
};
