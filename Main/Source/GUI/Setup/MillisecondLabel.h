// Copyright 2011 Peter Stegemann

#pragma once

#include "Label.h"

class GUI_Setup_MillisecondLabel : public GUI_Setup_Label
{
	private:
		char millisecondLabelText[ 7];
	
	public:
		void SetMillisecond( uint8_t Millisecond);
};
