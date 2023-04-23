// Copyright 2014 Peter Stegemann

#ifndef SETUP_PPM_H
#define SETUP_PPM_H

#include "AVR/Source/Types.h"

struct Setup_PPM
{
	int8_t Center;
	bool Inverted;

	uint8_t ChannelMapping[ SIGNAL_PPM_CHANNELS];
};

#endif
