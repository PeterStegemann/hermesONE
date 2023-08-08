// Copyright 2007 Peter Stegemann

#pragma once

#include "Base.h"
#include "Channels.h"
#include "Info.h"
#include "Model.h"
#include "Models.h"
#include "SetupStatus.h"
#include "System.h"
#include "Types.h"

class Screen_Setup_Main : public Screen_Setup_Base
{
  private:
  	virtual void display( void)
    {
    	// Adjust menu entries to frame and set them up.
	    const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

    	uint8_t Line = 0;

    	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop, SCREEN_SETUP_BASE_MAIN_FONT,
	    							 LCD_65K_RGB::C_WarmYellow, LCD_65K_RGB::C_Black,
		    						 LCD::PO_Proportional, Text::Exit);

    	Line += 2;

    	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
	    							 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
		    						 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Models);

    	Line++;

    	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
	    							 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_WarmYellow,
		    						 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Setup);

    	uint16_t SubMenuLeft = menuLeft + Font->CellWidth;

    	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
	    							 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
		    						 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Channels);
    	Line++;

    	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
	    							 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
		    						 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Model);

    	char ModelName[ SETUP_MODEL_NAME_SIZE + 1];

    	uint8_t SelectedModelId = GLOBAL.SetupService.GetSelectedModelId();

    	GLOBAL.SetupService.GetModelName( SelectedModelId, ModelName, sizeof( ModelName));

    	GLOBAL.SetupDisplay.Print( SubMenuLeft + ( 15 * Font->CellWidth),
	    						   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
		    					   SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
			    				   LCD_65K_RGB::C_Black, LCD::PO_Proportional, ModelName);

    	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
	    							 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
		    						 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Type);

    	GLOBAL.SetupService.GetTypeName( GLOBAL.SetupService.GetSelectedTypeId( SelectedModelId),
	    								 ModelName, sizeof( ModelName));

    	GLOBAL.SetupDisplay.Print( SubMenuLeft + ( 15 * Font->CellWidth),
	    						   frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
		    					   SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
			    				   LCD_65K_RGB::C_Black, LCD::PO_Proportional, ModelName);

    	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
	    							 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
		    						 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Global);

    	Line++;

    	GLOBAL.SetupDisplay.Print_P( SubMenuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
	    							 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
		    						 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Status);

    	Line += 2;

    	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
	    							 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
		    						 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Info);

    	GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT),
	    							 SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
		    						 LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::System);
    }

    virtual bool processMenu( DoMenuResult Result)
    {
    	switch( Result)
	    {
    		case DMR_Selected :
	    	{
		    	switch( currentMenuEntry)
			    {
				    case  0 : return( false);

    				case  2 : doModels();			break;

    				case  5 : doSetupChannels();	break;

	    			case  7 : doSetupModel();		break;
		    		case  8 : doSetupType();		break;
			    	case  9 : doSetupGlobal();		break;

    				case 11 : doSetupStatus();		break;

    				case 14 : doInfo();				break;
	    			case 15 : doSystem();			break;

    				default : break;
	    		}

    			ReDisplay();
	    	}
		    break;

    		default : break;
	    }

    	return( true);
    }

    void doModels( void)
    {
        Screen_Setup_Types TypesScreen;
        TypesScreen.Run();
    }

    void doSetupChannels( void)
    {
        Screen_Setup_Channels ChannelsScreen;
        ChannelsScreen.Run();
    }

    void doSetupModel( void)
    {
        Screen_Setup_Model SetupModelScreen( Signal_Source_Source::L_Model);
        SetupModelScreen.Run();
    }

    void doSetupType( void)
    {
        Screen_Setup_Model SetupTypeScreen( Signal_Source_Source::L_Type);
        SetupTypeScreen.Run();
    }

    void doSetupGlobal( void)
    {
        Screen_Setup_Model SetupGlobalScreen( Signal_Source_Source::L_Global);
        SetupGlobalScreen.Run();
    }

    void doSetupStatus( void)
    {
        Screen_Setup_SetupStatus SetupStatusScreen;
        SetupStatusScreen.Run();
    }

    void doInfo( void)
    {
        Screen_Setup_Info InfoScreen;
        InfoScreen.Run();
    }

    void doSystem( void)
    {
        Screen_Setup_System SystemScreen;
        SystemScreen.Run();
    }

  public:
    Screen_Setup_Main( void)
        : Screen_Setup_Base( 0b1100101110100101, Text::Main)
    {
    }
};
