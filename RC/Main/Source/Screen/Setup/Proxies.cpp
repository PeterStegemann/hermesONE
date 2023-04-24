// Copyright 2013 Peter Stegemann

#include "Proxies.h"

#include "GUI/Setup/Popup.h"
#include "Main/Setup.h"
#include "Source/Proxy.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Proxies::Screen_Setup_Proxies( void)
					: Screen_Setup_BaseList( Text::Proxies)
					, filledSourceLines( 0)
{
	for( uint8_t SourceLine = 0; SourceLine < visibleLines; SourceLine++)
	{
		valueGauge[ SourceLine].SetOptions(
			( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
										 GUI_Setup_Gauge::O_DualPercentage |
										 GUI_Setup_Gauge::O_CenterLine |
										 GUI_Setup_Gauge::O_Marker));
	}
}

void Screen_Setup_Proxies::display( void)
{
	displayMarker();

	// Adjust gauges to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);

	Line += 2;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Add);

	Line += 2;
	
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

void Screen_Setup_Proxies::reDisplay( void)
{
	// Clear all channels.
	for( uint8_t SourceLine = 0; SourceLine < filledSourceLines; SourceLine++)
	{
		valueGauge[ SourceLine].Clear();
	}

	// Find Proxies to display.
	uint8_t SignalSourceId = 0;
	uint8_t ProxiesMatched = 0;
	uint8_t SourceLine = 0;

	while( SourceLine < visibleLines)
	{
		// Loop Proxies.
		if( SignalSourceId < SIGNAL_SOURCES)
		{
			const Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

			if( Source->GetType() == Signal_Source_Source::T_Proxy)
			{
				// Skip all until firstLine is reached.
				if( ProxiesMatched >= firstLine)
				{
					source[ SourceLine] = SignalSourceId;

					SourceLine++;
				}

				ProxiesMatched++;
			}

			// Skip to next source.
			SignalSourceId++;
		}
		else
		{
			break;
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

	reDisplayMarkers( countProxies());
}

void Screen_Setup_Proxies::update( void)
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

bool Screen_Setup_Proxies::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			if( currentMenuEntry == 0)
			{
				return( false);
			}
			else if( currentMenuEntry == 1)
			{
				doAdd();
			}
			else
			{
				doSelect( currentMenuEntry - 2);
			}

			ReDisplay();
		}
		break;

		case DMR_LongPress :
		{
			if( currentMenuEntry != 0)
			{
				if( currentMenuEntry != 1)
				{
					doDelete( currentMenuEntry - 2);
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

void Screen_Setup_Proxies::displayMarker( void)
{
	uint8_t MarkerLine = currentMenuEntry;

	if( MarkerLine > 0)
	{
		// Add one for the gap after exit.
		MarkerLine++;
	}

	if( MarkerLine > 2)
	{
		// Add another one for the gap after Add.
		MarkerLine++;
	}
	
	menuMarker.Display( markerLeft, markerTop + ( MarkerLine * SCREEN_SETUP_BASE_LINE_HEIGHT));
}

void Screen_Setup_Proxies::doChanged( void)
{
	uint8_t StartLine;

	// Set start of Proxies and end of Proxies.
	StartLine = 2;

	if( Screen_Setup_BaseList::doChanged( countProxies(), StartLine))
	{
		reDisplay();
	}

	displayMarker();
}

void Screen_Setup_Proxies::doAdd( void)
{
	// Find free proxy slot in setup.
	uint8_t ProxyId;

	bool ProxyIdAvailable = GLOBAL.SetupService.FindNextUnusedProxyId( &ProxyId);

	// Find free source in setup.
	uint16_t SetupSourceId = 0;
	uint8_t SignalSourceId = 0;

	bool SetupSourceAvailable = GLOBAL.SetupService.FindNextEmptySource( &SetupSourceId);
	bool SignalSourceAvailable = GLOBAL.SignalProcessor.FindNextEmptySource( &SignalSourceId);

	// Check if it worked.
	if(( SetupSourceAvailable == false) || ( SignalSourceAvailable == false) || ( ProxyIdAvailable == false))
	{
		GUI_Setup_Popup Popup;

		// Set text.
		if( ProxyIdAvailable == false)
		{
			Popup.SetText_P( Text::NoProxyId);
		}
		else if( SetupSourceAvailable == false)
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
	uint8_t ModelId = GLOBAL.SetupService.GetSelectedTypeId( SelectedModelId);

	GLOBAL.SetupService.ResetSource( Source, ModelId, SetupSourceId, Signal_Source_Source::T_Proxy);

	Signal_Source_Proxy* SourceProxy = &( Source->Body.Proxy);

	SourceProxy->Setup.Slot = ProxyId;

	GLOBAL.SetupService.SetSource( SetupSourceId, Source);

	doSelectSource( SignalSourceId);
}

void Screen_Setup_Proxies::doSelect( uint8_t LineId)
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

void Screen_Setup_Proxies::doSelectSource( uint8_t SignalSourceId)
{
	Screen_Setup_Source_Proxy ProxyScreen( SignalSourceId);
	ProxyScreen.Run();
}

void Screen_Setup_Proxies::doDelete( uint8_t LineId)
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
	const flash_char* Format = Text::DeleteSourceProxyFormat;

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
	Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

	GLOBAL.SetupService.SetSourceType( Source->GetSetupSourceId(), Signal_Source_Source::T_Empty);
	Source->SetType( Signal_Source_Source::T_Empty);

	// Adjust display
	uint8_t StartLine;

	// Set start of sources and end of sources.
	StartLine = 2;

	adjustAfterDelete( countProxies(), StartLine);
}

uint8_t Screen_Setup_Proxies::countProxies( void)
{
	// Find matching Proxies.
	uint8_t ProxiesMatched = 0;

	// Loop Proxies.
	for( uint8_t SignalSourceId = 0; SignalSourceId < SIGNAL_SOURCES; SignalSourceId++)
	{
		const Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

		if( Source->GetType() == Signal_Source_Source::T_Proxy)
		{
			ProxiesMatched++;
		}
	}

	return( ProxiesMatched);
}
