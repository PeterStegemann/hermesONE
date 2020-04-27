// Copyright 2011 Peter Stegemann

#include "Time.h"

#include "AVR/Components/Utility.h"

Status_Time::Status_Time( void)
		   : uptime( 0)
{
}

void Status_Time::Initialize( void)
{
}

void Status_Time::Process( void)
{
	uptime++;
}

uint16_t Status_Time::GetUptime( void)
{
	return( uptime);
}
