// Copyright 2007 Peter Stegemann

#pragma once

#include "Setup/Source/Tuple.h"

#include "AVR/Source/LCD/LCD_65K_RGB.h"

#define GUI_SETUP_GRAPH_POINTS		7

class GUI_Setup_Graph
{
	public:
		// Whether to clip or warp the endpoints.
		enum ClipOrWarpMode
		{
			COWM_Clip,
			COWM_Warp
		};

	private:
		// Marks wether we have remembered values from last draw.
		bool remembered;

		int16_t marker;

		// Location.
		uint16_t left, top;

		ClipOrWarpMode mode;

	protected:
		uint16_t size;

		// Remembered values.
		int16_t graphPoint[ GUI_SETUP_GRAPH_POINTS];
		uint8_t graphPoints;

		void display( int16_t Marker, int16_t NewGraphPoint[], uint8_t NewGraphPoints,
					  bool GraphUpdated);

	private:
		// Internal display with a "print" or "clear" mode.
		enum ClearOrPrintMode
		{
			COPM_Clear,
			COPM_Print
		};

		// Copy new values.
		void updateGraph( int16_t NewGraphPoint[], uint8_t NewGraphPoints);

		void displayFrame( void);
		void displayGraph( ClearOrPrintMode ClearOrPrint);

		void drawLine( uint16_t Left, uint16_t Top, int16_t LowerX, int16_t LowerY,
					   int16_t UpperX, int16_t UpperY, uint16_t Size, LCD_65K_RGB::Color Color);

		LCD_65K_RGB::Color foregroundColor;
		LCD_65K_RGB::Color backgroundColor;
		LCD_65K_RGB::Color detailColor;

	public:
		GUI_Setup_Graph( void);

		// Set the location of the map on the screen.
		void SetDimensions( uint16_t Left, uint16_t Top, uint16_t Size);

		void SetMode( ClipOrWarpMode Mode);
};
