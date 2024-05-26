// Copyright 2008 Peter Stegemann

#include "Models.h"

#include "Main/Global.h"
#include "GUI/Setup/Popup.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

Screen_Setup_Models::Screen_Setup_Models( uint8_t SetupTypeId)
				   : Screen_Setup_BaseList( Text::Models)
				   , setupTypeId( SetupTypeId)
{
	visibleLines = SCREEN_SETUP_BASELIST_MAXIMUM_LINES - 2;

	for( uint8_t MapLine = 0; MapLine < visibleLines; MapLine++)
	{
		modelLabel[ MapLine].SetText( modelName[ MapLine]);
	}
}

void Screen_Setup_Models::display( void)
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

		modelLabel[ MapLine].SetDimensions( menuLeft, LineTop);

		Line++;
	}

	reDisplay();
}

void Screen_Setup_Models::reDisplay( void)
{
	// Find models to display.
	uint8_t SetupModelId = 0;
	uint8_t ModelsMatched = 0;

	for( uint8_t MapLine = 0; MapLine < visibleLines;)
	{
		// Loop models.
		if( SetupModelId < SETUP_MODELS_END)
		{
			if(( GLOBAL.SetupService.GetModelState( SetupModelId) != Setup_Service::MS_Empty) &&
			   ( GLOBAL.SetupService.GetSelectedTypeId( SetupModelId) == setupTypeId))
			{
				// Skip all until firstLine is reached.
				if( ModelsMatched >= firstLine)
				{
					model[ MapLine] = SetupModelId;

					MapLine++;
				}

				ModelsMatched++;
			}

			// Skip to next model.
			SetupModelId++;
		}
		else
		{
			// No models left, fill the rest with none.
			model[ MapLine] = SETUP_MODEL_NONE;

			MapLine++;
		}
	}

	// Update labels.
	uint8_t SelectedModelId = GLOBAL.SetupService.GetSelectedModelId();

	for( uint8_t MapLine = 0; MapLine < visibleLines; MapLine++)
	{
		if( model[ MapLine] != SETUP_MODEL_NONE)
		{
			modelLabel[ MapLine].Clear();
			GLOBAL.SetupService.GetModelName( model[ MapLine], modelName[ MapLine],
											  SETUP_MODEL_NAME_SIZE + 1);

			if( model[ MapLine] == SelectedModelId)
			{
				modelLabel[ MapLine].ForegroundColor = LCD_65K_RGB::C_Red;
			}
			else
			{
				modelLabel[ MapLine].ForegroundColor = LCD_65K_RGB::C_White;
			}

			modelLabel[ MapLine].Display();
		}
	}

	reDisplayMarkers( GLOBAL.SetupService.CountModels( setupTypeId, Setup_Service::CMO_Used));
}

bool Screen_Setup_Models::processMenu( DoMenuResult Result)
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

void Screen_Setup_Models::displayMarker( void)
{
	uint8_t MarkerLine = currentMenuEntry;

	if( MarkerLine > 0)
	{
		// Add one for the gap after exit.
		MarkerLine++;
	}

	if( MarkerLine > 2)
	{
		// Add another one for the gap after Add for model screen with a concrete type.
		MarkerLine++;
	}

	menuMarker.Display( markerLeft, markerTop + ( MarkerLine * SCREEN_SETUP_BASE_LINE_HEIGHT));
}

void Screen_Setup_Models::doChanged( void)
{
	if( Screen_Setup_BaseList::doChanged(
		GLOBAL.SetupService.CountModels( setupTypeId, Setup_Service::CMO_Used), 2))
	{
		reDisplay();
	}

	displayMarker();
}

void Screen_Setup_Models::doAdd( void)
{
	// Find free model in setup.
	uint8_t SetupModelId;

	bool SetupModelAvailable = GLOBAL.SetupService.FindNextEmptyModel( &SetupModelId);

	if( SetupModelAvailable == false)
	{
		GUI_Setup_Popup Popup;

		// Set text.
		Popup.SetText_P( Text::NoSystemStorage);
		Popup.SetOKText_P( Text::Ok);
//		Popup.SetCancelText( NULL);

		Popup.Show();
		Popup.Ask();

		return;
	}

	GLOBAL.SetupService.ResetModel( SetupModelId, setupTypeId, Setup_Service::MS_Used);

	// Select model.
	doSelectModel( SetupModelId);
}

void Screen_Setup_Models::doSelect( uint8_t LineId)
{
	// Just to play safe...
	if( LineId >= visibleLines)
	{
		return;
	}

	uint8_t SetupModelId = model[ LineId];

	if( SetupModelId >= SETUP_MODELS)
	{
		// Not valid.
		return;
	}

	doSelectModel( SetupModelId);
}

void Screen_Setup_Models::doSelectModel( uint8_t SetupModelId)
{
	GLOBAL.SetupService.SetSelectedModelId( SetupModelId);

	GLOBAL.SignalProcessor.LoadModel();

	GLOBAL.StatusScreen.Display();
}

void Screen_Setup_Models::doDelete( uint8_t LineId)
{
	// Just to play safe...
	if( LineId >= visibleLines)
	{
		return;
	}

	uint8_t SetupModelId = model[ LineId];

	if( SetupModelId >= SETUP_MODELS)
	{
		// Not valid.
		return;
	}

	GUI_Setup_Popup Popup;

	if( SetupModelId == GLOBAL.SetupService.GetSelectedModelId())
	{
		// Can't delete selected model!

		// Set text.
		Popup.SetText_P( Text::CantDeleteCurrentModel);
		Popup.SetOKText_P( Text::Ok);
		
		Popup.Show();
		Popup.Ask();

		return;
	}

	// Set text.
	char Text[ 30];
	snprintf_P( Text, sizeof( Text), Text::DeleteModelFormat, modelName[ LineId]);

	Popup.SetText( Text);
	Popup.SetOKText_P( Text::Delete);
	Popup.SetCancelText_P( Text::Cancel);

	Popup.Show();

	if( Popup.Ask() != true)
	{
		return;
	}

	GLOBAL.SetupService.ResetModel( SetupModelId, SETUP_MODEL_TYPES_START, Setup_Service::MS_Empty);

	// Adjust display
	adjustAfterDelete( GLOBAL.SetupService.CountModels( setupTypeId, Setup_Service::CMO_Used), 2);
}
