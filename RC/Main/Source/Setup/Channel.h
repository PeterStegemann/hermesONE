// Copyright 2008 Peter Stegemann

#ifndef SETUP_CHANNEL_H
#define SETUP_CHANNEL_H

#include "Source/Tuple.h"

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

	Setup_Source_Tuple InputSource;
	Setup_Source_Tuple TrimSource;
	Setup_Source_Tuple LimitSource;

	bool Reverse;

	int16_t PointVolume[ PV_PointVolumeCount];

	uint8_t Mode;
};

#endif
