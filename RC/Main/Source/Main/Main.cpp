// Copyright 2024 Peter Stegemann

#include "Global.h"

#if( SYSTEM_MODE == SYSTEM_MODE_SETUP)
    #include "Main/Setup.h"

    __attribute__((used)) Main_Setup MainInstance;
#elif( SYSTEM_MODE == SYSTEM_MODE_STATUS)
    #include "Main/Status.h"

    __attribute__((used)) Main_Status MainInstance;
#else
    #error "Unknown system mode."
#endif

int main( void)
{
	MainInstance.Run();

	return( 0);
}
