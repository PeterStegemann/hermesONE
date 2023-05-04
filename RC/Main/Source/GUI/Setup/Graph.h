// Copyright 2007 Peter Stegemann

#pragma once

#include "Setup/Source/Tuple.h"
#include "Text/Text.h"

#include "AVR/Source/LCD/LCD_65K_RGB.h"

#define GUI_SETUP_GRAPH_POINTS		    7
#define GUI_SETUP_GRAPH_LABEL_FONT		FONT::FI_Medium

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
    // Marks whether we have remembered values from last draw.
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

    void display( int16_t Marker, int16_t NewGraphPoint[], uint8_t NewGraphPoints, bool GraphUpdated)
    {
        int32_t NewMarker = Marker;
        NewMarker -= SIGNAL_MINIMUM_VALUE;
        NewMarker *= ( uint32_t)( size - 5);
        NewMarker /= ( uint32_t) SIGNAL_VALUE_RANGE;

        if( remembered == false)
        {
            // Remember new values.
            remembered = true;

            updateGraph( NewGraphPoint, NewGraphPoints);
        }
        else
        {
            bool MarkerUpdated = NewMarker != marker;

            if( GraphUpdated == true)
            {
                // Clear graph.
                displayGraph( COPM_Clear);

                updateGraph( NewGraphPoint, NewGraphPoints);
            }
            else if( MarkerUpdated == true)
            {
                // Clear old marker?
                if( marker >= 0)
                {
                    GLOBAL.SetupDisplay.DrawVerticalLine( left + 2 + marker, top + 2, size - 4, backgroundColor);
                }
            }
            else
            {
                // No drawing needed.
                return;
            }
        }

        marker = NewMarker;

        // Display graph.
        displayGraph( COPM_Print);
    }

  private:
    // Internal display with a "print" or "clear" mode.
    enum ClearOrPrintMode
    {
        COPM_Clear,
        COPM_Print
    };

    // Copy new values.
    void updateGraph( int16_t NewGraphPoint[], uint8_t NewGraphPoints)
    {
    	for( uint8_t PointId = 0; PointId < NewGraphPoints; PointId++)
	    {
    		graphPoint[ PointId] = NewGraphPoint[ PointId];
	    }

    	graphPoints = NewGraphPoints;
    }

    void displayFrame( void)
    {
        if( size == 0)
        {
            return;
        }

        const FONT_Type* Font = FONT::GetFont( GUI_SETUP_GRAPH_LABEL_FONT);

        uint16_t LabelLeft = left - 3 * Font->CellWidth;
        uint16_t LabelTop = ( top - Font->CellHeight);

        GLOBAL.SetupDisplay.Print_P( LabelLeft, LabelTop, GUI_SETUP_GRAPH_LABEL_FONT, detailColor,
                                     backgroundColor, LCD_65K_RGB::PO_Fixed, Text::Minus100Percent);

        LabelLeft += size / 2;
        GLOBAL.SetupDisplay.Print_P( LabelLeft, LabelTop, GUI_SETUP_GRAPH_LABEL_FONT, detailColor,
                                     backgroundColor, LCD_65K_RGB::PO_Fixed, Text::PaddedZeroPercent);

        LabelLeft += size / 2 + 1;
        LabelLeft += ( 1 * Font->CellWidth);
        GLOBAL.SetupDisplay.Print_P( LabelLeft, LabelTop, GUI_SETUP_GRAPH_LABEL_FONT, detailColor,
                                     backgroundColor, LCD_65K_RGB::PO_Fixed, Text::Plus100Percent);

        LabelTop += size / 2;
        LabelTop += Font->CellHeight / 2;
        LabelLeft += ( 2 * Font->CellWidth) + 1;
        GLOBAL.SetupDisplay.Print_P( LabelLeft, LabelTop, GUI_SETUP_GRAPH_LABEL_FONT, detailColor,
                                     backgroundColor, LCD_65K_RGB::PO_Fixed, Text::ZeroPercent);

        LabelTop += size / 2;
        LabelTop += ( Font->CellHeight / 2) + 2;
        LabelLeft -= ( 3 * Font->CellWidth);
        GLOBAL.SetupDisplay.Print_P( LabelLeft, LabelTop, GUI_SETUP_GRAPH_LABEL_FONT, detailColor,
                                     backgroundColor, LCD_65K_RGB::PO_Fixed, Text::Minus100Percent);

        // Border
        GLOBAL.SetupDisplay.DrawRect( left, top, size, size, foregroundColor, LCD::RO_Rounded);
        GLOBAL.SetupDisplay.DrawRect( left + 1, top + 1, size - 2, size - 2, foregroundColor, LCD::RO_Boxed);
    }

    void displayGraph( ClearOrPrintMode ClearOrPrint)
    {
        if( size == 0)
        {
            return;
        }

        LCD_65K_RGB::Color UseForegroundColor;
        LCD_65K_RGB::Color UseDetailColor;

        switch( ClearOrPrint)
        {
            case COPM_Clear :
            {
                UseForegroundColor = backgroundColor;
                UseDetailColor = backgroundColor;
            }
            break;

            default :
            case COPM_Print :
            {
                UseForegroundColor = foregroundColor;
                UseDetailColor = detailColor;
            }
            break;
        }

        // Move graph inside frame.
        uint16_t Left = left + 2;
        uint16_t Top = top + 2;

        if( marker >= 0)
        {
            GLOBAL.SetupDisplay.DrawVerticalLine( Left + marker, Top, size - 4, UseDetailColor);
        }

        uint16_t Size = size - 5;

        if( graphPoints == 0)
        {
            // Done.
        }
        else if( graphPoints == 1)
        {
            uint16_t X = Left;
            uint16_t Y = Top + Size - graphPoint[ 0];

            GLOBAL.SetupDisplay.DrawLine( X, Y, X + Size + 1, Y, UseForegroundColor);

            X = Left + Size / 2;

            GLOBAL.SetupDisplay.DrawDot( X, Y, UseDetailColor);
        }
        else if( mode == COWM_Warp)
        {
            uint16_t LastX = 0;
            uint16_t LastY = 0;

            for( uint8_t GraphId = 0; GraphId < graphPoints; GraphId++)
            {
                uint16_t X = Left + ( GraphId * Size) / ( graphPoints - 1);
                uint16_t Y = Top + Size - graphPoint[ GraphId];

                if( GraphId != 0)
                {
                    GLOBAL.SetupDisplay.DrawLine( LastX, LastY, X, Y, UseForegroundColor);
                }

                GLOBAL.SetupDisplay.DrawDot( X, Y, UseDetailColor);

                LastX = X;
                LastY = Y;
            }
        }
        else if( mode == COWM_Clip)
        {
            // This only works well with 3 points, which is what this was made for.
            uint16_t Center = graphPoint[ 1];
            uint16_t LowerValue = graphPoint[ 0];
            uint16_t UpperValue = graphPoint[ 2];

            // Lower _must_ be below upper.
            if( LowerValue > UpperValue)
            {
                uint16_t Temp = LowerValue;
                LowerValue = UpperValue;
                UpperValue = Temp;
            }

            int16_t CorrectedCenter = Center - ( Size / 2);
            int16_t LowerX = LowerValue - CorrectedCenter;
            int16_t UpperX = UpperValue - CorrectedCenter;

            // Draw.
            drawLine( Left, Top, 0, LowerValue, LowerX, LowerValue, Size, UseForegroundColor);
            drawLine( Left, Top, LowerX, LowerValue, UpperX, UpperValue, Size, UseForegroundColor);
            drawLine( Left, Top, UpperX, UpperValue, Size + 1, UpperValue, Size, UseForegroundColor);
        }

        // Draw frame on first draw.
        displayFrame();
    }

    void drawLine( uint16_t Left, uint16_t Top, int16_t LowerX, int16_t LowerY, int16_t UpperX, int16_t UpperY,
                   uint16_t Size, LCD_65K_RGB::Color Color)
    {
        if(( UpperX <= 0) || ( LowerX > ( int16_t) Size))
        {
            // Not visible at all.
            return;
        }

        if( LowerX <= 0)
        {
            if( LowerY != UpperY)
            {
                LowerY -= LowerX;
            }
            LowerX = 0;
        }

        if( UpperX > ( int16_t) Size + 1)
        {
            int16_t Clip = UpperX - Size;
            if( LowerY != UpperY)
            {
                UpperY -= Clip;
            }
            UpperX = Size;
        }

        // Map to frame.
        LowerX = Left + LowerX;
        LowerY = Top + Size - LowerY;

        UpperX = Left + UpperX;
        UpperY = Top + Size - UpperY;

        // Draw.
        GLOBAL.SetupDisplay.DrawLine( LowerX, LowerY, UpperX, UpperY, Color);
    }

    LCD_65K_RGB::Color foregroundColor;
    LCD_65K_RGB::Color backgroundColor;
    LCD_65K_RGB::Color detailColor;

  public:
    GUI_Setup_Graph( void)
       : remembered( false)
       , marker( -1)
       , left( 0)
       , top( 0)
       , mode( COWM_Warp)
       , size( 0)
       , graphPoints( 0)
       , foregroundColor( LCD_65K_RGB::C_White)
       , backgroundColor( LCD_65K_RGB::C_Black)
       , detailColor( LCD_65K_RGB::C_WarmYellow)
    {
    }

    // Set the location of the map on the screen.
    void SetDimensions( uint16_t Left, uint16_t Top, uint16_t Size)
    {
        left = Left;
        top = Top;
        size = Size;

        const FONT_Type* Font = FONT::GetFont( GUI_SETUP_GRAPH_LABEL_FONT);

        size -= UTILITY_Maximum( Font->CellHeight, Font->CellWidth);

        // First draw will redraw everything.
        remembered = false;
    }

    void SetMode( ClipOrWarpMode Mode)
    {
        if( mode == Mode)
        {
            return;
        }

        // Clear graph.
        displayGraph( COPM_Clear);

        mode = Mode;

        // First draw will redraw everything.
        remembered = false;
    }
};
