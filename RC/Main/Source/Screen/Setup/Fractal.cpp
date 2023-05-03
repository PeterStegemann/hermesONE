// Copyright 2006 Peter Stegemann. All rights reserved.

#include "Fractal.h"

#include "Input.h"
#include "Main/Setup.h"
#include "Ports.h"

#include "AVR/Components/Utility.h"
#include "AVR/Components/LCD/LCD.h"

using namespace Screen::Setup;

Fractal::Fractal( void)
	   : centerX( -0.74)
       , centerY( 0.15)
       , height( 1.1)
       , resolutionWidth( 640)
       , resolutionHeight( 480)
       , maximumIteration( 1024)
/*       , hasSubFractals( false)
       , leftTopFractal( NULL)
	   , rightTopFractal( NULL)
       , leftBottomFractal( NULL)
	   , rightBottomFractal( NULL)
	   , fractalArray( 0)
	   , singleIteration( 0)
       , fractalWidth( 0)
       , fractalHeight( 0)
*/
{
}
/*
Fractal::~Fractal( void)
{
	free( fractalArray);
}

void Fractal::calculate( void)
{
	// Clean up old fractal.
	// Remove array if available.
	sMemory::Release( fractalArray);
	fractalArray = NULL;

	// Set new dimensions.
	fractalWidth = resolutionWidth;
	fractalHeight = resolutionHeight;
	
	width = height * resolutionWidth / resolutionHeight;
	
	cornerX = centerX - width / 2;
	cornerY = centerY - height / 2;

	// Check whether we're a single iteration fractal by calculating a frame around the fractal.
	uint64_t X, Y;
	bool IsSingleIteration = true;

	singleIteration = calculatePoint( 0, 0);

	// Calculate top line.
	X = 0;
	Y = 0;

	while( X < fractalWidth)
	{
		if( singleIteration != calculatePoint( X, Y))
		{
			IsSingleIteration = false;

			break;
		}

		X++;
	}

	if( IsSingleIteration)
	{
		// Calculate bottom line.
		X = 0;
		Y = fractalHeight - 1;

		while( X < fractalWidth)
		{
			if( singleIteration != calculatePoint( X, Y))
			{
				IsSingleIteration = false;
				
				break;
			}
			
			X++;
		}
	}

	if( IsSingleIteration)
	{
		// Calculate left line.
		X = 0;
		Y = fractalHeight - 2;

		while( Y > 0)
		{
			if( singleIteration != calculatePoint( X, Y))
			{
				IsSingleIteration = false;
				
				break;
			}
			
			Y--;
		}
	}
	
	if( IsSingleIteration)
	{
		X = fractalWidth - 1;
		Y = fractalHeight - 2;
		
		while( Y > 0)
		{
			if( singleIteration != calculatePoint( X, Y))
			{
				IsSingleIteration = false;
				
				break;
			}

			Y--;
		}
	}
 
	if( IsSingleIteration)
	{
		// We're a single iteration fractal. Done.
		return;
	}

	// If this rect is not very small, go into recursion and split it into 4 subfractals.
	if(( resolutionWidth > 10) && ( resolutionHeight > 10))
	{
		hasSubFractals = true;

		uint64_t FractalMiddleX = fractalWidth / 2;
		uint64_t FractalMiddleY = fractalHeight / 2;
		float HalfHeight = height / 2;
		float QuarterHeight = height / 4;
//		float HalfWidth = width / 2;
		float QuarterWidth = width / 4;

		Fractal leftTopFractal;
		Fractal rightTopFractal;
		Fractal leftBottomFractal;
		Fractal rightBottomFractal;
		
		Fractal leftTopFractal( centerX - QuarterWidth, centerY - QuarterHeight,
									  HalfHeight, FractalMiddleX, FractalMiddleY, maximumIteration);
		Fractal rightTopFractal( centerX + QuarterWidth, centerY - QuarterHeight, HalfHeight,
									   fractalWidth - FractalMiddleX, FractalMiddleY,
									   maximumIteration);
		Fractal leftBottomFractal( centerX - QuarterWidth, centerY + QuarterHeight,
										 HalfHeight, FractalMiddleX, fractalHeight - FractalMiddleY,
										 maximumIteration);
		Fractal rightBottomFractal( centerX + QuarterWidth, centerY + QuarterHeight,
										  HalfHeight, fractalWidth - FractalMiddleX,
										  fractalHeight - FractalMiddleY, maximumIteration);

		leftTopFractal->Calculate();
		rightTopFractal->Calculate();
		leftBottomFractal->Calculate();
		rightBottomFractal->Calculate();

		return;
	}

	// We need a fractal array.
	uint64_t NewArraySize = sizeof( uint64_t) * fractalWidth * fractalHeight;
	fractalArray = static_cast< uint64_t*>( sMemory::Allocate( NewArraySize));

	// Now calculate it.
	X = 0;

	while( X < fractalWidth)
	{
		Y = 0;

		while( Y < fractalHeight)
		{
			fractalArray[ X + ( Y * fractalWidth)] = calculatePoint( X, Y);
			
			Y++;
		}
		
		X++;
	}
}
*/
uint64_t Fractal::calculatePoint( uint16_t X, uint16_t Y)
{
 	float Zr, Zi, Cr, Ci, ZrSqr, ZiSqr;
	Cr = Zr = cornerX + width * X / resolutionWidth;
	Ci = Zi = cornerY + height * Y / resolutionHeight;

	uint64_t Iteration = 0;

	while( Iteration != maximumIteration)
	{
		ZrSqr = Zr * Zr;
		ZiSqr = Zi * Zi;

		if(( ZrSqr + ZiSqr) >= 16.0)
		{
			break;
		}

		float ZrNew = ZrSqr - ZiSqr + Cr;
		Zi = 2 * Zr * Zi + Ci;
		Zr = ZrNew;

		Iteration++;
	}

	return( Iteration);
}

void Fractal::SetCenter( float X, float Y)
{
	centerX = X;
	centerY = Y;
}

void Fractal::SetSize( float Height)
{
	height = Height;
}

void Fractal::SetResolution( uint16_t Left, uint16_t Top, uint16_t Width, uint16_t Height)
{
	resolutionLeft = Left;
	resolutionTop = Top;
	resolutionWidth = Width;
	resolutionHeight = Height;
}

void Fractal::SetMaximumIteration( uint64_t Iteration)
{
	maximumIteration = Iteration;
}
/*
uint64_t Fractal::GetPoint( uint64_t X, uint64_t Y, bool* IsMaximumIteration)
{
	// In range?
	if(( X >= fractalWidth) || ( Y >= fractalHeight))
	{
		return( 0);
	}

	// Get data from subfractals.
	if( hasSubFractals)
	{
		uint64_t SubWidth, SubHeight;

		leftTopFractal->GetResolution( &SubWidth, &SubHeight);

		if( X < SubWidth)
		{
			// Is left.
			if( Y < SubHeight)
			{
				// Is top.
				return( leftTopFractal->GetPoint( X, Y, IsMaximumIteration));
			}
			else
			{
				// Is bottom.
				return( leftBottomFractal->GetPoint( X, Y - SubHeight, IsMaximumIteration));
			}
		}
		else
		{
			// Is right.
			if( Y < SubHeight)
			{
				// Is top.
				return( rightTopFractal->GetPoint( X - SubWidth, Y, IsMaximumIteration));
			}
			else
			{
				// Is bottom.
				return( rightBottomFractal->GetPoint( X - SubWidth, Y - SubHeight, IsMaximumIteration));
			}
		}
	}

	uint64_t CurrentIteration;

	// Data available?
	if( fractalArray != NULL)
	{
		// Return data from our storage.
		CurrentIteration = fractalArray[ X + ( Y * fractalWidth)];
	}
	else
	{
		// Treat this as single iterated.
		CurrentIteration = singleIteration;
	}

	if( IsMaximumIteration != NULL)
	{
		*IsMaximumIteration = CurrentIteration == maximumIteration;
	}

	return( CurrentIteration);
}
*/
/*
void Fractal::render( uint64_t Left, uint64_t Top, FractalRendererInterface* Renderer)
{
	// Get data from subfractals.
	if( hasSubFractals)
	{
		uint64_t SubWidth, SubHeight;
		
		leftTopFractal->GetResolution( &SubWidth, &SubHeight);

		leftTopFractal->render( Left, Top, Renderer);
		leftBottomFractal->render( Left, Top + SubHeight, Renderer);
		rightTopFractal->render( Left + SubWidth, Top, Renderer);
		rightBottomFractal->render( Left + SubWidth, Top + SubHeight, Renderer);
	}
	else
	// Data available?
	if( fractalArray != NULL)
	{
		// Now render it.
		uint64_t X = 0;
		
		while( X < fractalWidth)
		{
			uint64_t Y = 0;
			
			while( Y < fractalHeight)
			{
				bool IsMaximumIteration;
				uint64_t CurrentIteration = GetPoint( X, Y, &IsMaximumIteration);

				Renderer->DrawPoint( Left + X, Top + Y, CurrentIteration, IsMaximumIteration);
				
				Y++;
			}
			
			X++;
		}
	}
	else
	{
		// Treat this as single iterated.
		Renderer->DrawRect( Left, Top, fractalWidth, fractalHeight, singleIteration,
							singleIteration == maximumIteration);
	}
}
*/
void Fractal::Run( void)
{
	DrawFrame( "Fractal");

	SetCenter( -0.74, 0.15);
	SetSize( 1.1);
	SetResolution( GetLeft(), GetTop(), GetWidth(), GetHeight());
	SetMaximumIteration( 256);

	// Set new dimensions.
	width = height * resolutionWidth / resolutionHeight;
	
	cornerX = centerX - width / 2;
	cornerY = centerY - height / 2;

	uint16_t X = 0;

	while( X < resolutionWidth)
	{
		uint16_t Y = 0;

		while( Y < resolutionHeight)
		{
			uint64_t CurrentIteration = calculatePoint( X, Y);

//			GetLCD()->DrawPixel( resolutionLeft + X, resolutionTop + Y, ( LCD::Color)( X * Y));
			GetLCD()->DrawPixel( resolutionLeft + X, resolutionTop + Y,
			                     LCD_RGB( CurrentIteration & 7,
			                              CurrentIteration & 192,
			                              CurrentIteration & 63));

			Y++;
		}

		X++;
	}

	while( true)
	{
		uint8_t Changed;
		uint8_t Keys = GetInputKeys()->GetKeys( &Changed);
		
		if( Changed != 0)
		{
			if( UTILITY_GetBit( Changed, INPUT_ENTER) && UTILITY_GetBit( Keys, INPUT_ENTER))
			{
				break;
			}
		}
		
		UTILITY::Pause( 5);
	}
}
