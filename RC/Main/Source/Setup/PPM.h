// Copyright 2014 Peter Stegemann

#pragma once

#include "AVR/Source/Types.h"

struct Setup_PPM
{
	int8_t Center;
	bool Inverted;

	uint8_t ChannelMapping[ SIGNAL_PPM_CHANNELS];
};
