// Copyright 2007 Peter Stegemann

#pragma once

#include "Ports.h"
#include "Rotary.h"

#include "AVR/Source/Types.h"

#include <stdio.h>

class Input_Service
{
  private:
	volatile uint8_t digitalInput[ ( INPUT_DIGITAL_PORTS - 1) / 8 + 1];

	Input_Rotary rotary;
	volatile int8_t rotarySelect;

	bool lastRotaryButton;
	volatile uint8_t rotaryButton;
	volatile uint16_t rotaryButtonTime;

	uint16_t lastActivityUptime;

  public:
	// Initialize.
	void Initialize( void);

	// This is for the interrupt, not for you.
	void Process( void);

	// Get one of the digital I/O pins.
	bool GetDigitalInput( uint8_t InputId);

	// Returns the set of keys 0 for not pressed, 1 for pressed. If an additional pointer to a byte
	// is given, the keys that have changed since the last call are marked.
//	uint8_t GetKeys( uint8_t* Changed = NULL);

	// Get current value of the rotary.
	// Select steps since last call
	// Button press & release cycles since last call
	// CurrentButton current button state
	// ButtonTime last button press duration in milliseconds
	void GetRotary( int8_t* Select = NULL, uint8_t* Button = NULL, bool* CurrentButton = NULL,
					uint16_t* ButtonTime = NULL);

	// Get last uptime the user showed some activity.
	uint16_t GetLastActivityUptime( void);
};
