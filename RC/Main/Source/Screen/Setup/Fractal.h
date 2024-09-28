// Copyright 2006 Peter Stegemann. All rights reserved.

#pragma once

#include "Screen.h"

class Screen_Setup_Fractal : public Screen_Setup_Base
{
	private:
		float centerX, centerY, height, width;
		float cornerX, cornerY;
		uint16_t resolutionLeft, resolutionTop, resolutionWidth, resolutionHeight;
		uint64_t maximumIteration;
/*
		bool hasSubFractals;

		uint64_t fractalArray[ 4000];
		uint64_t fractalWidth, fractalHeight;

		uint64_t singleIteration;

		void calculate( void);
*/
		uint64_t calculatePoint( uint16_t X, uint16_t Y);

//		void render( uint64_t Left, uint64_t Top);

	public:
		Screen_Setup_Fractal( Input_Service* InputService, Screen_Status_Status* StatusScreen);
//		~Fractal( void);

		void SetCenter( float X, float Y);
		void SetSize( float Height);
		void SetResolution( uint16_t Left, uint16_t Top, uint16_t Width, uint16_t Height);
		void SetMaximumIteration( uint64_t Iteration);

		// If IsMaximumIteration is supplied, it is set to true when the point has reached maximum
		// iteration.
//		uint64_t GetPoint( uint64_t X, uint64_t Y, bool* IsMaximumIteration = NULL);

		// Run this screen.
		void Run( void);
};
