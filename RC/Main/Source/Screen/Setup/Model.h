// Copyright 2008 Peter Stegemann

#ifndef SCREEN_SETUP_MODEL_H
#define SCREEN_SETUP_MODEL_H

#include "Base.h"
#include "Sources.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Defines.h"

class Screen_Setup_Model : public Screen_Setup_Base
{
	private:
		Signal_Source_Source::Level sourceLevel;
		uint8_t selectedModelId;
		uint8_t selectedTypeId;

		GUI_Setup_Label modelNameLabel;
		GUI_Setup_TextInput modelNameInput;
		char modelName[ SETUP_MODEL_NAME_SIZE + 1];

		GUI_Setup_Label typeLabel;
		char typeName[ SETUP_MODEL_TYPE_NAME_SIZE + 1];

		virtual void display( void);
		virtual bool processMenu( DoMenuResult Result);

		void doModelName( void);
		void doType( void);
		void doSelectType( uint8_t SetupTypeId);
		void doProxies( void);
		void doSources( Signal_Source_Source::Type SourceType);

		void updateType( GUI_Setup_Label* Label, int8_t Value);

		static void updateType( void* Object, GUI_Setup_Label* Label, int8_t Value);

	public:
		Screen_Setup_Model( Signal_Source_Source::Level
							SourceLevel = Signal_Source_Source::L_Model);
};

#endif
