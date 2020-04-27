// Copyright 2008 Peter Stegemann

#ifndef SETUP_SOURCE_MAP_H
#define SETUP_SOURCE_MAP_H

#include "Tupel.h"

#include "AVR/Components/Types.h"

#define SETUP_SOURCE_MAP_POINTS		7

struct Setup_Source_Map
{
	Setup_Source_Tupel InputSource;

	Setup_Source_Tupel PointSource[ SETUP_SOURCE_MAP_POINTS];
};

#endif
