// Copyright 2010 Peter Stegemann

#ifndef SCREEN_STATUS_MENU_CHANNELS_H
#define SCREEN_STATUS_MENU_CHANNELS_H

#include "Base.h"

class Screen_Status_Menu_Channels : public Screen_Status_Menu_Base
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
		Screen_Status_Menu_Channels( void);
		virtual ~Screen_Status_Menu_Channels( void);
};

#endif
