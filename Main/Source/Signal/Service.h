// Copyright 2014 Peter Stegemann

#ifndef SIGNAL_SERVICE_H
#define SIGNAL_SERVICE_H

#include "PPM.h"

#include "AVR/Components/Types.h"

#define SIGNAL_SERVICE_PPMS							2

class Signal_Service
{
	private:
		Signal_PPM ppm0;
		Signal_PPM ppm1;

	public:
		Signal_Service( void);

		// This is for the interrupt, not for you.
		void ProcessSignal( uint8_t Index);

		// Start sending loop.
		void Start( void);

		// Get ppm 0 or 1.
		Signal_PPM* GetPPM( uint8_t Index);
};

#endif
