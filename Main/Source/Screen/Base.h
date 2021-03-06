// Copyright 2010 Peter Stegemann

#ifndef SCREEN_BASE_H
#define SCREEN_BASE_H

#include "AVR/Source/Types.h"

#include <string.h>

class Screen_Base
{
	public:
		enum DoMenuResult
		{
			DMR_None,		// Nothing happened.
			DMR_Changed,	// Menu position has changed.
			DMR_Selected,	// Menu entry was selected.
			DMR_LongPress	// Menu entry was pressed long.
		};

	private:
		uint32_t menuPattern;

		// Call this in regular intervalls.
		DoMenuResult doMenu( uint32_t MenuPattern);

		// Draw this screens menu marker.
		virtual void drawMenuMarker( void);

		// Return false if the screen should exit.
		virtual bool processMenu( DoMenuResult Result);

	protected:
		// The current logical entry (not the display line).
		uint8_t currentMenuEntry;

		// Initial display.
		virtual void display( void);
		// Regular display update.
		virtual void update( void);

	public:
		Screen_Base( uint32_t MenuPattern = 0);
		virtual ~Screen_Base( void);

		// Change menu pattern.
		void SetMenuPattern( uint32_t MenuPattern);

		// Rebuild entire display.
		virtual void ReDisplay( void);

		// Run screen.
		void Run( void);
};

#endif
