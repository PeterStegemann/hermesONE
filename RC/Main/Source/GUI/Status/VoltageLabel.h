// Copyright 2012 Peter Stegemann

#pragma once

#include "Label.h"

class GUI_Status_VoltageLabel : public GUI_Status_Label
{
	private:
		char voltageLabelText[ 6];

	public:
		void SetVoltage( uint8_t Voltage);
};
