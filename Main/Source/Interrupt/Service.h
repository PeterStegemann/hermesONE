// Copyright 2009 Peter Stegemann

#pragma once

#include "AVR/Components/Types.h"

#define INTERRUPT_SERVICE_MAXIMUM_MILLIS	1000

class Interrupt_Service
{
  private:
	volatile uint16_t timeMillis;

  public:
	// Start processing loop.
	void Start( void);

	// This is for the interrupt, not for you.
	void Process( void);

	// Get millis.
	uint16_t GetMillis( void);
};
