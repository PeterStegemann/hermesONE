// Copyright 2008 Peter Stegemann

#include "Types.h"

#include "Models.h"
#include "Main/Global.h"
#include "GUI/Setup/Popup.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Types::Screen_Setup_Types( void)
				  : Screen_Setup_BaseList( Text::Types)
{
	visibleLines = SCREEN_SETUP_BASELIST_MAXIMUM_LINES - 2;

	for( uint8_t MapLine = 0; MapLine < visibleLines; MapLine++)
	{
		typeLabel[ MapLine].SetText( typeName[ MapLine]);
	}
}

void Screen_Setup_Types::display( void)
{
	displayMarker();

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

	for( uint8_t MapLine = 0; MapLine < visibleLines; MapLine++)
	{
		uint16_t LineTop = frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT);

		typeLabel[ MapLine].SetDimensions( menuLeft, LineTop);

		Line++;
	}

	reDisplay();
}

void Screen_Setup_Types::reDisplay( void)
{
	uint8_t SelectedModelId = GLOBAL.SetupService.GetSelectedModelId();
	selectedTypeId = GLOBAL.SetupService.GetSelectedTypeId( SelectedModelId);

	// Find types to display.
	uint8_t SetupTypeId = SETUP_MODEL_TYPES_START;
	uint8_t TypesMatched = 0;

	for( uint8_t MapLine = 0; MapLine < visibleLines;)
	{
		// Loop types.
		if( SetupTypeId < SETUP_MODEL_TYPES_END)
		{
			if( GLOBAL.SetupService.GetTypeState( SetupTypeId) != Setup_Service::TS_Empty)
			{
				// Skip all until firstLine is reached.
				if( TypesMatched >= firstLine)
				{
					type[ MapLine] = SetupTypeId;

					MapLine++;
				}

				TypesMatched++;
			}

			// Skip to next type.
			SetupTypeId++;
		}
		else
		{
			// No types left, fill the rest with none.
			type[ MapLine] = SETUP_MODEL_TYPE_NONE;

			MapLine++;
		}
	}

	// Update labels.
	for( uint8_t MapLine = 0; MapLine < visibleLines; MapLine++)
	{
		if( type[ MapLine] != SETUP_MODEL_TYPE_NONE)
		{
			typeLabel[ MapLine].Clear();
			GLOBAL.SetupService.GetTypeName( type[ MapLine], typeName[ MapLine],
											 SETUP_MODEL_TYPE_NAME_SIZE + 1);

			if( type[ MapLine] == selectedTypeId)
			{
				typeLabel[ MapLine].ForegroundColor = LCD_65K_RGB::C_Red;
			}
			else
			{
				typeLabel[ MapLine].ForegroundColor = LCD_65K_RGB::C_White;
			}

			typeLabel[ MapLine].Display();
		}
	}

	reDisplayMarkers( GLOBAL.SetupService.CountTypes( Setup_Service::CTO_Used));
}

bool Screen_Setup_Types::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			if( currentMenuEntry == 0)
			{
				return( false);
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

void Screen_Setup_Types::displayMarker( void)
{
	uint8_t MarkerLine = currentMenuEntry;

	if( MarkerLine > 0)
	{
		// Add one for the gap after exit.
		MarkerLine++;
	}

	if( MarkerLine > 2)
	{
		// Add another one for the gap after Add for type screen with a concrete type.
		MarkerLine++;
	}

	menuMarker.Display( markerLeft, markerTop + ( MarkerLine * SCREEN_SETUP_BASE_LINE_HEIGHT));
}

void Screen_Setup_Types::doChanged( void)
{
	if( Screen_Setup_BaseList::doChanged(
		GLOBAL.SetupService.CountTypes( Setup_Service::CTO_Used), 2))
	{
		reDisplay();
	}

	displayMarker();
}

void Screen_Setup_Types::doAdd( void)
{
	// Find free type in setup.
	uint8_t SetupTypeId;

	bool SetupTypeAvailable = GLOBAL.SetupService.FindNextEmptyType( &SetupTypeId);

	if( SetupTypeAvailable == false)
	{
		GUI_Setup_Popup Popup;

		// Set text.
		Popup.SetText_P( Text::NoSystemStorage);
		Popup.SetOKText_P( Text::Ok);

		Popup.Show();
		Popup.Ask();

		return;
	}

	GLOBAL.SetupService.ResetType( SetupTypeId, Setup_Service::TS_Used);
}

void Screen_Setup_Types::doSelect( uint8_t LineId)
{
	// Just to play safe...
	if( LineId >= visibleLines)
	{
		return;
	}

	uint8_t SetupTypeId = type[ LineId];

	if(( SetupTypeId < SETUP_MODEL_TYPES_START) || ( SetupTypeId >= SETUP_MODEL_TYPES_END))
	{
		// Not valid.
		return;
	}

	Screen_Setup_Models ModelsScreen( SetupTypeId);
	ModelsScreen.Run();
}

void Screen_Setup_Types::doDelete( uint8_t LineId)
{
	// Just to play safe...
	if( LineId >= visibleLines)
	{
		return;
	}

	uint8_t SetupTypeId = type[ LineId];

	if(( SetupTypeId < SETUP_MODEL_TYPES_START) || ( SetupTypeId >= SETUP_MODEL_TYPES_END))
	{
		// Not valid.
		return;
	}

	GUI_Setup_Popup Popup;

	if( SetupTypeId == selectedTypeId)
	{
		// Can't delete selected type!
		
		// Set text.
		Popup.SetText_P( Text::CantDeleteCurrentType);
		Popup.SetOKText_P( Text::Ok);
		
		Popup.Show();
		Popup.Ask();
		
		return;
	}

	// Set text.
	char Text[ 30];
	snprintf_P( Text, sizeof( Text), Text::DeleteTypeFormat, typeName[ LineId]);

	Popup.SetText( Text);
	Popup.SetOKText_P( Text::Delete);
	Popup.SetCancelText_P( Text::Cancel);

	Popup.Show();

	if( Popup.Ask() != true)
	{
		return;
	}

	GLOBAL.SetupService.ResetType( SetupTypeId, Setup_Service::TS_Empty);

	// Adjust display
	adjustAfterDelete( GLOBAL.SetupService.CountTypes( Setup_Service::CTO_Used), 2);
}
