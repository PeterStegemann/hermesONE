// Copyright 2008 Peter Stegemann

#pragma once

#include "BaseList.h"

#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "Setup/Defines.h"
#include "Signal/Source/Source.h"

class Screen_Setup_Sources : public Screen_Setup_BaseList
{
	private:
		Signal_Source_Source::Type sourceType;
		Signal_Source_Source::Level sourceLevel;

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

		void doInput( uint8_t SignalSourceId);
		void doMap( uint8_t SignalSourceId);
		void doMix( uint8_t SignalSourceId);
		void doProxy( uint8_t SignalSourceId);
		void doStore( uint8_t SignalSourceId);
		void doTimer( uint8_t SignalSourceId);
		void doFollower( uint8_t SignalSourceId);
		void doTrimmer( uint8_t SignalSourceId);

		uint8_t countSources( void);

	public:
		Screen_Setup_Sources( Signal_Source_Source::Type SourceType = Signal_Source_Source::T_Empty,
							  Signal_Source_Source::Level SourceLevel =
							  Signal_Source_Source::L_Model);
};
