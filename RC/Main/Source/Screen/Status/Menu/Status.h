// Copyright 2011 Peter Stegemann

#pragma once

#include "Base.h"

#define MENU_STATUS_EXIT            0
#define MENU_STATUS_SOURCE_START    1
#define MENU_STATUS_COUNT           5

class Screen_Status_Menu_Status : public Screen_Status_Menu_Base
{
  private:
    // Return false if the screen should exit.
    virtual bool processMenu( DoMenuResult Result)
    {
        switch( Result)
        {
            case DMR_Changed :
            {
                switch( currentMenuEntry)
                {
                    case MENU_STATUS_EXIT :
                    {
                        SetText_P( Text::Exit);

                        SetText_P(( Level)( menuLevel + 1), NULL);
                    }
                    break;

                    default :
                    {
                        const flash_char* Name[] =
                        {
                            Text::LeftSide, Text::LeftBottom, Text::RightSide, Text::RightBottom
                        };

                        uint8_t SourceStatusIndex = currentMenuEntry - MENU_STATUS_SOURCE_START;

                        SetText_P( Name[ SourceStatusIndex]);

                        uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusSourceId
                        (
                            GLOBAL.SetupService.GetSelectedModelId(), SourceStatusIndex
                        );

                        SetSource(( Level)( menuLevel + 1), SetupSourceId);
                    }
                    break;
                }
            }
            break;

            case DMR_Selected :
            {
                switch( currentMenuEntry)
                {
                    case MENU_STATUS_EXIT : return( false);

                    default :
                    {
                        uint8_t SourceStatusIndex = currentMenuEntry - MENU_STATUS_SOURCE_START;

                        uint16_t SetupSourceId = GLOBAL.SetupService.GetStatusSourceId
                        (
                            GLOBAL.SetupService.GetSelectedModelId(), SourceStatusIndex
                        );

                        uint8_t SignalSourceId = GLOBAL.SignalProcessor.GetSignalSourceId( SetupSourceId);

                        bool SourceChanged = selectSource
                        (
                            &SignalSourceId, &SetupSourceId, this, &updateSource, Signal_Source_Source::L_Model
                        );

                        if( SourceChanged == true)
                        {
                            GLOBAL.SetupService.SetStatusSourceId( SourceStatusIndex, SetupSourceId);
                        }
                    }
                    break;
                }
            }
            break;

            default : break;
        }

        return( true);
    }

    static void updateSource( void* Object, uint16_t SetupSourceId)
    {
    	(( Screen_Status_Menu_Status*) Object)->SetSource
    	(
	    	( Level)((( Screen_Status_Menu_Status*) Object)->menuLevel + 1), SetupSourceId
        );
    }

  protected:
    // Initial display.
    virtual void display( void)
    {
    	pickMenu( MENU_STATUS_SOURCE_START);
    }

  public:
    Screen_Status_Menu_Status( Input_Service* InputService)
        : Screen_Status_Menu_Base( InputService, MENU_STATUS_COUNT, L_Two)
    {
    }

    virtual ~Screen_Status_Menu_Status( void)
    {
    }
};
