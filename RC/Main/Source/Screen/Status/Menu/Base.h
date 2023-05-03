// Copyright 2010 Peter Stegemann

#pragma once

#include "../../Base.h"

#include <avr/pgmspace.h>

class Screen_Status_Menu_Base : public Screen_Base
{
  public:
	enum Level
	{
		L_One	=	0,
		L_Two	=	1,
		L_Three	=	2,
		L_Four	=	3
	};

  private:
	// Return false if the screen should exit.
	virtual bool processMenu( DoMenuResult Result);

  protected:
	uint8_t menuEntries;
	Level menuLevel;

	// Initial display.
	virtual void display( void);
	// Pick a menu entry for display.
	void pickMenu( uint8_t MenuEntry);
	// Draw this screens menu marker.
	virtual void drawMenuMarker( void);
	void drawMenuMarker( uint8_t MenuLevel);
	void drawMenuMarker( uint8_t MenuLevel, bool ShowLeft, bool ShowRight);
	// Clear sub menu line
	void clearSubMenuLine( void);
	// Regular display update.
	virtual void update( void);
	// Blanking.
	virtual void blank( bool Blanked);

	void updateBoolean( bool Value);

	bool selectValue( int8_t* Value, int8_t LowerLimit, int8_t UpperLimit, int8_t StepWidth,
					  void* Object, void ( *UpdateValue)( void* Object, int8_t Value));
	bool selectValue( int16_t* Value, int16_t LowerLimit, int16_t UpperLimit, int16_t StepWidth,
					  void* Object, void ( *UpdateValue)( void* Object, int16_t Value));
	bool selectValue( uint16_t* Value, uint16_t LowerLimit, uint16_t UpperLimit, uint16_t StepWidth,
					  void* Object, void ( *UpdateValue)( void* Object, uint16_t Value));

	bool selectSource( uint8_t* SignalSourceId, uint16_t* SetupSourceId, void* Object,
					   void ( *UpdateSource)( void* Object, uint16_t SetupSourceId),
					   Signal_Source_Source::Level Level);

	void SetText( const char* Text);
	void SetText_P( const flash_char* Text);
	void SetText( Level UseLevel, const char* Text);
	void SetText_P( Level UseLevel, const flash_char* Text);

	void SetSource( uint16_t SetupSourceId);
	void SetSource( Level UseLevel, uint16_t SetupSourceId);

  public:
	Screen_Status_Menu_Base( uint8_t MenuEntries, Level MenuLevel);
	virtual ~Screen_Status_Menu_Base( void);
};
