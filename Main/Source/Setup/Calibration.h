// Copyright 2008 Peter Stegemann

#ifndef SETUP_CALIBRATION_H
#define SETUP_CALIBRATION_H

#include "AVR/Source/Types.h"

struct Setup_Calibration
{
	enum Value
	{
		V_Low,
		V_Center,
		V_High,
		
		V_ValueCount
	};
	
	int16_t Value[ V_ValueCount];
};

#endif
