// Copyright 2012 Peter Stegemann

#ifndef GUI_STATUS_TIMELABEL_H
#define GUI_STATUS_TIMELABEL_H

#include "Label.h"

class GUI_Status_TimeLabel : public GUI_Status_Label
{
	private:
		char timeLabelText[ 7];
	
	public:
		void SetTime( int16_t Time);
};

#endif
