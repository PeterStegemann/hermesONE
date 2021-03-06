// Copyright 2011 Peter Stegemann

#ifndef SCREEN_STATUS_MENU_CHANNELMAPPING_H
#define SCREEN_STATUS_MENU_CHANNELMAPPING_H

#include "Base.h"
#include "Setup/PPM.h"

class Screen_Status_Menu_ChannelMapping : public Screen_Status_Menu_Base
{
	private:
		uint8_t ppmId;
		Setup_PPM ppmSetup;

		void updateChannelMapping( int8_t Value);
		static void updateChannelMapping( void* Object, int8_t Value);

		// Return false if the screen should exit.
		virtual bool processMenu( DoMenuResult Result);

	protected:
		// Initial display.
		virtual void display( void);

	public:
		Screen_Status_Menu_ChannelMapping( uint8_t PPMId);
		virtual ~Screen_Status_Menu_ChannelMapping( void);
};

#endif
