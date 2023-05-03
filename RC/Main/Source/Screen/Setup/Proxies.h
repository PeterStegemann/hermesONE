// Copyright 2013 Peter Stegemann

#pragma once

#include "BaseList.h"

#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "Setup/Defines.h"
#include "Signal/Source/Source.h"

class Screen_Setup_Proxies : public Screen_Setup_BaseList
{
	private:
		uint8_t filledSourceLines;

		uint8_t source[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];

		GUI_Setup_Label sourceLabel[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];
		char sourceName[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES][ SETUP_SOURCE_NAME_SIZE + 1];
		GUI_Setup_Gauge valueGauge[ SCREEN_SETUP_BASELIST_MAXIMUM_LINES];

		virtual void display( void);
		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

		void reDisplay( void);
		void displayMarker( void);

		void doChanged( void);
		void doAdd( void);
		void doSelect( uint8_t LineId);
		void doSelectSource( uint8_t SignalSourceId);
		void doDelete( uint8_t LineId);

		uint8_t countProxies( void);

	public:
		Screen_Setup_Proxies( void);
};
