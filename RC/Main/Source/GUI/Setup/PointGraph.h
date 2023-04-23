// Copyright 2013 Peter Stegemann

#pragma once

#include "Graph.h"

class GUI_Setup_PointGraph : public GUI_Setup_Graph
{
	private:
	bool calculateGraph( const int16_t Point[], uint8_t Points,
						 int16_t GraphPoint[], uint8_t* GraphPoints);

	public:
		// This will update the view to reflect the given map.
		void Display( int16_t Marker, const int16_t Point[], uint8_t Points);
};
