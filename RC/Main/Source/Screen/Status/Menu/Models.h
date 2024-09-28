// Copyright 2010 Peter Stegemann

#pragma once

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
	Screen_Status_Menu_Models( Input_Service* InputService, uint8_t SetupTypeId);
	virtual ~Screen_Status_Menu_Models( void);
};
