// Copyright 2012 Peter Stegemann

#pragma once

#include "Label.h"

class GUI_Setup_TimeLabel : public GUI_Setup_Label
{
	private:
		char timeLabelText[ 7];

	public:
		void SetTime( int16_t Time);
};
