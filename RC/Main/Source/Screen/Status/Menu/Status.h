// Copyright 2011 Peter Stegemann

#pragma once

#include "Base.h"

class Screen_Status_Menu_Status : public Screen_Status_Menu_Base
{
	private:
		// Return false if the screen should exit.
		virtual bool processMenu( DoMenuResult Result);

		static void updateSource( void* Object, uint16_t SetupSourceId);

	protected:
		// Initial display.
		virtual void display( void);

	public:
		Screen_Status_Menu_Status( void);
		virtual ~Screen_Status_Menu_Status( void);
};
