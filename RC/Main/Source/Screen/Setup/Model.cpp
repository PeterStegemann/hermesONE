// Copyright 2007 Peter Stegemann

#include "Model.h"

#include "Proxies.h"
#include "Sources.h"
#include "GUI/Setup/Popup.h"
#include "GUI/Setup/Select.h"
#include "Main/Global.h"
#include "Setup/Service.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"

const flash_char* getTitle( Signal_Source_Source::Level SourceLevel)
{
	switch( SourceLevel)
	{
		case Signal_Source_Source::L_Model :	return( Text::Model);
		case Signal_Source_Source::L_Type :		return( Text::Type);
		case Signal_Source_Source::L_Global :	return( Text::Global);
	}

	return( NULL);
}

uint16_t getMenuPattern( Signal_Source_Source::Level SourceLevel)
{
	switch( SourceLevel)
	{
		case Signal_Source_Source::L_Model :	return( 0b1011111110101101);
		case Signal_Source_Source::L_Type :		return( 0b1000011111110101);
		case Signal_Source_Source::L_Global :	return( 0b1000000111111101);
	}

	return( 0);
}

Screen_Setup_Model::Screen_Setup_Model( Signal_Source_Source::Level SourceLevel)
				  : Screen_Setup_Base( getMenuPattern( SourceLevel), getTitle( SourceLevel))
				  , sourceLevel( SourceLevel)
				  , selectedModelId( GLOBAL.SetupService.GetSelectedModelId())
				  , selectedTypeId( GLOBAL.SetupService.GetSelectedTypeId( selectedModelId))
{
	if( sourceLevel == Signal_Source_Source::L_Model)
	{
		GLOBAL.SetupService.GetModelName( selectedModelId, modelName, sizeof( modelName));
	}
	else if( sourceLevel == Signal_Source_Source::L_Type)
	{
		GLOBAL.SetupService.GetTypeName( selectedTypeId, modelName, sizeof( modelName));
	}

	modelNameInput.SetOptions(( GUI_Setup_TextInput::Options)
		( GUI_Setup_TextInput::O_LimitAlphaNumeric));
}

void Screen_Setup_Model::display( void)
{
	// Adjust menu entries to frame and set them up.
	const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

	uint16_t ContentLeft = frameLeft + 10 * Font->CellWidth;

	uint8_t Line = 0;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
								 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
								 LCD::PO_Proportional, Text::Exit);
	Line += 2;

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Name);

	if( sourceLevel != Signal_Source_Source::L_Global)
	{
		modelNameLabel.SetDimensions( ContentLeft,
									  frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
		modelNameInput.SetDimensions( menuLeft,
									  frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));

		modelNameLabel.Clear();
		modelNameLabel.Display( modelName);

		Line++;
	}

	if( sourceLevel == Signal_Source_Source::L_Model)
	{
		Line--;

		GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
									 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
									 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Type);

		typeLabel.SetDimensions( ContentLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
		updateType( &typeLabel, selectedTypeId);

		Line++;

		GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
									 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
									 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Proxies);

		Line++;
	}

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Inputs);
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Maps);
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Mixers);
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Stores);
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Timer);
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Followers);
	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Trimmer);

	if( sourceLevel == Signal_Source_Source::L_Model)
	{
		Line += 1;
	}
	else if( sourceLevel == Signal_Source_Source::L_Type)
	{
		Line += 4;
	}
	else if( sourceLevel == Signal_Source_Source::L_Global)
	{
		Line += 6;
	}

	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
								 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
								 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Sources);
}

bool Screen_Setup_Model::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Selected :
		{
			uint8_t CurrentLine = currentMenuEntry;

			if( CurrentLine == 0)
			{
				return( false);
			}

			if( sourceLevel != Signal_Source_Source::L_Global)
			{
				if( CurrentLine == 2)
				{
					doModelName();

					ReDisplay();

					break;
				}

				CurrentLine -= 2;
			}

			if( sourceLevel == Signal_Source_Source::L_Model)
			{
				if( CurrentLine == 1)
				{
					doType();

					ReDisplay();

					break;
				}
				else if( CurrentLine == 3)
				{
					doProxies();

					ReDisplay();

					break;
				}

				CurrentLine -= 3;
			}

			switch( CurrentLine)
			{
				case 2 : doSources( Signal_Source_Source::T_Input);			break;
				case 3 : doSources( Signal_Source_Source::T_Map);			break;
				case 4 : doSources( Signal_Source_Source::T_Mix);			break;
				case 5 : doSources( Signal_Source_Source::T_Store);			break;
				case 6 : doSources( Signal_Source_Source::T_Timer);			break;
				case 7 : doSources( Signal_Source_Source::T_Follower);		break;
				case 8 : doSources( Signal_Source_Source::T_Trimmer);		break;

				default : doSources( Signal_Source_Source::T_All);			break;
			}

			ReDisplay();
		}
		break;

		case DMR_Changed :	break;

		default : break;
	}

	return( true);
}

void Screen_Setup_Model::doModelName( void)
{
	modelNameLabel.Clear();

	modelNameInput.ReadText( modelName, sizeof( modelName) - 1);

	strncpy( modelName, modelNameInput.GetText(), sizeof( modelName));
	modelName[ sizeof( modelName) - 1] = 0;

	if( sourceLevel == Signal_Source_Source::L_Model)
	{
		GLOBAL.SetupService.SetModelName( selectedModelId, modelName);
	}
	else if( sourceLevel == Signal_Source_Source::L_Type)
	{
		GLOBAL.SetupService.SetTypeName( selectedTypeId, modelName);
	}

	modelNameInput.Clear();
}

void Screen_Setup_Model::doType( void)
{
	if( GUI_Setup_Select::DoSelect(( int8_t*) &selectedTypeId, SETUP_MODEL_TYPES_START,
									SETUP_MODEL_TYPES_END, 1, &menuMarker, &typeLabel, this,
								    &staticUpdate, &updateType))
	{
		doSelectType( selectedTypeId);
	}
}

void Screen_Setup_Model::doSelectType( uint8_t SetupTypeId)
{
	if(( SetupTypeId < SETUP_MODEL_TYPES_START) || ( SetupTypeId >= SETUP_MODEL_TYPES_END))
	{
		// Not valid.
		return;
	}

	GUI_Setup_Popup Popup;

	// Set text.
	Popup.SetText_P( Text::SwitchTypeWarning);
	Popup.SetOKText_P( Text::Ok);
	Popup.SetCancelText_P( Text::Cancel);

	Popup.Show();

	if( Popup.Ask() != true)
	{
		// Revert to old type.
		selectedTypeId = GLOBAL.SetupService.GetSelectedTypeId( selectedModelId);
		updateType( &typeLabel, selectedTypeId);

		return;
	}

	GLOBAL.SetupService.SetSelectedTypeId( selectedModelId, SetupTypeId);

	selectedTypeId = SetupTypeId;

	GLOBAL.SignalProcessor.LoadModel();
}

void Screen_Setup_Model::doProxies( void)
{
	Screen_Setup_Proxies ProxiesScreen;
	ProxiesScreen.Run();
}

void Screen_Setup_Model::doSources( Signal_Source_Source::Type SourceType)
{
	Screen_Setup_Sources SourcesScreen( SourceType, sourceLevel);
	SourcesScreen.Run();
}

void Screen_Setup_Model::updateType( void* Object, GUI_Setup_Label* Label, int8_t Value)
{
	(( Screen_Setup_Model*) Object)->updateType( Label, Value);
}

void Screen_Setup_Model::updateType( GUI_Setup_Label* Label, int8_t Value)
{
	Label->Clear();

	// Skip to non-empty type.
	while( true)
	{
		if( GLOBAL.SetupService.GetTypeState( Value) != Setup_Service::TS_Empty)
		{
			break;
		}

		// Skip to next type.
		Value++;

		if( Value >= SETUP_MODEL_TYPES_END)
		{
			Value = SETUP_MODEL_TYPES_START;
		}

		if( Value == selectedTypeId)
		{
			// Looped over, looks like there is no type.
			return;
		}
	}

	selectedTypeId = Value;

	GLOBAL.SetupService.GetTypeName( selectedTypeId, typeName, sizeof( typeName));
	Label->SetText( typeName);
	Label->Display();
}
