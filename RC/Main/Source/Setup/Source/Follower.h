// Copyright 2010 Peter Stegemann

#pragma once

#include "Tuple.h"

#include "AVR/Source/Types.h"

struct Setup_Source_Follower
{
	Setup_Source_Tuple TargetSource;
	Setup_Source_Tuple StepSource;

	uint16_t TriggerSource;
	int16_t TriggerHighLimit, TriggerLowLimit;
};
