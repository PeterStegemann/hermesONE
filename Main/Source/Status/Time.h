// Copyright 2011 Peter Stegemann

#ifndef STATUS_TIME_H
#define STATUS_TIME_H

#include "AVR/Source/Types.h"

class Status_Time
{
	private:
		volatile uint16_t uptime;

	public:
		Status_Time( void);

		// Initialize status engine.
		void Initialize( void);

		// This is for the interrupt, not for you.
		void Process( void);

		// Get uptime in seconds.
		uint16_t GetUptime( void);
};

#endif
