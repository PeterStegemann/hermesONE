// Copyright 2008 Peter Stegemann

#pragma once

#include "Setup/Defines.h"

#include "AVR/Components/Types.h"

class GUI_Setup_Gauge;
class GUI_Setup_Label;
class GUI_Setup_Marker;

class GUI_Setup_Select
{
  private:
	static void blink( GUI_Setup_Label* Label, uint16_t* Millis, bool* Inverted);

  public:
	static bool DoSelect( int8_t* Value, int8_t LowerLimit, int8_t UpperLimit, int8_t StepWidth,
						    GUI_Setup_Marker* Marker, GUI_Setup_Label* Label, void* Object,
						    void ( *Update)( void* Object),
						    void ( *UpdateLabel)( void* Object, GUI_Setup_Label* Label,
						    int8_t Value));

	static bool DoSelect16( int16_t* Value, int16_t LowerLimit, int16_t UpperLimit,
							  int16_t StepWidth, GUI_Setup_Marker* Marker, GUI_Setup_Label* Label,
							  void* Object, void ( *Update)( void* Object),
							  void ( *UpdateLabel)( void* Object, GUI_Setup_Label* Label,
							  int16_t Value));

	static bool DoTimeSelect( int16_t* Value, int16_t LowerLimit, int16_t UpperLimit,
							  uint8_t StepWidth, GUI_Setup_Marker* Marker, GUI_Setup_Label* Label,
							  void* Object,
							  void ( *UpdateLabel)( void* Object, GUI_Setup_Label* Label,
													int16_t Value));

	// UseGauge may be null. T_Empty means "All"
	static bool DoSourceSelect( uint8_t* SignalSourceId, uint16_t* SetupSourceId,
								GUI_Setup_Marker* Marker, GUI_Setup_Label* Label,
								GUI_Setup_Gauge* Gauge,
								char SourceName[ SETUP_SOURCE_NAME_SIZE + 1],
								void* Object, void ( *Update)( void* Object), bool HasFixed,
								Signal_Source_Source::Level SourceLevel,
								Signal_Source_Source::Type SourceType =
								Signal_Source_Source::T_Empty);
/*
		static bool DoValueSourceSelect( int16_t* Value, int16_t LowerLimit, int16_t UpperLimit,
										 int16_t StepWidth, GUI_Setup_Marker* UseMarker, GUI_Setup_Label* UseLabel,
										 char SourceName[ SETUP_SOURCE_NAME_SIZE + 1],
										 void* Object, void ( *Display)( void* Object),
										 void ( *Update)( void* Object, int16_t Value));
*/
};
