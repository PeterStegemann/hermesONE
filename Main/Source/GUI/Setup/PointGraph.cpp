// Copyright 2013 Peter Stegemann

#include "PointGraph.h"

#include "Main/Setup.h"
#include "Text/Text.h"

#include "AVR/Components/Utility.h"

bool GUI_Setup_PointGraph::calculateGraph( const int16_t Point[], uint8_t Points,
										   int16_t GraphPoint[], uint8_t* GraphPoints)
{
	bool Updated = false;

	// Number of points we can do is limited...
	if( Points > GUI_SETUP_GRAPH_POINTS)
	{
		Points = GUI_SETUP_GRAPH_POINTS;
	}

	uint8_t GraphId = 0;

	for( uint8_t PointId = 0; PointId < Points; PointId++)
	{
		int32_t CurrentPoint = Point[ PointId];

		if( CurrentPoint >= SIGNAL_MINIMUM_VALUE)
		{
			uint32_t NormalizedPoint = CurrentPoint - SIGNAL_MINIMUM_VALUE;
			int16_t NewGraphPoint =
				(( NormalizedPoint * ( uint32_t)( size - 5)) / ( uint32_t) SIGNAL_VALUE_RANGE);

			if( NewGraphPoint != graphPoint[ GraphId])
			{
				Updated = true;
			}

			GraphPoint[ GraphId] = NewGraphPoint;

			GraphId++;
		}
	}

	if( graphPoints != GraphId)
	{
		Updated = true;
	}

	*GraphPoints = GraphId;

	return( Updated);
}

void GUI_Setup_PointGraph::Display( int16_t Marker, const int16_t Point[], uint8_t Points)
{
	int16_t GraphPoint[ GUI_SETUP_GRAPH_POINTS];
	uint8_t GraphPoints = 0;

	bool Updated = calculateGraph( Point, Points, GraphPoint, &GraphPoints);

	display( Marker, GraphPoint, GraphPoints, Updated);
}
