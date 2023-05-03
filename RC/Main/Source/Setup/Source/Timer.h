// Copyright 2011 Peter Stegemann

#pragma once

#include "AVR/Source/Types.h"

struct Setup_Source_Timer
{
	int16_t InitTime;
	int16_t CurrentTime;

	bool Store;
	bool Reverse;

	uint16_t TriggerSource;
	int16_t TriggerHighLimit, TriggerLowLimit;

	int16_t WarnLowTime;
	int16_t WarnCriticalTime;
	int16_t WarnPauseTime;
};
