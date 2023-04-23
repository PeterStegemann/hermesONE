// Copyright 2008 Peter Stegemann

#ifndef SETUP_SOURCE_SOURCE_H
#define SETUP_SOURCE_SOURCE_H

#include "Follower.h"
#include "Input.h"
#include "Map.h"
#include "Mix.h"
#include "Proxy.h"
#include "Store.h"
#include "Timer.h"
#include "Trimmer.h"

union Setup_Source_Union
{
	Setup_Source_Follower Follower;
	Setup_Source_Input Input;
	Setup_Source_Map Map;
	Setup_Source_Mix Mix;
	Setup_Source_Proxy Proxy;
	Setup_Source_Store Store;
	Setup_Source_Timer Timer;
	Setup_Source_Trimmer Trimmer;
};

struct Setup_Source_Source
{
	char Name[ SETUP_SOURCE_NAME_SIZE];

	uint8_t Type;
	uint8_t ModelId;

	Setup_Source_Union Body;
};

#endif
