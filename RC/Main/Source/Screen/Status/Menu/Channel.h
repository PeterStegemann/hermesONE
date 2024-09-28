// Copyright 2010 Peter Stegemann

#pragma once

#include "Base.h"

#include "Signal/Channel.h"

class Screen_Status_Menu_Channel : public Screen_Status_Menu_Base
{
	private:
		uint8_t channelId;

		Signal_Channel* channel;

		static void updateSource( void* Object, uint16_t SetupSourceId);
		void updatePoint( uint8_t PointId);
		void updatePointValue( int16_t Value);
		static void updatePointValue( void* Object, int16_t Value);
		bool changePoint( uint8_t PointId);

		// Return false if the screen should exit.
		virtual bool processMenu( DoMenuResult Result);

	protected:
		// Initial display.
		virtual void display( void);

	public:
		Screen_Status_Menu_Channel( Input_Service* InputService, uint8_t ChannelId);
		virtual ~Screen_Status_Menu_Channel( void);
};
