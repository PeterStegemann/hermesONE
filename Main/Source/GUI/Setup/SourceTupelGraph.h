// Copyright 2013 Peter Stegemann

#pragma once

#include "Graph.h"

class GUI_Setup_SourceTupelGraph : public GUI_Setup_Graph
{
	private:
		int16_t getPoint( const Setup_Source_Tupel Point[], uint8_t PointId);
		bool calculateGraph( const Setup_Source_Tupel Point[], uint8_t Points,
							 int16_t GraphPoint[], uint8_t* GraphPoints);

	public:
		// This will update the view to reflect the given map.
		void Display( int16_t Marker, const Setup_Source_Tupel Point[], uint8_t Points);
};
