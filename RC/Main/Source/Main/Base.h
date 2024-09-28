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
#include "AVR/Source/LCD/DOG/DOG_S102.h"

#include <avr/interrupt.h>
#include <avr/wdt.h>

class Main_Base
{
  private:
    uint16_t lastStoreModifiedTime;
    bool debug;

  protected:
    Input_Service inputService;
    Interrupt_Service interruptService;
    Screen_Status_Status statusScreen;

  public:
    Setup_Service SetupService;
    Signal_Service SignalService;
    Signal_Processor SignalProcessor;
    Status_Battery StatusBattery;
    Status_Service StatusService;
    Status_Time StatusTime;

    avr::SPI Spi;

    LCD_DOG_S102 StatusDisplay;

  private:
    virtual void run( void) = 0;

  public:
    Main_Base( void)
        : lastStoreModifiedTime( 0)
        , debug( false)
        , interruptService( &inputService, &SignalProcessor, &StatusService)
        , statusScreen( &inputService, &StatusDisplay)
        , SignalProcessor( &inputService)
        , StatusBattery( &SignalProcessor, &StatusService)
        , StatusService( &SignalService)
    {
    }

    virtual ~Main_Base( void)
    {
    }

    // Go.
    void Run( void)
    {
    	// Disable watchdog.
    	wdt_disable();

        // Wait some time for the controller and other components to stabilize.
    	avr::Utility::Pause( 100);

        // Setup goes first.
        SetupService.Initialize();

    	// Get debug state.
//    	debug = SetupService.GetDebug();

        StatusService.Initialize();

        inputService.Initialize();

        StatusBattery.Initialize( &SetupService);

        // Spi needs to go before displays that might need spi.
        Spi.Initialize();

        // Set up head lcd.
        StatusDisplay.Initialize
        (
            SetupService.GetStatusBacklight(), SetupService.GetStatusContrast(), SetupService.GetStatusInverted()
        );

        // Show intro on status screen.
        statusScreen.ShowIntro();

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
        interruptService.Start();

        // Wait a moment for all services to come up.
        avr::Utility::Pause( 5);

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

    virtual void ClearScreens( void)
    {
	    StatusDisplay.Clear();
    }

    bool GetDebug( void)
    {
        return( debug);
    }

    void SetDebug( bool Debug)
    {
        debug = Debug;
    }
};
