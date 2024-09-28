// Copyright 2009 Peter Stegemann

#pragma once

#include "Input/Service.h"
#include "Signal/Processor.h"
#include "Status/Service.h"

#include "AVR/Source/Types.h"

#define INTERRUPT_SERVICE_MAXIMUM_MILLIS	1000

class Interrupt_Service
{
  private:
	Input_Service* inputService;
	Signal_Processor* signalProcessor;
	Status_Service* statusService;

	volatile uint16_t timeMillis;

  public:
    Interrupt_Service
    (
    	Input_Service* InputService,
    	Signal_Processor* SignalProcessor,
    	Status_Service* StatusService
    )
        : inputService( InputService)
        , signalProcessor( SignalProcessor)
        , statusService( StatusService)
    {
    }

	// Start processing loop.
	void Start( void);

	// This is for the interrupt, not for you.
	void Process( void);

	// Get millis.
	uint16_t GetMillis( void);
};
