// Copyright 2010 Peter Stegemann

#ifndef SETUP_SOURCE_FOLLOWER_H
#define SETUP_SOURCE_FOLLOWER_H

#include "Tupel.h"

#include "AVR/Components/Types.h"

struct Setup_Source_Follower
{
	Setup_Source_Tupel TargetSource;
	Setup_Source_Tupel StepSource;

	uint16_t TriggerSource;
	int16_t TriggerHighLimit, TriggerLowLimit;
};

#endif
