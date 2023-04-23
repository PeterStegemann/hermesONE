// Copyright 2010 Peter Stegemann

#ifndef SCREEN_STATUS_MENU_MODELS_H
#define SCREEN_STATUS_MENU_MODELS_H

#include "Base.h"

class Screen_Status_Menu_Models : public Screen_Status_Menu_Base
{
  private:
	uint8_t selectedModelId;
	uint8_t setupTypeId;

	// Return false if the screen should exit.
	virtual bool processMenu( DoMenuResult Result);

  protected:
	// Initial display.
	virtual void display( void);

  public:
	Screen_Status_Menu_Models( uint8_t SetupTypeId);
	virtual ~Screen_Status_Menu_Models( void);
};

#endif
