// Copyright 2010 Peter Stegemann

#pragma once

class Main_Setup;

#include "Base.h"

#include "AVR/Source/LCD/Serial/SerialOled.h"

class Main_Setup : public Main_Base
{
  private:
	bool hasSetupDisplay;

	uint16_t setupBlankTime;
	uint16_t statusBlankTime;

    uint16_t lastSequence;

  public:
	LCD_SerialOled< OLED_SERIAL_ID> SetupDisplay;

  private:
	virtual void run( void);

	void runOnStatusDisplay( void);
	void doStatusStatus( void);		
	void doStatusMain( void);

	void runOnSetupDisplay( void);
	void doSetupStatus( void);		
	void doSetupMain( void);

	void blank( void);

  public:
	Main_Setup( void);
	virtual ~Main_Setup( void);

	virtual void Update( void);

	// Set new setup blank time.
	void SetSetupBlankTime( uint16_t SetupBlankTime);
	// Set new status blank time.
	void SetStatusBlankTime( uint16_t StatusBlankTime);

	virtual void ClearScreens( void);
};
