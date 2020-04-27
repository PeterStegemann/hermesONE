// Copyright 2008 Peter Stegemann

#ifndef SETUP_CHANNEL_H
#define SETUP_CHANNEL_H

#include "Source/Tupel.h"

#include "AVR/Source/Types.h"

struct Setup_Channel
{
	enum PointVolume
	{
		PV_Low,
		PV_Center,
		PV_High,

		PV_PointVolumeCount
	};

	enum Mode
	{
		M_Warp,
		M_Clip,

		M_ModeCount
	};

	Setup_Source_Tupel InputSource;
	Setup_Source_Tupel TrimSource;
	Setup_Source_Tupel LimitSource;

	bool Reverse;

	int16_t PointVolume[ PV_PointVolumeCount];

	uint8_t Mode;
};

#endif
