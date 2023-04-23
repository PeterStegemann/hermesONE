// Copyright 2008 Peter Stegemann

#ifndef SCREEN_SETUP_MODELS_H
#define SCREEN_SETUP_MODELS_H

#include "BaseList.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "Setup/Defines.h"

class Screen_Setup_Models : public Screen_Setup_BaseList
{
	private:
		uint8_t setupTypeId;
		uint8_t model[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];

		GUI_Setup_Label modelLabel[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];
		char modelName[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES][ SETUP_MODEL_NAME_SIZE + 1];

		virtual void display( void);
		virtual bool processMenu( DoMenuResult Result);

		void reDisplay( void);
		void displayMarker( void);

		void doChanged( void);
		void doAdd( void);
		void doSelect( uint8_t LineId);
		void doSelectModel( uint8_t SetupModelId);
		void doDelete( uint8_t LineId);

	public:
		Screen_Setup_Models( uint8_t SetupTypeId);
};

#endif
