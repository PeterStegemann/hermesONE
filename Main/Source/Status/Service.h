// Copyright 2008 Peter Stegemann

#ifndef STATUS_SERVICE_H
#define STATUS_SERVICE_H

#include "AVR/Components/Types.h"

class Status_Service
{
	private:
		// Blink count.
		uint16_t blinkCount;

		uint16_t beepCount;
		volatile uint16_t beepLength;
		volatile uint16_t beepPause;
		volatile uint8_t beepRepeat;

	public:
		Status_Service( void);

		// Initialize status engine.
		void Initialize( void);

		// This is for the interrupt, not for you.
		void Process( void);

		// Buzz.
		void Buzz( uint16_t Length, uint16_t Pause, uint8_t Repeat);

		// Give a beep.
		void Beep( void);
};

#endif
