// Copyright 2011 Peter Stegemann

#pragma once

#include "../Base.h"
#include "GUI/Setup/CheckBox.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/GaugeSelect.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/Select.h"

class Screen_Setup_System_Display : public Screen_Setup_Base
{
  private:
    GUI_Setup_Select select;
    GUI_Setup_GaugeSelect gaugeSelect;

    GUI_Setup_Gauge setupBacklightGauge;
    uint8_t setupBacklight;

    GUI_Setup_Label setupBlankTimeLabel;
    char setupBlankTimeLabelText[ 6];
    int16_t setupBlankTime;

    GUI_Setup_Gauge statusBacklightGauge;
    uint8_t statusBacklight;
    GUI_Setup_Gauge statusContrastGauge;
    uint8_t statusContrast;

    GUI_Setup_Label statusBlankTimeLabel;
    char statusBlankTimeLabelText[ 6];
    int16_t statusBlankTime;

    GUI_Setup_CheckBox statusInvertedCheckBox;
    bool statusInverted;

    virtual void display( void);
    virtual bool processMenu( DoMenuResult Result);

    void doSetupBacklight( void);
    static void updateSetupBacklight( uint8_t Value);

    void doSetupBlankTime( void);
    void displaySetupBlankTime( void);
    static void updateSetupBlankTime( void* Object, GUI_Setup_Label* Label, int16_t Value);

    void doStatusBacklight( void);
    static void updateStatusBacklight( uint8_t Value);
    void doStatusContrast( void);
    static void updateStatusContrast( uint8_t Value);

    void doStatusBlankTime( void);
    void displayStatusBlankTime( void);
    static void updateStatusBlankTime( void* Object, GUI_Setup_Label* Label, int16_t Value);

    void doStatusInverted( void);

  public:
    Screen_Setup_System_Display
    (
        Input_Service* InputService,
        Interrupt_Service* InterruptService,
        Screen_Status_Status* StatusScreen
    );
};
