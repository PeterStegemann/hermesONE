// Copyright 2010 Peter Stegemann

#ifndef MAIN_BASE_H
#define MAIN_BASE_H

#include "Input/Service.h"
#include "Interrupt/Service.h"
#include "Screen/Status/Status.h"
#include "Setup/Service.h"
#include "Signal/Processor.h"
#include "Signal/Service.h"
#include "Status/Battery.h"
#include "Status/Service.h"
#include "Status/Time.h"

#include "AVR/Components/SPI.h"

#include "AVR/Components/LCD/LCD_DOG_S102.h"

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
		Main_Base( void);
		virtual ~Main_Base( void) = 0;

		// Go.
		void Run( void);

		// Called by the screens in regular intervalls.
		virtual void Update( void);

		// Clear screens.
		virtual void ClearScreens( void);
};

#endif
