// Copyright 2024 Peter Stegemann

#pragma once

#if( SYSTEM_MODE == SYSTEM_MODE_SETUP)
    #include "Main/Setup.h"

    extern Main_Setup MainInstance;
#elif( SYSTEM_MODE == SYSTEM_MODE_STATUS)
    #include "Main/Status.h"

    extern Main_Status MainInstance;
#else
    #error "Unknown system mode."
#endif

#define GLOBAL	MainInstance
