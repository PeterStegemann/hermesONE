// Copyright 2014 Peter Stegemann

#ifndef SCREEN_STATUS_MENU_TYPES_H
#define SCREEN_STATUS_MENU_TYPES_H

#include "Base.h"

class Screen_Status_Menu_Types : public Screen_Status_Menu_Base
{
  private:
	uint8_t selectedModelId;
	uint8_t selectedTypeId;

	// Return false if the screen should exit.
	virtual bool processMenu( DoMenuResult Result);

  protected:
	// Initial display.
	virtual void display( void);

  public:
	Screen_Status_Menu_Types( void);
	virtual ~Screen_Status_Menu_Types( void);
};

#endif
