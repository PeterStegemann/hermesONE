// Copyright 2012 Peter Stegemann

#ifndef SETUP_SOURCE_TRIMMER_H
#define SETUP_SOURCE_TRIMMER_H

#include "Tuple.h"

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

	Setup_Source_Tuple InputSource;
	Setup_Source_Tuple TrimSource;
	Setup_Source_Tuple LimitSource;

	bool Reverse;

	int16_t PointVolume[ PV_PointVolumeCount];
};

#endif
