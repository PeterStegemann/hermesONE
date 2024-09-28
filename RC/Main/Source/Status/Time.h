// Copyright 2011 Peter Stegemann

#pragma once

#include "AVR/Source/Types.h"

class Status_Time
{
  private:
    volatile uint16_t uptime;

  public:
    Status_Time( void)
        : uptime( 0)
    {
    }

    void Initialize( void)
    {
    }

    // This is for the interrupt, not for you.
    void Process( void)
    {
        uptime++;
    }

    // Get uptime in seconds.
    uint16_t GetUptime( void)
    {
        return( uptime);
    }
};
