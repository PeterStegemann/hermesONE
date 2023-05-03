// Copyright 2010 Peter Stegemann

#pragma once

#include "Base.h"

class Screen_Status_Menu_Calibrations : public Screen_Status_Menu_Base
{
	private:
		// Return false if the screen should exit.
		virtual bool processMenu( DoMenuResult Result);

	protected:
		// Initial display.
		virtual void display( void);
		// Regular display update.
		virtual void update( void);

	public:
		Screen_Status_Menu_Calibrations( void);
		virtual ~Screen_Status_Menu_Calibrations( void);
};
