// Copyright 2010 Peter Stegemann

#ifndef SCREEN_STATUS_MENU_DISPLAY_H
#define SCREEN_STATUS_MENU_DISPLAY_H

#include "Base.h"

class Screen_Status_Menu_Display : public Screen_Status_Menu_Base
{
	private:
		void updateBacklight( int8_t Value);
		static void updateBacklight( void* Object, int8_t Value);

		void updateContrast( int8_t Value);
		static void updateContrast( void* Object, int8_t Value);

		void updateBlankTime( uint16_t Value);
		static void updateBlankTime( void* Object, uint16_t Value);

		// Return false if the screen should exit.
		virtual bool processMenu( DoMenuResult Result);

	protected:
		// Initial display.
		virtual void display( void);

	public:
		Screen_Status_Menu_Display( void);
		virtual ~Screen_Status_Menu_Display( void);
};

#endif
