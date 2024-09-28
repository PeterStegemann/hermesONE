// Copyright 2007 Peter Stegemann

#pragma once

#include "Screen.h"

class Screen_Setup_Test : public Screen_Setup_Base
{
  public:
    Screen_Setup_Test( Input_Service* InputService, Screen_Status_Status* StatusScreen);

    // Run this screen.
    void Run( void);
};
