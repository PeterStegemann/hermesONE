// Copyright 2014 Peter Stegemann

#include "Utility.h"

#include "Service.h"
#include "Main/Global.h"
#include "Text/Text.h"

char* Setup_Utility::GetRFModeName( uint8_t RFMode, char* RFModeName, uint8_t Size)
{
	switch( RFMode)
	{
		default :
		case Setup_Service::RF_None :
		{
			strncpy_P( RFModeName, Text::RFNone, Size);
		}
		break;

		case Setup_Service::RF_Both :
		{
			strncpy_P( RFModeName, Text::RFBoth, Size);
		}
		break;

		case Setup_Service::RF_Module_0 :
		{
			GLOBAL.SetupService.GetPPMName( 0, RFModeName, Size);
		}
		break;

		case Setup_Service::RF_Module_1 :
		{
			GLOBAL.SetupService.GetPPMName( 1, RFModeName, Size);
		}
		break;
	}

	return RFModeName;
}
