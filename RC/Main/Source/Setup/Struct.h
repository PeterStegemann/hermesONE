// Copyright 2008 Peter Stegemann

#pragma once

#include "Battery.h"
#include "Calibration.h"
#include "Model.h"
#include "PPM.h"
#include "Type.h"

#include "Source/Source.h"

struct Setup_Struct
{
	char Owner[ SETUP_OWNER_SIZE];

	uint8_t SelectedModelId;

	uint8_t SetupBacklight;
	uint16_t SetupBlankTime;

	uint8_t StatusBacklight;
	uint8_t StatusContrast;
	uint16_t StatusBlankTime;
	uint8_t StatusInverted;

	Setup_Battery Battery;

	Setup_PPM PPM[ SIGNAL_SERVICE_PPMS];
	char PPMName[ SIGNAL_SERVICE_PPMS][ SETUP_PPM_NAME_SIZE];

	Setup_Calibration Calibration[ SIGNAL_PROCESSOR_ANALOG_INPUTS];
	Setup_Type Type[ SETUP_MODEL_TYPES];
};

struct Setup_ExtensionStruct
{
	Setup_Model Model[ SETUP_MODELS];

	Setup_Source_Source Source[ 0];
};

#define SETUP_EXTENSIONSTRUCT_SIZE  ( sizeof( Setup_ExtensionStruct) + sizeof( Setup_Source_Source) * SETUP_SOURCES)
