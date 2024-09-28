// Copyright 2008 Peter Stegemann

#pragma once

#include "Base.h"

#define SCREEN_SETUP_BASELIST_MAXIMUM_LINES		14

class Screen_Setup_BaseList : public Screen_Setup_Base
{
  protected:
    GUI_Setup_Marker scrollUpMarker;
    GUI_Setup_Marker scrollDownMarker;

    // This is the first line shown when scrolling.
    uint8_t firstLine;
    // This is the total number of lines shown when scrolling.
    uint8_t visibleLines;

    void setupScrollMarkers( uint8_t StartLine);
    void reDisplayMarkers( uint8_t Entries);
    bool doChanged( uint8_t Entries, uint8_t StartLine);
    void adjustAfterDelete( uint8_t Entries, uint8_t StartLine);

  public:
    Screen_Setup_BaseList
    (
        Input_Service* InputService,
        Screen_Status_Status* StatusScreen,
        const flash_char* Title
    );
};
