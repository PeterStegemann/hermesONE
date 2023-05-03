// Copyright 2008 Peter Stegemann

#pragma once

#include "Tuple.h"

#include "AVR/Source/Types.h"

#define SETUP_SOURCE_MAP_POINTS		7

struct Setup_Source_Map
{
	Setup_Source_Tuple InputSource;

	Setup_Source_Tuple PointSource[ SETUP_SOURCE_MAP_POINTS];
};
