// Copyright 2012 Peter Stegemann

#pragma once

#include "Base.h"
#include "GUI/Setup/CheckBox.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/PointGraph.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Defines.h"
#include "Setup/Source/Trimmer.h"

class Screen_Setup_Source_Trimmer : public Screen_Setup_Source_Base
{
  private:
    GUI_Setup_Select select;

    Signal_Source_Trimmer* sourceTrimmer;

    // Point array including modifications by trim and limit for graph display.
    int16_t point[ Setup_Source_Trimmer::PV_PointVolumeCount];

    GUI_Setup_Label sourceNameLabel;
    char sourceName[ SETUP_SOURCE_NAME_SIZE + 1];
    GUI_Setup_Label sourceVolumeLabel;

    GUI_Setup_Label trimNameLabel;
    char trimName[ SETUP_SOURCE_NAME_SIZE + 1];
    GUI_Setup_Label trimVolumeLabel;

    GUI_Setup_Label limitNameLabel;
    char limitName[ SETUP_SOURCE_NAME_SIZE + 1];
    GUI_Setup_Label limitVolumeLabel;

    GUI_Setup_CheckBox reverseCheckBox;
    bool reverse;

    GUI_Setup_Label pointVolumeLabel[ Setup_Source_Trimmer::PV_PointVolumeCount];

    GUI_Setup_PointGraph trimmerGraph;

    GUI_Setup_Gauge currentGauge;

    virtual void display( void);
    virtual void update( void);
    virtual bool processMenu( DoMenuResult Result);

    bool doSource( uint8_t* SignalSourceId, Setup_Source_Tuple* SourceTuple,
                   char SourceName[ SETUP_SOURCE_NAME_SIZE + 1],
                   GUI_Setup_Label* SourceNameLabel, GUI_Setup_Label* SourceVolumeLabel);

    void updatePointVolume( uint8_t CurrentPointId, int16_t Value);

    static void updateVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);
    static void updatePointVolume( void* Object, GUI_Setup_Label* Label, int16_t Value);

    // This is used for the GUI_Setup_Select callbacks.
    uint8_t currentPointId;
    GUI_Setup_Label* currentVolumeLabel;

  public:
    Screen_Setup_Source_Trimmer
    (
        Input_Service* InputService,
        Interrupt_Service* InterruptService,
        Screen_Status_Status* StatusScreen,
        uint8_t SignalSourceId
    );
};
