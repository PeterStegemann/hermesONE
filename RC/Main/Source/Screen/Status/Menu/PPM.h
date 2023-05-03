// Copyright 2014 Peter Stegemann

#pragma once

#include "Base.h"
#include "Setup/PPM.h"

class Screen_Status_Menu_PPM : public Screen_Status_Menu_Base
{
	private:
		uint8_t ppmId;
		Setup_PPM ppmSetup;

		// Return false if the screen should exit.
		virtual bool processMenu( DoMenuResult Result);

	protected:
		// Initial display.
		virtual void display( void);

	public:
		Screen_Status_Menu_PPM( uint8_t PPMId);
		virtual ~Screen_Status_Menu_PPM( void);
};
