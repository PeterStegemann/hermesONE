// Copyright 2008 Peter Stegemann

#pragma once

#include "Setup/Calibration.h"

class Signal_Calibration
{
  public:
	Setup_Calibration Setup;

  public:
	Signal_Calibration( void);

	int16_t Calibrate( int16_t Value);
};
