// Copyright 2007 Peter Stegemann

#ifndef SCREEN_SETUP_BASE_H
#define SCREEN_SETUP_BASE_H

#include "../Base.h"

#include "GUI/Setup/Marker.h"

#define SCREEN_SETUP_BASE_LINE_HEIGHT			(( uint16_t) 15)
#define SCREEN_SETUP_BASE_MENU_MARKER_SIZE		(( uint16_t) 4)
#define SCREEN_SETUP_BASE_MAIN_FONT				FONT::FI_Normal
#define SCREEN_SETUP_BASE_TITLE_FONT			FONT::FI_Large
#define SCREEN_SETUP_BASE_GAUGE_THICKNESS		SCREEN_SETUP_BASE_LINE_HEIGHT - 3

class GUI_Setup_Label;

class Screen_Setup_Base : public Screen_Base
{
	private:
		bool manageMenuMarker;
		const flash_char* title;

		uint16_t lastSequence;

		// Draw this screens frame. This will also set the left and top value depending on the space
		// the frame needs.
		void drawFrame( void);

		// Draw this screens menu marker.
		virtual void drawMenuMarker( void);

	protected:
		uint16_t markerLeft, markerTop, menuLeft;
		uint16_t frameLeft, frameTop, frameWidth, frameHeight;

		GUI_Setup_Marker menuMarker;

		static void SetSourceLabel( GUI_Setup_Label* Label, char* Name, uint8_t SignalSourceId);

		// Update screen.
		virtual void update( void);

		static void staticUpdate( void* Object);

	public:
		Screen_Setup_Base( uint32_t MenuPattern, const flash_char* Title = NULL,
						   bool ManageMenuMarker = true);
		virtual ~Screen_Setup_Base( void);

		// Rebuild entire display.
		virtual void ReDisplay( void);
};

#endif
