// Copyright 2008 Peter Stegemann

#include "Sources.h"

#include "GUI/Setup/Popup.h"
#include "Main/Global.h"
#include "Source/Follower.h"
#include "Source/Input.h"
#include "Source/Map.h"
#include "Source/Mix.h"
#include "Source/Store.h"
#include "Source/Timer.h"
#include "Source/Trimmer.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

static const flash_char* getTitle( Signal_Source_Source::Type SourceType)
{
	switch( SourceType)
	{
		case Signal_Source_Source::T_Empty :		return( Text::Sources);
		case Signal_Source_Source::T_Input :		return( Text::Inputs);
		case Signal_Source_Source::T_Map :			return( Text::Maps);
		case Signal_Source_Source::T_Mix :			return( Text::Mixers);
		case Signal_Source_Source::T_Store :		return( Text::Stores);
		case Signal_Source_Source::T_Timer :		return( Text::Timers);
		case Signal_Source_Source::T_Follower :		return( Text::Followers);
		case Signal_Source_Source::T_Trimmer :		return( Text::Trimmers);
		case Signal_Source_Source::T_Proxy :		return( Text::Proxies);
	}

	return( NULL);
}

Screen_Setup_Sources::Screen_Setup_Sources( Signal_Source_Source::Type SourceType,
										    Signal_Source_Source::Level SourceLevel)
					: Screen_Setup_BaseList( getTitle( SourceType))
					, sourceType( SourceType)
					, sourceLevel( SourceLevel)
					, filledSourceLines( 0)
{
	if( sourceType != Signal_Source_Source::T_Empty)
	{
		visibleLines = SCREEN_SETUP_BASELIST_MAXIMUM_LINES - 2;
	}

	for( uint8_t SourceLine = 0; SourceLine < visibleLines; SourceLine++)
	{
		valueGauge[ SourceLine].SetOptions(
			( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
										 GUI_Setup_Gauge::O_DualPercentage |
										 GUI_Setup_Gauge::O_CenterLine |
										 GUI_Setup_Gauge::O_Marker));
	}
}

void Screen_Setup_Sources::display( void)
{
	displayMarker();

	// Adjust gauges to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	Line += 2;

	if( sourceType != Signal_Source_Source::T_Empty)
	{
		GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
									 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
									 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Add);

		Line += 2;
	}

	setupScrollMarkers( Line);

	uint16_t GaugeLeft = menuLeft + ( SETUP_SOURCE_NAME_SIZE + 1) * Font->CellWidth;
	uint16_t GaugeWidth = frameWidth - ( GaugeLeft - frameLeft) - 1;

	for( uint8_t SourceLine = 0; SourceLine < visibleLines; SourceLine++)
	{
		uint16_t LineTop = frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT);

		sourceLabel[ SourceLine].SetDimensions( menuLeft, LineTop);
		valueGauge[ SourceLine].SetDimensions( GaugeLeft, LineTop, GaugeWidth,
											   SCREEN_SETUP_BASE_GAUGE_THICKNESS);

		Line++;
	}

	reDisplay();
}

void Screen_Setup_Sources::reDisplay( void)
{
	// Clear all channels.
	for( uint8_t SourceLine = 0; SourceLine < filledSourceLines; SourceLine++)
	{
		valueGauge[ SourceLine].Clear();
	}

	// Find sources to display.
	uint8_t SignalSourceId = 0;
	uint8_t SourcesMatched = 0;
	uint8_t SourceLine = 0;

	// Loop sources.
	for(; ( SourceLine < visibleLines) && ( SignalSourceId < SIGNAL_SOURCES); SignalSourceId++)
	{
		const Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

		Signal_Source_Source::Type CurrentSourceType = Source->GetType();

		// Ignore empty sources.
		if( CurrentSourceType == Signal_Source_Source::T_Empty)
		{
			continue;
		}

		// Ignore proxies.
		if( CurrentSourceType == Signal_Source_Source::T_Proxy)
		{
			continue;
		}

		// Only sources on the same level.
		if( Source->GetLevel() != sourceLevel)
		{
			continue;
		}

		if(( sourceType == Signal_Source_Source::T_All) || ( CurrentSourceType == sourceType))
		{
			// Skip all until firstLine is reached.
			if( SourcesMatched >= firstLine)
			{
				source[ SourceLine] = SignalSourceId;

				SourceLine++;
			}

			SourcesMatched++;
		}
	}

	filledSourceLines = SourceLine;

	SourceLine = 0;

	// Update labels.
	while( SourceLine < filledSourceLines)
	{
		SetSourceLabel( &( sourceLabel[ SourceLine]), sourceName[ SourceLine], source[ SourceLine]);

		SourceLine++;
	}

	reDisplayMarkers( countSources());
}

void Screen_Setup_Sources::update( void)
{
	Screen_Setup_BaseList::update();
	
	// Print all channels.
	for( uint8_t SourceLine = 0; SourceLine < filledSourceLines; SourceLine++)
	{
		valueGauge[ SourceLine].Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
										 GLOBAL.SignalProcessor.GetSourceValue(
											source[ SourceLine]));
	}
}

bool Screen_Setup_Sources::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			if( currentMenuEntry == 0)
			{
				return( false);
			}

			if( sourceType == Signal_Source_Source::T_Empty)
			{
				doSelect( currentMenuEntry - 1);
			}
			else
			{
				if( currentMenuEntry == 1)
				{
					doAdd();
				}
				else
				{
					doSelect( currentMenuEntry - 2);
				}
			}

			ReDisplay();
		}
		break;

		case DMR_LongPress :
		{
			if( currentMenuEntry != 0)
			{
				if( sourceType == Signal_Source_Source::T_Empty)
				{
					doDelete( currentMenuEntry - 1);
				}
				else
				{
					if( currentMenuEntry != 1)
					{
						doDelete( currentMenuEntry - 2);
					}
				}
			}

			ReDisplay();
		}
		break;

		case DMR_Changed :
		{
			doChanged();
		}
		break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Sources::displayMarker( void)
{
	uint8_t MarkerLine = currentMenuEntry;

	if( MarkerLine > 0)
	{
		// Add one for the gap after exit.
		MarkerLine++;
	}

	if(( sourceType != Signal_Source_Source::T_Empty) && ( MarkerLine > 2))
	{
		// Add another one for the gap after Add for source screen with a concrete type.
		MarkerLine++;
	}

	menuMarker.Display( markerLeft, markerTop + ( MarkerLine * SCREEN_SETUP_BASE_LINE_HEIGHT));
}

void Screen_Setup_Sources::doChanged( void)
{
	uint8_t StartLine;

	// Set start of sources and end of sources.
	if( sourceType == Signal_Source_Source::T_Empty)
	{
		StartLine = 1;
	}
	else
	{
		StartLine = 2;
	}

	if( Screen_Setup_BaseList::doChanged( countSources(), StartLine))
	{
		reDisplay();
	}

	displayMarker();
}

void Screen_Setup_Sources::doAdd( void)
{
	if( sourceType == Signal_Source_Source::T_Empty)
	{
		return;
	}

	// Find free source in setup.
	uint16_t SetupSourceId = 0;
	uint8_t SignalSourceId = 0;

	bool SetupSourceAvailable = GLOBAL.SetupService.FindNextEmptySource( &SetupSourceId);
	bool SignalSourceAvailable = GLOBAL.SignalProcessor.FindNextEmptySource( &SignalSourceId);

	if(( SetupSourceAvailable == false) || ( SignalSourceAvailable == false))
	{
		GUI_Setup_Popup Popup;

		// Set text.
		if( SetupSourceAvailable == false)
		{
			Popup.SetText_P( Text::NoSystemStorage);
		}
		else
		{
			Popup.SetText_P( Text::NoModelStorage);
		}

		Popup.SetOKText_P( Text::Ok);

		Popup.Show();
		Popup.Ask();

		return;
	}

	Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

	uint8_t SelectedModelId = GLOBAL.SetupService.GetSelectedModelId();

	uint8_t ModelId;

	if( sourceLevel == Signal_Source_Source::L_Global)
	{
		ModelId = SETUP_MODEL_GLOBAL;
	}
	else if( sourceLevel == Signal_Source_Source::L_Type)
	{
		ModelId = GLOBAL.SetupService.GetSelectedTypeId( SelectedModelId);
	}
	else
	{
		ModelId = SelectedModelId;
	}

	GLOBAL.SetupService.ResetSource( Source, ModelId, SetupSourceId, sourceType);

	doSelectSource( SignalSourceId);
}

void Screen_Setup_Sources::doSelect( uint8_t LineId)
{
	// Just to play safe...
	if( LineId >= visibleLines)
	{
		return;
	}

	uint8_t SignalSourceId = source[ LineId];

	if( SignalSourceId >= SIGNAL_SOURCES)
	{
		// Not valid.
		return;
	}

	doSelectSource( SignalSourceId);
}

void Screen_Setup_Sources::doSelectSource( uint8_t SignalSourceId)
{
	const Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

	if( Source == NULL)
    {
        return;
    }

	switch( Source->GetType())
	{
		case Signal_Source_Source::T_Input :		doInput( SignalSourceId);		break;
		case Signal_Source_Source::T_Map :			doMap( SignalSourceId);			break;
		case Signal_Source_Source::T_Mix :			doMix( SignalSourceId);			break;
		case Signal_Source_Source::T_Store :		doStore( SignalSourceId);		break;
		case Signal_Source_Source::T_Timer :		doTimer( SignalSourceId);		break;
		case Signal_Source_Source::T_Follower :		doFollower( SignalSourceId);	break;
		case Signal_Source_Source::T_Trimmer :		doTrimmer( SignalSourceId);		break;

		default : break;
	}
}

void Screen_Setup_Sources::doDelete( uint8_t LineId)
{
	// Just to play safe...
	if( LineId >= visibleLines)
	{
		return;
	}

	uint8_t SignalSourceId = source[ LineId];

	if( SignalSourceId >= SIGNAL_SOURCES)
	{
		// Not valid.
		return;
	}

	GUI_Setup_Popup Popup;

	// Set text.
	const flash_char* Format;

	Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

	switch( Source->GetType())
	{
		case Signal_Source_Source::T_Input : 	Format = Text::DeleteSourceInputFormat;		break;
		case Signal_Source_Source::T_Map :		Format = Text::DeleteSourceMapFormat;		break;
		case Signal_Source_Source::T_Mix :		Format = Text::DeleteSourceMixFormat;		break;
		case Signal_Source_Source::T_Store :	Format = Text::DeleteSourceStoreFormat;		break;
		case Signal_Source_Source::T_Timer :	Format = Text::DeleteSourceTimerFormat;		break;
		case Signal_Source_Source::T_Follower :	Format = Text::DeleteSourceFollowerFormat;	break;
		case Signal_Source_Source::T_Trimmer :	Format = Text::DeleteSourceTrimmerFormat;	break;

		default : Format = Text::DeleteSourceUnknownFormat;	break;
	}

	char Text[ 30];
	snprintf_P( Text, sizeof( Text), Format, sourceName[ LineId]);

	Popup.SetText( Text);
	Popup.SetOKText_P( Text::Delete);
	Popup.SetCancelText_P( Text::Cancel);

	Popup.Show();

	if( Popup.Ask() != true)
	{
		return;
	}

	Popup.Show();

	// Remove from processor and setup.
	GLOBAL.SetupService.SetSourceType( Source->GetSetupSourceId(), Signal_Source_Source::T_Empty);
	Source->SetType( Signal_Source_Source::T_Empty);

	// Adjust display
	uint8_t StartLine;

	// Set start of sources and end of sources.
	if( sourceType == Signal_Source_Source::T_Empty)
	{
		StartLine = 1;
	}
	else
	{
		StartLine = 2;
	}

	adjustAfterDelete( countSources(), StartLine);
}

void Screen_Setup_Sources::doInput( uint8_t SignalSourceId)
{
	Screen_Setup_Source_Input MapInput( SignalSourceId);
	MapInput.Run();
}

void Screen_Setup_Sources::doMap( uint8_t SignalSourceId)
{
	Screen_Setup_Source_Map MapScreen( SignalSourceId);
	MapScreen.Run();
}

void Screen_Setup_Sources::doMix( uint8_t SignalSourceId)
{
	Screen_Setup_Source_Mix MixScreen( SignalSourceId);
	MixScreen.Run();
}

void Screen_Setup_Sources::doStore( uint8_t SignalSourceId)
{
	Screen_Setup_Source_Store StoreScreen( SignalSourceId);
	StoreScreen.Run();
}

void Screen_Setup_Sources::doTimer( uint8_t SignalSourceId)
{
	Screen_Setup_Source_Timer TimerScreen( SignalSourceId);
	TimerScreen.Run();
}

void Screen_Setup_Sources::doFollower( uint8_t SignalSourceId)
{
	Screen_Setup_Source_Follower FollowerScreen( SignalSourceId);
	FollowerScreen.Run();
}

void Screen_Setup_Sources::doTrimmer( uint8_t SignalSourceId)
{
	Screen_Setup_Source_Trimmer TrimmerScreen( SignalSourceId);
	TrimmerScreen.Run();
}

uint8_t Screen_Setup_Sources::countSources( void)
{
	// Find matching sources.
	uint8_t SourcesMatched = 0;

	// Loop sources.
	for( uint8_t SignalSourceId = 0; SignalSourceId < SIGNAL_SOURCES; SignalSourceId++)
	{
		const Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

		Signal_Source_Source::Type CurrentSourceType = Source->GetType();

		// Ignore empty sources.
		if( CurrentSourceType == Signal_Source_Source::T_Empty)
		{
			continue;
		}

		// Ignore proxies.
		if( CurrentSourceType == Signal_Source_Source::T_Proxy)
		{
			continue;
		}

		// Only sources on the same level.
		if( Source->GetLevel() != sourceLevel)
		{
			continue;
		}

		if(( sourceType == Signal_Source_Source::T_All) || ( CurrentSourceType == sourceType))
		{
			SourcesMatched++;
		}
	}

	return( SourcesMatched);
}
