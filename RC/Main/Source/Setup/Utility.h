// Copyright 2014 Peter Stegemann

#pragma once

#include <stdint.h>

class Setup_Utility
{
  public:
	static char* GetRFModeName( uint8_t RFMode, char* RFModeName, uint8_t Size);
};
