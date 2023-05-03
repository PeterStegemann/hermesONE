// Copyright 2012 Peter Stegemann

#pragma once

#include "Label.h"

class GUI_Status_TimeLabel : public GUI_Status_Label
{
	private:
		char timeLabelText[ 7];
	
	public:
		void SetTime( int16_t Time);
};
