// Copyright 2012 Peter Stegemann

#ifndef GUI_STATUS_VOLTAGELABEL_H
#define GUI_STATUS_VOLTAGELABEL_H

#include "Label.h"

class GUI_Status_VoltageLabel : public GUI_Status_Label
{
	private:
		char voltageLabelText[ 6];

	public:
		void SetVoltage( uint8_t Voltage);
};

#endif
