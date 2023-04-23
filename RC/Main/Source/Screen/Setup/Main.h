// Copyright 2007 Peter Stegemann

#ifndef SCREEN_SETUP_MAIN_H
#define SCREEN_SETUP_MAIN_H

#include "Base.h"

class Screen_Setup_Main : public Screen_Setup_Base
{
	private:
		virtual void display( void);
		virtual bool processMenu( DoMenuResult Result);

		void doModels( void);
		void doSetupChannels( void);
		void doSetupModel( void);
		void doSetupType( void);
		void doSetupGlobal( void);
		void doSetupStatus( void);
		void doSystem( void);
		void doInfo( void);

	public:
		Screen_Setup_Main( void);
};

#endif
