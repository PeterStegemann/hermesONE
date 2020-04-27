// Copyright 2009 Peter Stegemann

#ifndef SCREEN_SETUP_INFO_H
#define SCREEN_SETUP_INFO_H

#include "Base.h"
#include "GUI/Setup/CheckBox.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Defines.h"

class Screen_Setup_Info : public Screen_Setup_Base
{
	private:
		GUI_Setup_Label ownerLabel;
		GUI_Setup_TextInput ownerInput;
		char owner[ SETUP_OWNER_SIZE + 1];

		virtual void display( void);
	//		virtual void update( void);
		virtual bool processMenu( DoMenuResult Result);

		void doOwner( void);

	public:
		Screen_Setup_Info( void);
};

#endif
