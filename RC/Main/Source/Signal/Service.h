// Copyright 2014 Peter Stegemann

#pragma once

#include "PPM.h"

#include "AVR/Source/Types.h"
#include "AVR/Source/Utility.h"

#include <avr/io.h>

#define SIGNAL_SERVICE_PPMS							2

#define SIGNAL_SERVICE_MATCH_INTERUPT_1				TIMER3_COMPA_vect
#define SIGNAL_SERVICE_MATCH_INTERUPT_2				TIMER3_COMPB_vect

#define SIGNAL_SERVICE_MATCH_INTERUPT_1_ENABLE		OCIE3A
#define SIGNAL_SERVICE_MATCH_INTERUPT_2_ENABLE		OCIE3B

class Signal_Service
{
  private:
    Signal_PPM ppm0;
    Signal_PPM ppm1;

  public:
    Signal_Service( void);

    // This is for the interrupt, not for you.
    void ProcessSignal( uint8_t Index)
    {
        if( Index == 0)
        {
            ppm0.ProcessSignal();
        }
        else
        {
            ppm1.ProcessSignal();
        }
    }

    // Start sending loop.
    void Start( void)
    {
        // Clear counter before use. This will also clear all other settings.
        TCNT3 = 0;

        ppm0.Start();
        ppm1.Start();

        // No clear of timer by match and split clock for timer by 8.
        TCCR3B = UTILITY_BitValue( CS30);
        // Switch interrupt for compare match on.
        TIMSK3 = UTILITY_BitValue( SIGNAL_SERVICE_MATCH_INTERUPT_1_ENABLE) |
                 UTILITY_BitValue( SIGNAL_SERVICE_MATCH_INTERUPT_2_ENABLE);
    }

    // Get ppm 0 or 1.
    Signal_PPM* GetPPM( uint8_t Index)
    {
        if( Index == 0)
        {
            return &ppm0;
        }
        else
        {
            return &ppm1;
        }
    }
};
