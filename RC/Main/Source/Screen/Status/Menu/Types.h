// Copyright 2014 Peter Stegemann

#pragma once

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
	Screen_Status_Menu_Types( Input_Service* InputService);
	virtual ~Screen_Status_Menu_Types( void);
};
