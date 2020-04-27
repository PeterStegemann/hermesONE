// Copyright 2014 Peter Stegemann

#include "Types.h"

#include "Models.h"
#include "Text/Text.h"

#include "AVR/Components/Utility.h"
#include "AVR/Components/Font/Font.h"
#include "AVR/Components/LCD/LCD_DOG.h"

#define MENU_EXIT		0
#define MENU_START		1
#define MENU_COUNT		1

Screen_Status_Menu_Types::Screen_Status_Menu_Types( void)
						: Screen_Status_Menu_Base( GLOBAL.SetupService.CountTypes(
							Setup_Service::CTO_Used) + MENU_COUNT, L_Two)
{
}

Screen_Status_Menu_Types::~Screen_Status_Menu_Types( void)
{
}

void Screen_Status_Menu_Types::display( void)
{
	// Pick selected type.
	selectedModelId = GLOBAL.SetupService.GetSelectedModelId();
	selectedTypeId = GLOBAL.SetupService.GetSelectedTypeId(selectedModelId);

	uint8_t CurrentTypeId = 0;
	uint8_t StartMenuEntry = MENU_START;

	// Loop models.
	while( CurrentTypeId != selectedTypeId)
	{
		if( GLOBAL.SetupService.GetTypeState( CurrentTypeId) != Setup_Service::TS_Empty)
		{
			StartMenuEntry++;
		}

		// Skip to next type.
		CurrentTypeId++;

		if( CurrentTypeId >= SETUP_MODEL_TYPES_END)
		{
			StartMenuEntry = MENU_START;

			break;
		}
	}

	pickMenu( StartMenuEntry);
}

bool Screen_Status_Menu_Types::processMenu( DoMenuResult Result)
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
					selectedTypeId = 0;
					uint8_t TypesMatched = 0;

					// Loop models.
					while( true)
					{
						if( GLOBAL.SetupService.GetTypeState( selectedTypeId) !=
						    Setup_Service::TS_Empty)
						{
							TypesMatched++;

							if( TypesMatched > ( currentMenuEntry - MENU_START))
							{
								break;
							}
						}

						// Skip to next type.
						selectedTypeId++;
					}

					char TypeName[ SETUP_MODEL_TYPE_NAME_SIZE + 1];

					GLOBAL.SetupService.GetTypeName( selectedTypeId, TypeName,
													 SETUP_MODEL_TYPE_NAME_SIZE + 1);

					SetText( TypeName);
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
					Screen_Status_Menu_Models ModelsScreen( selectedTypeId);
					ModelsScreen.Run();

					clearSubMenuLine();
				}
				break;
			}
		}
		break;

		default : break;
	}

	return( true);
}
