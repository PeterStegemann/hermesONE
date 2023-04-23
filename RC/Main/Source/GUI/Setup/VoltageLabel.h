// Copyright 2011 Peter Stegemann

#pragma once

#include "Label.h"

class GUI_Setup_VoltageLabel : public GUI_Setup_Label
{
	private:
		char voltageLabelText[ 6];

	public:
		void SetVoltage( uint8_t Voltage);
};
