// Copyright 2008 Peter Stegemann

#include "BaseList.h"

#include <string.h>

Screen_Setup_BaseList::Screen_Setup_BaseList( const flash_char* Title)
					 : Screen_Setup_Base( -1, Title, false)
					 , firstLine( 0)
					 , visibleLines( SCREEN_SETUP_BASELIST_MAXIMUM_LINES)
{
	scrollUpMarker.SetOptions( GUI_Setup_Marker::O_LookUp);
	scrollUpMarker.SetSize( SCREEN_SETUP_BASE_MENU_MARKER_SIZE - 1);
	scrollUpMarker.ForegroundColor = LCD_65K_RGB::C_WarmYellow;
	scrollDownMarker.SetOptions( GUI_Setup_Marker::O_LookDown);
	scrollDownMarker.SetSize( SCREEN_SETUP_BASE_MENU_MARKER_SIZE - 1);
	scrollDownMarker.ForegroundColor = LCD_65K_RGB::C_WarmYellow;
}

void Screen_Setup_BaseList::setupScrollMarkers( uint8_t StartLine)
{
	scrollUpMarker.SetArea( frameLeft, frameTop + ( StartLine * SCREEN_SETUP_BASE_LINE_HEIGHT),
							SCREEN_SETUP_BASE_MENU_MARKER_SIZE + 1,
						    SCREEN_SETUP_BASE_MENU_MARKER_SIZE);
	scrollUpMarker.SetPosition( frameLeft + ( SCREEN_SETUP_BASE_MENU_MARKER_SIZE / 2),
								frameTop + ( StartLine * SCREEN_SETUP_BASE_LINE_HEIGHT) +
								( SCREEN_SETUP_BASE_LINE_HEIGHT / 2) -
							    ( SCREEN_SETUP_BASE_MENU_MARKER_SIZE - 1));

	uint8_t StopLine = StartLine + visibleLines - 1;

	scrollDownMarker.SetArea( frameLeft, frameTop + ( StopLine * SCREEN_SETUP_BASE_LINE_HEIGHT) +
							  ( SCREEN_SETUP_BASE_LINE_HEIGHT / 2) - 1,
			 				  SCREEN_SETUP_BASE_MENU_MARKER_SIZE + 1,
							  SCREEN_SETUP_BASE_MENU_MARKER_SIZE);
	scrollDownMarker.SetPosition( frameLeft + ( SCREEN_SETUP_BASE_MENU_MARKER_SIZE / 2),
								  frameTop + ( StopLine * SCREEN_SETUP_BASE_LINE_HEIGHT) +
								  ( SCREEN_SETUP_BASE_LINE_HEIGHT / 2) - 1);
}

void Screen_Setup_BaseList::reDisplayMarkers( uint8_t Entries)
{
	if( firstLine != 0)
	{
		scrollUpMarker.Display();
	}
	else
	{
		scrollUpMarker.Clear();
	}
	
	if( Entries > ( firstLine + visibleLines))
	{
		scrollDownMarker.Display();
	}
	else
	{
		scrollDownMarker.Clear();
	}
}

bool Screen_Setup_BaseList::doChanged( uint8_t Entries, uint8_t StartLine)
{
	bool ReDisplay = false;

	uint8_t EndLine;
	
	if( visibleLines < Entries)
	{
		EndLine = StartLine + visibleLines - 1;
	}
	else
	{
		EndLine = StartLine + Entries - 1;
	}
	
	// Scroll down.
	if( currentMenuEntry > ( EndLine - 1))
	{
		// Scroll only when not yet at the bottom.
		if(( firstLine + visibleLines) < Entries)
		{
			uint8_t ScrollJump = currentMenuEntry - ( EndLine - 1);
			
			if(( firstLine + ScrollJump + visibleLines) <= Entries)
			{
				firstLine += ScrollJump;
				currentMenuEntry = ( EndLine - 1);
			}
			else
			{
				// Went too far, correct to maximum.
				firstLine = Entries - visibleLines;
				currentMenuEntry = EndLine;
			}
			
			ReDisplay = true;
		}
		else
		{
			// Don't go beyond bottom.
			currentMenuEntry = EndLine;
		}
	}
	// Scroll up.
	else if( currentMenuEntry < ( StartLine + 1))
	{
		// Scroll only when not yet at the top.
		if( firstLine > 0)
		{
			uint8_t ScrollJump = ( StartLine + 1) - currentMenuEntry;
			
			// If not enough lines are left, some step is left for the marker.
			if( firstLine >= ScrollJump)
			{
				firstLine -= ScrollJump;
				currentMenuEntry = ( StartLine + 1);
			}
			else
			{
				// Went too far, correct to minimum, but let marker go on.
				firstLine = 0;
				currentMenuEntry = ScrollJump - firstLine;
			}

			ReDisplay = true;
		}
	}

	return( ReDisplay);
}

void Screen_Setup_BaseList::adjustAfterDelete( uint8_t Entries, uint8_t StartLine)
{
	uint8_t EndLine;
	
	if( visibleLines < Entries)
	{
		EndLine = StartLine + visibleLines - 1;
	}
	else
	{
		EndLine = StartLine + Entries - 1;
	}
	
	if(( firstLine + visibleLines) >= Entries)
	{
		// Adjust firstLine
		uint8_t ScrollJump = ( firstLine + visibleLines) - Entries;
		
		// Don't jump above the top!
		if( ScrollJump > firstLine)
		{
			ScrollJump = firstLine;
		}
		
		firstLine -= ScrollJump;
		currentMenuEntry += ScrollJump;
	}
	
	// Make sure currentMenuEntry does not drop beyond end of list.
	if( currentMenuEntry > EndLine)
	{
		currentMenuEntry = EndLine;
	}
}
