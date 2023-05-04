// Copyright 2013 Peter Stegemann

#pragma once

#include "Graph.h"

#include "Main/Setup.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

class GUI_Setup_SourceTupleGraph : public GUI_Setup_Graph
{
  private:
    int16_t getPoint( const Setup_Source_Tuple Point[], uint8_t PointId)
    {
        uint8_t SignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( Point[ PointId].Source);
        int16_t Volume = Point[ PointId].Volume;

        if( SignalSourceId == SIGNAL_SOURCE_NONE)
        {
            // No source, use neutral value.
            return( SIGNAL_NEUTRAL_VALUE);
        }
        else if( SignalSourceId == SIGNAL_SOURCE_FIXED)
        {
            return( Volume);
        }
        else
        {
            int32_t Value = GLOBAL.SignalProcessor.GetSourceValue( SignalSourceId);

            Value *= Volume;
            Value /= SIGNAL_CHANNEL_100_PERCENT_VALUE;

            if( Value < SIGNAL_MINIMUM_VALUE)
            {
                Value = SIGNAL_MINIMUM_VALUE;
            }
            else if( Value > SIGNAL_MAXIMUM_VALUE)
            {
                Value = SIGNAL_MAXIMUM_VALUE;
            }

            return( Value);
        }
    }

    bool calculateGraph( const Setup_Source_Tuple Point[], uint8_t Points, int16_t GraphPoint[], uint8_t* GraphPoints)
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
            if( Point[ PointId].Source != SETUP_SOURCE_NONE)
            {
                int32_t CurrentPoint = getPoint( Point, PointId);
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

  public:
    // This will update the view to reflect the given map.
    void Display( int16_t Marker, const Setup_Source_Tuple Point[], uint8_t Points)
    {
    	int16_t GraphPoint[ GUI_SETUP_GRAPH_POINTS];
    	uint8_t GraphPoints = 0;

    	bool Updated = calculateGraph( Point, Points, GraphPoint, &GraphPoints);

    	display( Marker, GraphPoint, GraphPoints, Updated);
    }
};
