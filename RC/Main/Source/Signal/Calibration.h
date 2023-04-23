// Copyright 2008 Peter Stegemann

#ifndef SIGNAL_CALIBRATION_H
#define SIGNAL_CALIBRATION_H

#include "Setup/Calibration.h"

class Signal_Calibration
{
  public:
	Setup_Calibration Setup;

  public:
	Signal_Calibration( void);

	int16_t Calibrate( int16_t Value);
};

#endif
