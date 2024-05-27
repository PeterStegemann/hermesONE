// Copyright 2007 Peter Stegemann

#pragma once

class Screen_Setup_Intro
{
  public:
    Screen_Setup_Intro( void)
    {
    }

	// Run screen.
	void Run( void)
    {
    	FONT::FontId UseFont = FONT::FID_Large;

    	GLOBAL.SetupDisplay.Clear( LCD_65K_RGB::Black);

    	const FONT_Type* Font = FONT::GetFont( UseFont);

    	uint8_t TextLeft = ( GLOBAL.SetupDisplay.GetWidth() - strlen_P( Text::hermesONE) * Font->getCellWidth()) / 2;
    	uint8_t TextTop = ( GLOBAL.SetupDisplay.GetHeight() - Font->getCellHeight()) / 2;

    	// Flash in screen.
	    GLOBAL.SetupDisplay.Clear( LCD_65K_RGB::White);

    	// Fade in text.
	    for( uint8_t ColorPart = 0; ColorPart <= LCD_65K_RGB_MAXIMUM_GREY; ColorPart++)
	    {
    		GLOBAL.SetupDisplay.Print_P( TextLeft, TextTop, UseFont,
	        							 LCD_65K_RGB_GREY( LCD_65K_RGB_MAXIMUM_GREY - ColorPart),
			        					 LCD_65K_RGB::White, LCD::PO_Proportional, Text::hermesONE);

    		// 25 fps.
	    	UTILITY::Pause( 1000 / 25);
	    }

    	UTILITY::Pause( 750);

    	// Fade out text.
	    for( uint8_t ColorPart = 0; ColorPart <= LCD_65K_RGB_MAXIMUM_GREY; ColorPart++)
	    {
    		GLOBAL.SetupDisplay.Print_P( TextLeft, TextTop, UseFont, LCD_65K_RGB_GREY( ColorPart),
	        							 LCD_65K_RGB::White, LCD::PO_Proportional, Text::hermesONE);

    		// 15 fps.
	    	UTILITY::Pause( 1000 / 25);
	    }

    	// Fade out screen.
	    for( uint8_t ColorPart = 0; ColorPart <= LCD_65K_RGB_MAXIMUM_GREY; ColorPart += 3)
	    {
    		GLOBAL.SetupDisplay.Clear( LCD_65K_RGB_GREY( LCD_65K_RGB_MAXIMUM_GREY - ColorPart));

    		// 25 fps.
//	    	UTILITY::Pause( 1000 / 25);
	    }

    	GLOBAL.SetupDisplay.Clear( LCD_65K_RGB::Black);

    	UTILITY::Pause( 250);
    }
/*
		Orientation = Landscape;
		LCD_S65_Cls( white);

		int Characters = ( LCD_S65_WindowWidth() / LCD_S65_GetFontInfo( UseFont)->CellWidth) + 1;

		int ScrollTextStart = 0;
		int CharacterPixel = 0;

		char CurrentText[ 80] = "";
  	strncpy( CurrentText, ScrollText, 28);
  	CurrentText[ Characters] = ' ';

		while( ScrollTextStart < strlen( ScrollText))
		{
			if( CharacterPixel == 0)
			{
				CharacterPixel = LCD_S65_GetFontInfo( UseFont)->CellWidth;

				strncpy( CurrentText, ScrollText + ScrollTextStart, Characters);
				CurrentText[ Characters + 1] = 0;

				ScrollTextStart++;
			}

			LCD_S65_Print_P( CurrentText, CharacterPixel - LCD_S65_GetFontInfo( UseFont)->CellWidth,
			           LCD_S65_WindowHeight() >> 1, UseFont, 1, 1, black, white);

			CharacterPixel--;

			delay_ms( 20);
		}
	}
*/
};
