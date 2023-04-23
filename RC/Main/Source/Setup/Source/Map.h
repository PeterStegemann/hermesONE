// Copyright 2008 Peter Stegemann

#ifndef SETUP_SOURCE_MAP_H
#define SETUP_SOURCE_MAP_H

#include "Tuple.h"

#include "AVR/Source/Types.h"

#define SETUP_SOURCE_MAP_POINTS		7

struct Setup_Source_Map
{
	Setup_Source_Tuple InputSource;

	Setup_Source_Tuple PointSource[ SETUP_SOURCE_MAP_POINTS];
};

#endif
