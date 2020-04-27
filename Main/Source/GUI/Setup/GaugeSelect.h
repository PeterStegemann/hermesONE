// Copyright 2014 Peter Stegemann

#pragma once

#include "Setup/Defines.h"

#include "AVR/Source/Types.h"

class GUI_Setup_Gauge;
class GUI_Setup_Marker;

class GUI_Setup_GaugeSelect
{
  public:
	static bool DoSelect( uint8_t* Value, uint8_t LowerLimit, uint8_t UpperLimit, uint8_t StepWidth,
						  GUI_Setup_Marker* Marker, GUI_Setup_Gauge* Gauge,
						  void ( *UpdateGauge)( uint8_t Value));
};
