// Copyright 2008 Peter Stegemann

#ifndef SCREEN_SETUP_TYPES_H
#define SCREEN_SETUP_TYPES_H

#include "BaseList.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "Setup/Defines.h"

class Screen_Setup_Types : public Screen_Setup_BaseList
{
	private:
		uint8_t selectedTypeId;
		uint8_t type[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];

		GUI_Setup_Label typeLabel[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];
		char typeName[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES][ SETUP_MODEL_NAME_SIZE + 1];

		virtual void display( void);
		virtual bool processMenu( DoMenuResult Result);

		void reDisplay( void);
		void displayMarker( void);

		void doChanged( void);
		void doAdd( void);
		void doSelect( uint8_t LineId);
		void doDelete( uint8_t LineId);

	public:
		Screen_Setup_Types( void);
};

#endif
