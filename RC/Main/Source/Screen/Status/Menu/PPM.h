// Copyright 2014 Peter Stegemann

#pragma once

#include "Base.h"
#include "ChannelMapping.h"
#include "Main/Global.h"
#include "Setup/PPM.h"

class Screen_Status_Menu_PPM : public Screen_Status_Menu_Base
{
  private:
    enum MenuEntry
    {
        MENU_EXIT,
        MENU_PPM_INVERTED,
        MENU_PPM_CENTER,
        MENU_CHANNEL_MAPPING,
        MENU_COUNT
    };

    uint8_t ppmId;
    Setup_PPM ppmSetup;

    // Return false if the screen should exit.
    virtual bool processMenu( DoMenuResult Result)
    {
        switch( Result)
        {
            case DMR_Changed :
            {
                switch( currentMenuEntry)
                {
                    case MENU_EXIT :
                    {
                        SetText_P( Text::Exit);

                        SetText_P(( Level)( menuLevel + 1), NULL);
                    }
                    break;

                    case MENU_PPM_INVERTED :
                    {
                        SetText_P( Text::Inverted);

                        updateBoolean( ppmSetup.Inverted);
                    }
                    break;

                    case MENU_PPM_CENTER :
                    {
                        SetText_P( Text::Center);

    //					updateBoolean( PPMInverted);
                    }
                    break;

                    case MENU_CHANNEL_MAPPING :
                    {
                        SetText_P( Text::ChannelMapping);

                        SetText_P(( Level)( menuLevel + 1), NULL);
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

                    case MENU_PPM_INVERTED :
                    {
                        ppmSetup.Inverted = UTILITY::Invert( ppmSetup.Inverted);

                        GLOBAL.SetupService.SetPPM( ppmId, &ppmSetup);
                        GLOBAL.SignalService.GetPPM( ppmId)->SetInverted( ppmSetup.Inverted);

                        updateBoolean( ppmSetup.Inverted);
                    }
                    break;

                    case MENU_PPM_CENTER :
                    {
    //					PPMInverted = UTILITY_Invert( PPMInverted);

    //					GLOBAL.SetupService.SetPPM( ppmId, &ppmSetup);
    //					GLOBAL.SignalService.SetPPMInverted( PPMInverted);

    //					updateBoolean( PPMInverted);
                    }
                    break;

                    case MENU_CHANNEL_MAPPING :
                    {
                        Screen_Status_Menu_ChannelMapping ChannelMappingScreen( ppmId);
                        ChannelMappingScreen.Run();

                        GLOBAL.SetupService.GetPPM( ppmId, &ppmSetup);

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

  protected:
    // Initial display.
    virtual void display( void)
    {
    	pickMenu( MENU_PPM_INVERTED);
    }

  public:
    Screen_Status_Menu_PPM( uint8_t PPMId)
        : Screen_Status_Menu_Base( MENU_COUNT, L_Two)
        , ppmId( PPMId)
    {
    	GLOBAL.SetupService.GetPPM( ppmId, &ppmSetup);
    }

    virtual ~Screen_Status_Menu_PPM( void)
    {
    }
};
