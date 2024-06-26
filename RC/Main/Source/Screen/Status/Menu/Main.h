// Copyright 2010 Peter Stegemann

#pragma once

#include "Base.h"
#include "Calibrations.h"
#include "Channels.h"
#include "PPM.h"
#include "Status.h"
#include "System.h"
#include "Types.h"

class Screen_Status_Menu_Main : public Screen_Status_Menu_Base
{
  private:
    enum MenuEntry
    {
        MENU_EXIT,
        MENU_MODELS,
        MENU_CHANNELS,
        MENU_STATUS,
        MENU_SYSTEM,
        MENU_PPM_0,
        MENU_PPM_1,
        MENU_CALIBRATIONS,
        MENU_COUNT
    };

    // Return false if the screen should exit.
    virtual bool processMenu( DoMenuResult Result)
    {
        switch( Result)
        {
            case DMR_Changed :
            {
                switch( currentMenuEntry)
                {
                    case MENU_EXIT :			SetText_P( Text::Exit);			break;
                    case MENU_MODELS :			SetText_P( Text::Models);		break;
                    case MENU_CHANNELS :		SetText_P( Text::Channels);		break;
                    case MENU_STATUS :			SetText_P( Text::Status);		break;
                    case MENU_SYSTEM :			SetText_P( Text::System);		break;
                    case MENU_PPM_0 :			SetText_P( Text::PPM);			break;
                    case MENU_PPM_1 :			SetText_P( Text::PPM);			break;
                    case MENU_CALIBRATIONS :	SetText_P( Text::Calibration);	break;
                }
            }
            break;

            case DMR_Selected :
            {
                switch( currentMenuEntry)
                {
                    case MENU_EXIT : return( false);

                    case MENU_MODELS :
                    {
                        Screen_Status_Menu_Types TypesScreen;
                        TypesScreen.Run();
                    }
                    break;

                    case MENU_CHANNELS :
                    {
                        Screen_Status_Menu_Channels ChannelsScreen;
                        ChannelsScreen.Run();
                    }
                    break;

                    case MENU_STATUS :
                    {
                        Screen_Status_Menu_Status StatusScreen;
                        StatusScreen.Run();
                    }
                    break;

                    case MENU_SYSTEM :
                    {
                        Screen_Status_Menu_System SystemScreen;
                        SystemScreen.Run();
                    }
                    break;

                    case MENU_PPM_0 :
                    {
                        Screen_Status_Menu_PPM PPMScreen( 0);
                        PPMScreen.Run();
                    }
                    break;

                    case MENU_PPM_1 :
                    {
                        Screen_Status_Menu_PPM PPMScreen( 1);
                        PPMScreen.Run();
                    }
                    break;

                    case MENU_CALIBRATIONS :
                    {
                        Screen_Status_Menu_Calibrations CalibrationsScreen;
                        CalibrationsScreen.Run();
                    }
                    break;
                }

                clearSubMenuLine();
            }
            break;

            default : break;
        }

        return( true);
    }

  protected:
    // Initial display.
    virtual void display( void)
    {
    	GLOBAL.StatusDisplay.Clear();

    	pickMenu( MENU_MODELS);
    }

  public:
    Screen_Status_Menu_Main( void)
        : Screen_Status_Menu_Base( MENU_COUNT, L_One)
    {
    }

    virtual ~Screen_Status_Menu_Main( void)
    {
    }
};
