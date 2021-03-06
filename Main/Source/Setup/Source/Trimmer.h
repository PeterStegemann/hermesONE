// Copyright 2012 Peter Stegemann

#ifndef SETUP_SOURCE_TRIMMER_H
#define SETUP_SOURCE_TRIMMER_H

#include "Tupel.h"

#include "AVR/Source/Types.h"

struct Setup_Source_Trimmer
{
	enum PointVolume
	{
		PV_Low,
		PV_Center,
		PV_High,

		PV_PointVolumeCount
	};

	Setup_Source_Tupel InputSource;
	Setup_Source_Tupel TrimSource;
	Setup_Source_Tupel LimitSource;

	bool Reverse;

	int16_t PointVolume[ PV_PointVolumeCount];
};

#endif
