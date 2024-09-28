// Copyright 2009 Peter Stegemann

#pragma once

#include "Input/Service.h"
#include "Signal/Processor.h"
#include "Status/Battery.h"
#include "Status/Service.h"

#include "AVR/Source/Types.h"

#define INTERRUPT_SERVICE_MAXIMUM_MILLIS	1000

class Interrupt_Service
{
  private:
	Input_Service* inputService;
	Signal_Processor* signalProcessor;
	Status_Battery* statusBattery;
	Status_Service* statusService;
    Status_Time* statusTime;

	volatile uint16_t timeMillis;

  public:
    Interrupt_Service
    (
    	Input_Service* InputService,
        Signal_Processor* SignalProcessor,
	    Status_Battery* StatusBattery,
    	Status_Service* StatusService,
        Status_Time* StatusTime
    )
        : inputService( InputService)
        , signalProcessor( SignalProcessor)
        , statusBattery( StatusBattery)
        , statusService( StatusService)
        , statusTime( StatusTime)
    {
    }

	// Start processing loop.
	void Start( void);

	// This is for the interrupt, not for you.
	void Process( void);

	// Get millis.
	uint16_t GetMillis( void);
};
