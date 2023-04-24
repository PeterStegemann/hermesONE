// Copyright 2010 Peter Stegemann

#pragma once

#include "Input/Service.h"
#include "Interrupt/Service.h"
#include "Screen/Status/Status.h"
#include "Setup/Service.h"
#include "Signal/Processor.h"
#include "Signal/Service.h"
#include "Status/Battery.h"
#include "Status/Service.h"
#include "Status/Time.h"

#include "AVR/Source/SPI.h"
#include "AVR/Source/Utility.h"
#include "AVR/Source/LCD/LCD_DOG_S102.h"

#include <avr/interrupt.h>

#define GLOBAL	MainInstance

class Main_Base
{
  private:
    uint16_t lastStoreModifiedTime;

  public:
    Setup_Service SetupService;

    Status_Battery StatusBattery;
    Status_Service StatusService;
    Status_Time StatusTime;
    Input_Service InputService;
    Interrupt_Service InterruptService;
    Signal_Service SignalService;
    Signal_Processor SignalProcessor;

    SPI Spi;

    LCD_DOG_S102 StatusDisplay;

    Screen_Status_Status StatusScreen;

  private:
    virtual void run( void) = 0;

  public:
    Main_Base( void)
        : lastStoreModifiedTime( 0)
    {
    }

    virtual ~Main_Base( void)
    {
    }

    // Go.
    void Run( void)
    {
        // Wait some time for the controller and other components to stabilize.
    	UTILITY::Pause( 100);

        // Initialize status.
        StatusService.Initialize();

        // Initialize input.
        InputService.Initialize();

        // Init SPI.
        Spi.Initialize();

        // Set up head lcd.
        StatusDisplay.Initialize( SetupService.GetStatusBacklight(), SetupService.GetStatusContrast(),
                                  SetupService.GetStatusInverted());

        // Show intro on status screen.
        StatusScreen.ShowIntro();

        // Enable interrupts.
        sei();

        // Run ppm engine.
        SignalService.Start();

        //	StatusService.Beep();

        // Initialize processing.
        SignalProcessor.Initialize();

        // Run timer.
        StatusTime.Initialize();

        // Run interrupt service last.
        InterruptService.Start();

        // Wait a moment for all services to come up.
        UTILITY::Pause( 5);

        run();
    }

    // Called by the screens in regular intervals.
    virtual void Update( void)
    {
    	uint16_t Uptime = StatusTime.GetUptime();

    	// Store modified sources on a regular basis.
	    if(( Uptime - lastStoreModifiedTime) >= SYSTEM_STORE_MODIFIED_DELAY)
	    {
    		lastStoreModifiedTime = Uptime;

            SignalProcessor.StoreModifiedSources();
	    }
    }

    // Clear screens.
    virtual void ClearScreens( void)
    {
	    StatusDisplay.Clear();
    }
};
