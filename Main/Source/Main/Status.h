// Copyright 2010 Peter Stegemann

#ifndef MAIN_STATUS_H
#define MAIN_STATUS_H

#include "Base.h"

class Main_Status : public Main_Base
{
	private:
		uint16_t statusBlankTime;

	private:
		virtual void run( void);

		void doStatusStatus( void);		
		void doStatusMain( void);

	public:
		Main_Status( void);
		virtual ~Main_Status( void);

		virtual void Update( void);

		// Set new status blank time.
		void SetStatusBlankTime( uint16_t StatusBlankTime);
};

extern Main_Status MainInstance;

#endif
