// Copyright 2010 Peter Stegemann

#include "Models.h"

#include "Main/Global.h"
#include "Text/Text.h"

#include "AVR/Source/Utility.h"
#include "AVR/Source/Font/Font.h"
#include "AVR/Source/LCD/DOG/DOG.h"

#define MENU_EXIT		0
#define MENU_START		1
#define MENU_COUNT		1

Screen_Status_Menu_Models::Screen_Status_Menu_Models
(
    Input_Service* InputService, uint8_t SetupTypeId
)
    : Screen_Status_Menu_Base
    (
        InputService,
        GLOBAL.SetupService.CountModels( SetupTypeId, Setup_Service::CMO_Used) + MENU_COUNT,
        L_Three
    )
    , setupTypeId( SetupTypeId)
{
}

Screen_Status_Menu_Models::~Screen_Status_Menu_Models( void)
{
}

void Screen_Status_Menu_Models::display( void)
{
	// Pick selected model.
	selectedModelId = GLOBAL.SetupService.GetSelectedModelId();

	uint8_t StartMenuEntry = MENU_START;

	if( GLOBAL.SetupService.GetSelectedTypeId( selectedModelId) == setupTypeId)
	{
		uint8_t CurrentModelId = 0;

		// Loop models.
		while( CurrentModelId != selectedModelId)
		{
			if(( GLOBAL.SetupService.GetModelState( CurrentModelId) != Setup_Service::MS_Empty) &&
			   ( GLOBAL.SetupService.GetSelectedTypeId( CurrentModelId) == setupTypeId))
			{
				StartMenuEntry++;
			}

			// Skip to next model.
			CurrentModelId++;

			if( CurrentModelId >= SETUP_MODELS_END)
			{
				StartMenuEntry = MENU_START;

				break;
			}
		}
	}

	pickMenu( StartMenuEntry);
}

bool Screen_Status_Menu_Models::processMenu( DoMenuResult Result)
{
	switch( Result)
	{
		case DMR_Changed :
		{
			switch( currentMenuEntry)
			{
				case MENU_EXIT : SetText_P( Text::Exit); break;

				default :
				{
					// Find model to display.
					selectedModelId = 0;
					uint8_t ModelsMatched = 0;

					// Loop models.
					while( true)
					{
						if(( GLOBAL.SetupService.GetModelState( selectedModelId) !=
						     Setup_Service::MS_Empty) &&
						   ( GLOBAL.SetupService.GetSelectedTypeId( selectedModelId) ==
							 setupTypeId))
						{
							ModelsMatched++;

							if( ModelsMatched > ( currentMenuEntry - MENU_START))
							{
								break;
							}
						}

						// Skip to next model.
						selectedModelId++;
					}

					char ModelName[ SETUP_MODEL_NAME_SIZE + 1];

					GLOBAL.SetupService.GetModelName( selectedModelId, ModelName,
													  SETUP_MODEL_NAME_SIZE + 1);

					SetText( ModelName);
				}
				break;
			}
		}
		break;

		case DMR_Selected :
		{
			switch( currentMenuEntry)
			{
				case MENU_EXIT : return( false);

				default :
				{
					GLOBAL.SetupService.SetSelectedModelId( selectedModelId);

					GLOBAL.SignalProcessor.LoadModel();

					return( false);
				}
				break;
			}
		}
		break;

		default : break;
	}

	return( true);
}
