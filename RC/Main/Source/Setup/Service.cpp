// Copyright 2007 Peter Stegemann

#include "Service.h"

#include "Ports.h"
#include "Main/Global.h"
#include "Signal/Defines.h"
#include "Signal/Processor.h"
#include "Struct.h"
#include "Text/Text.h"

#include "AVR/Source/EEPROM.h"
#include "AVR/Source/Utility.h"
#include "AVR/Source/LCD/65K_RGB.h"
#include "AVR/Source/LCD/DOG/DOG.h"

#include <stdio.h>
#include <string.h>
#include <avr/eeprom.h>

#define ExtensionEeprom 	( *(( Setup_ExtensionStruct*) NULL))

Setup_Struct Eeprom EEMEM =
{
	"Your name",		// Owner

	0,					// SelectModelId

	50,					// SetupBacklight
	30,					// SetupBlankTime

	50,					// StatusBacklight
	50,					// StatusContrast
	30,					// StatusBlankTime
	0,					// StatusInverted

	{},					// Battery

	{
		{
			0,					// PPMCenter
			0,					// PPMInverted

			{ 0, 1, 2, 3, 4, 5, 6, 7 }
		},
		{
			0,					// PPMCenter
			0,					// PPMInverted

			{ 0, 1, 2, 3, 4, 5, 6, 7 }
		}
	},

	{},					// PPMName
	{},					// Calibration
	{}					// Type

//    0                   // Debug
};

Setup_Service::Setup_Service( void)
{
	avr::EEPROM::Initialize();

	selectedModelId = eeprom_read_byte( &( Eeprom.SelectedModelId));

	if( selectedModelId >= SETUP_MODELS)
	{
		selectedModelId = 0;
	}
}

void Setup_Service::Initialize( void)
{
}

void Setup_Service::writeString( const char* String, void* EepromString, uint8_t EepromSize)
{
	// Measure size of string including termination.
	uint8_t Size = strlen( String) + 1;

	// Check the size.
	if( Size > EepromSize)
	{
		Size = EepromSize;
	}

	// So for the maximum string length, no termination will be written.
	eeprom_write_block( String, EepromString, Size);
}

void Setup_Service::readString( char* String, uint8_t Size, void* EepromString, uint8_t EepromSize)
{
	if( Size == 0)	return;

	// Make room for termination in buffer.
	Size--;

	// Check the size.
	if( Size > EepromSize)
	{
		Size = EepromSize;
	}

	// If the string is shorter than what we read, it will come with a short termination.
	eeprom_read_block( String, EepromString, Size);

	// Add safety termination.
	String[ Size] = 0;
}

void Setup_Service::SetOwner( const char* Owner)
{
	writeString( Owner, &( Eeprom.Owner), SETUP_OWNER_SIZE);
}

char* Setup_Service::GetOwner( char* Owner, uint8_t Size)
{
	readString( Owner, Size, &( Eeprom.Owner), SETUP_OWNER_SIZE);

	return( Owner);
}

void Setup_Service::SetDebug( bool Debug)
{
//	eeprom_write_byte( &( Eeprom.Debug), Debug ? 1 : 0);
}

bool Setup_Service::GetDebug( void)
{
	uint8_t Debug = 0; //eeprom_read_byte( &( Eeprom.Debug));

	return( Debug == 1);
}

void Setup_Service::SetSetupBacklight( uint8_t SetupBacklight)
{
	eeprom_write_byte( &( Eeprom.SetupBacklight), SetupBacklight);
}

uint8_t Setup_Service::GetSetupBacklight( void)
{
	uint8_t SetupBacklight = eeprom_read_byte( &( Eeprom.SetupBacklight));

	if( SetupBacklight > LCD_65K_RGB_BACKLIGHT_FULL)
	{
		SetupBacklight = LCD_65K_RGB_BACKLIGHT_FULL;
	}

	return( SetupBacklight);
}

void Setup_Service::SetSetupBlankTime( uint16_t SetupBlankTime)
{
	eeprom_write_word( &( Eeprom.SetupBlankTime), SetupBlankTime);
}

uint16_t Setup_Service::GetSetupBlankTime( void)
{
	return( eeprom_read_word( &( Eeprom.SetupBlankTime)));
}

void Setup_Service::SetStatusBacklight( uint8_t StatusBacklight)
{
	eeprom_write_byte( &( Eeprom.StatusBacklight), StatusBacklight);
}

uint8_t Setup_Service::GetStatusBacklight( void)
{
	uint8_t StatusBacklight = eeprom_read_byte( &( Eeprom.StatusBacklight));

	if( StatusBacklight > LCD_DOG_BACKLIGHT_FULL)
	{
		StatusBacklight = LCD_DOG_BACKLIGHT_FULL;
	}

	return( StatusBacklight);
}

void Setup_Service::SetStatusContrast( uint8_t StatusContrast)
{
	eeprom_write_byte( &( Eeprom.StatusContrast), StatusContrast);
}

uint8_t Setup_Service::GetStatusContrast( void)
{
	uint8_t StatusContrast = eeprom_read_byte( &( Eeprom.StatusContrast));

	if( StatusContrast > LCD_DOG_CONTRAST_FULL)
	{
		StatusContrast = LCD_DOG_CONTRAST_FULL;
	}

	return( StatusContrast);
}

void Setup_Service::SetStatusBlankTime( uint16_t StatusBlankTime)
{
	eeprom_write_word( &( Eeprom.StatusBlankTime), StatusBlankTime);
}

uint16_t Setup_Service::GetStatusBlankTime( void)
{
	return( eeprom_read_word( &( Eeprom.StatusBlankTime)));
}

void Setup_Service::SetStatusInverted( bool StatusInverted)
{
	eeprom_write_byte( &( Eeprom.StatusInverted), StatusInverted ? 1 : 0);
}

bool Setup_Service::GetStatusInverted( void)
{
	bool StatusInverted = eeprom_read_byte( &( Eeprom.StatusInverted));

	return( StatusInverted == 1);
}

void Setup_Service::SetBattery( const Setup_Battery* Battery)
{
	eeprom_write_block( Battery, &( Eeprom.Battery), sizeof( Setup_Battery));
}

void Setup_Service::GetBattery( Setup_Battery* Battery)
{
	eeprom_read_block( Battery, &( Eeprom.Battery), sizeof( Setup_Battery));
}

void Setup_Service::SetCalibration( uint8_t Index, const Setup_Calibration* Calibration)
{
	if( Index >= SIGNAL_PROCESSOR_ANALOG_INPUTS)
	{
		// That index is not available.
		return;
	}

	eeprom_write_block( Calibration, &( Eeprom.Calibration[ Index]), sizeof( Setup_Calibration));
}

void Setup_Service::GetCalibration( uint8_t Index, Setup_Calibration* Calibration)
{
	if( Index >= SIGNAL_PROCESSOR_ANALOG_INPUTS)
	{
		// That index is not available.
		return;
	}

	eeprom_read_block( Calibration, &( Eeprom.Calibration[ Index]), sizeof( Setup_Calibration));
}

void Setup_Service::SetPPM( uint8_t Index, const Setup_PPM* PPM)
{
	if( Index >= SIGNAL_SERVICE_PPMS)
	{
		// That index is not available.
		return;
	}

	eeprom_write_block( PPM, &( Eeprom.PPM[ Index]), sizeof( Setup_PPM));
}

void Setup_Service::GetPPM( uint8_t Index, Setup_PPM* PPM)
{
	if( Index >= SIGNAL_SERVICE_PPMS)
	{
		// That index is not available.
		return;
	}

	eeprom_read_block( PPM, &( Eeprom.PPM[ Index]), sizeof( Setup_PPM));
}

void Setup_Service::SetPPMName( uint8_t PPMId, const char* PPMName)
{
	if( PPMId >= SIGNAL_SERVICE_PPMS)
	{
		// That index is not available.
		return;
	}

	writeString( PPMName, &( Eeprom.PPMName[ PPMId]), SETUP_PPM_NAME_SIZE);
}

char* Setup_Service::GetPPMName( uint8_t PPMId, char* PPMName, uint8_t Size)
{
	if( PPMId >= SIGNAL_SERVICE_PPMS)
	{
		// That index is not available.
		*PPMName = 0;
	}
	else
	{
		readString( PPMName, Size, &( Eeprom.PPMName[ PPMId]), SETUP_PPM_NAME_SIZE);
	}

	return( PPMName);
}

bool Setup_Service::FindNextEmptyModel( uint8_t* ModelId)
{
	// Loop models.
	for( uint8_t CurrentModelId = 0; CurrentModelId < SETUP_MODELS; CurrentModelId++)
	{
		if( GetModelState( CurrentModelId) == MS_Empty)
		{
			*ModelId = CurrentModelId;
			
			return( true);
		}
	}
	
	return( false);
}

bool Setup_Service::FindNextEmptyType( uint8_t* TypeId)
{
	// Loop types.
	for( uint8_t CurrentTypeId = SETUP_MODEL_TYPES_START; CurrentTypeId < SETUP_MODEL_TYPES_END; CurrentTypeId++)
	{
		if( GetTypeState( CurrentTypeId) == TS_Empty)
		{
			*TypeId = CurrentTypeId;

			return( true);
		}
	}

	return( false);
}

uint8_t Setup_Service::CountModels( CountModelsOptions UseCountModelsOptions)
{
	// Find matching models.
	uint8_t ModelsMatched = 0;

	// Loop models.
	for( uint8_t SetupModelId = 0; SetupModelId < SETUP_MODELS; SetupModelId++)
	{
		ModelState CurrentModelState = GetModelState( SetupModelId);

		if((( UseCountModelsOptions == CMO_Empty) && ( CurrentModelState == Setup_Service::MS_Empty)) ||
		   (( UseCountModelsOptions == CMO_Used) &&	( CurrentModelState != Setup_Service::MS_Empty)))
		{
			ModelsMatched++;
		}
	}

	return( ModelsMatched);
}

uint8_t Setup_Service::CountModels( uint8_t SetupTypeId, CountModelsOptions UseCountModelsOptions)
{
	// Find matching models.
	uint8_t ModelsMatched = 0;

	// Loop models.
	for( uint8_t SetupModelId = 0; SetupModelId < SETUP_MODELS; SetupModelId++)
	{
		uint8_t CurrentModelTypeId = GetSelectedTypeId( SetupModelId);

		if( CurrentModelTypeId == SetupTypeId)
		{
			ModelState CurrentModelState = GetModelState( SetupModelId);

			if((( UseCountModelsOptions == CMO_Empty) && ( CurrentModelState == Setup_Service::MS_Empty)) ||
			   (( UseCountModelsOptions == CMO_Used) && ( CurrentModelState != Setup_Service::MS_Empty)))
			{
				ModelsMatched++;
			}
		}
	}

	return( ModelsMatched);
}

uint8_t Setup_Service::CountTypes( CountTypesOptions UseCountTypesOptions)
{
	// Find matching types.
	uint8_t TypesMatched = 0;

	// Loop types.
	for( uint8_t SetupTypeId = SETUP_MODEL_TYPES_START; SetupTypeId < SETUP_MODEL_TYPES_END; SetupTypeId++)
	{
		TypeState CurrentTypeState = GetTypeState( SetupTypeId);

		if((( UseCountTypesOptions == CTO_Empty) && ( CurrentTypeState == Setup_Service::TS_Empty)) ||
		   (( UseCountTypesOptions == CTO_Used) && ( CurrentTypeState != Setup_Service::TS_Empty)))
		{
			TypesMatched++;
		}
	}

	return( TypesMatched);
}

void Setup_Service::SetTypeName( uint8_t TypeId, const char* TypeName)
{
	if(( TypeId < SETUP_MODEL_TYPES_START) || ( TypeId >= SETUP_MODEL_TYPES_END))
	{
		// That index is not available.
		return;
	}

	TypeId -= SETUP_MODEL_TYPES_START;

	eeprom_write_block( TypeName, Eeprom.Type[ TypeId].Name, SETUP_MODEL_TYPE_NAME_SIZE);
//	avr::EEPROM::WriteString( EEPROM_Address( ExtensionEeprom.Type[ TypeId].Name),
//						SETUP_MODEL_TYPE_NAME_SIZE, TypeName);
}

char* Setup_Service::GetTypeName( uint8_t TypeId, char* TypeName, uint8_t Size)
{
	if(( TypeId < SETUP_MODEL_TYPES_START) || ( TypeId >= SETUP_MODEL_TYPES_END))
	{
		// That index is not available.
		*TypeName = 0;
	}
	else
	{
		TypeId -= SETUP_MODEL_TYPES_START;

		eeprom_read_block( TypeName, &( Eeprom.Type[ TypeId].Name), Size);
//		avr::EEPROM::ReadString( EEPROM_Address( ExtensionEeprom.Type[ TypeId].Name),
//						   SETUP_MODEL_TYPE_NAME_SIZE, TypeName, Size);
	}

	return( TypeName);
}

void Setup_Service::SetModelName( uint8_t ModelId, const char* ModelName)
{
	if( ModelId >= SETUP_MODELS)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteString( EEPROM_Address( ExtensionEeprom.Model[ ModelId].Name), SETUP_MODEL_NAME_SIZE, ModelName);
}

char* Setup_Service::GetModelName( uint8_t ModelId, char* ModelName, uint8_t Size)
{
	if( ModelId >= SETUP_MODELS)
	{
		// That index is not available.
		*ModelName = 0;
	}
	else
	{
		avr::EEPROM::ReadString( EEPROM_Address( ExtensionEeprom.Model[ ModelId].Name), SETUP_MODEL_NAME_SIZE, ModelName, Size);
	}

	return( ModelName);
}

void Setup_Service::SetModelState( uint8_t ModelId, ModelState State)
{
	if( ModelId >= SETUP_MODELS_END)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteByte( EEPROM_Address( ExtensionEeprom.Model[ ModelId].State), State);
}

Setup_Service::ModelState Setup_Service::GetModelState( uint8_t ModelId)
{
	if( ModelId >= SETUP_MODELS_END)
	{
		// That index is not available.
		return( MS_Empty);
	}

	uint8_t Result;

	avr::EEPROM::ReadByte( EEPROM_Address( ExtensionEeprom.Model[ ModelId].State), &Result);

	return(( Setup_Service::ModelState) Result);
}

void Setup_Service::SetTypeState( uint8_t TypeId, TypeState State)
{
	if(( TypeId < SETUP_MODEL_TYPES_START) || ( TypeId >= SETUP_MODEL_TYPES_END))
	{
		// That index is not available.
		return;
	}

	TypeId -= SETUP_MODEL_TYPES_START;

	eeprom_write_byte( &( Eeprom.Type[ TypeId].State), State);
//	avr::EEPROM::WriteByte( EEPROM_Address( ExtensionEeprom.Type[ TypeId].State), State);
}

Setup_Service::TypeState Setup_Service::GetTypeState( uint8_t TypeId)
{
	if(( TypeId < SETUP_MODEL_TYPES_START) || ( TypeId >= SETUP_MODEL_TYPES_END))
	{
		// That index is not available.
		return( TS_Empty);
	}

	TypeId -= SETUP_MODEL_TYPES_START;

	uint8_t Result;

	Result = eeprom_read_byte( &( Eeprom.Type[ TypeId].State));
//	avr::EEPROM::ReadByte( EEPROM_Address( ExtensionEeprom.Type[ TypeId].State), &Result);

	return(( Setup_Service::TypeState) Result);
}

bool Setup_Service::FindNextEmptySource( uint16_t* SetupSourceId)
{
	// Loop sources.
	for( uint16_t CurrentSourceId = 0; CurrentSourceId < SETUP_SOURCES; CurrentSourceId++)
	{
		if( GetSourceType( CurrentSourceId) == Signal_Source_Source::T_Empty)
		{
			*SetupSourceId = CurrentSourceId;

			return( true);
		}
	}

	return( false);
}

uint16_t Setup_Service::CountSources( CountSourcesOptions UseCountSourcesOptions)
{
	// Find matching models.
	uint16_t SourcesMatched = 0;
	
	// Loop models.
	for( uint16_t SetupSourceId = 0; SetupSourceId < SETUP_SOURCES; SetupSourceId++)
	{
		Signal_Source_Source::Type CurrentSourceType = GetSourceType( SetupSourceId);

		if((( UseCountSourcesOptions == CSO_Empty) && ( CurrentSourceType == Signal_Source_Source::T_Empty)) ||
		   (( UseCountSourcesOptions == CSO_Used) && ( CurrentSourceType != Signal_Source_Source::T_Empty)))
		{
			SourcesMatched++;
		}
	}
	
	return( SourcesMatched);
}

void Setup_Service::SetSourceName( uint16_t SetupSourceId, const char* SourceName)
{
	if( SetupSourceId >= SETUP_SOURCES)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteString( EEPROM_Address( ExtensionEeprom.Source[ SetupSourceId].Name),
						SETUP_SOURCE_NAME_SIZE, SourceName);
}

char* Setup_Service::GetSourceName( uint16_t SetupSourceId, char* SourceName, uint8_t Size)
{
	if( SetupSourceId >= SETUP_SOURCES)
	{
		// That index is not available.
		*SourceName = 0;
	}
	else
	{
		avr::EEPROM::ReadString( EEPROM_Address( ExtensionEeprom.Source[ SetupSourceId].Name), SETUP_SOURCE_NAME_SIZE,
		                   SourceName, Size);
	}

	return( SourceName);
}

void Setup_Service::SetSourceType( uint16_t SetupSourceId, Signal_Source_Source::Type SourceType)
{
	if( SetupSourceId >= SETUP_SOURCES)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteByte( EEPROM_Address( ExtensionEeprom.Source[ SetupSourceId].Type), SourceType);
}

Signal_Source_Source::Type Setup_Service::GetSourceType( uint16_t SetupSourceId)
{
	if( SetupSourceId >= SETUP_SOURCES)
	{
		// That index is not available.
		return( Signal_Source_Source::T_Empty);
	}

	uint8_t Result;

	avr::EEPROM::ReadByte( EEPROM_Address( ExtensionEeprom.Source[ SetupSourceId].Type), &Result);

	return(( Signal_Source_Source::Type) Result);
}

void Setup_Service::SetSourceModelId( uint16_t SetupSourceId, uint8_t ModelId)
{
	if( SetupSourceId >= SETUP_SOURCES)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteByte( EEPROM_Address( ExtensionEeprom.Source[ SetupSourceId].ModelId), ModelId);
}

uint8_t Setup_Service::GetSourceModelId( uint16_t SetupSourceId)
{
	if( SetupSourceId >= SETUP_SOURCES)
	{
		// That index is not available.
		return( 0);
	}

	uint8_t Result;

	avr::EEPROM::ReadByte( EEPROM_Address( ExtensionEeprom.Source[ SetupSourceId].ModelId), &Result);

	return( Result);
}

void Setup_Service::SetSource( uint16_t SetupSourceId, const Signal_Source_Source* Source)
{
	if( SetupSourceId >= SETUP_SOURCES)
	{
		// That index is not available.
		return;
	}

	Signal_Source_Source::Type SourceType = Source->GetType();

	SetSourceType( SetupSourceId, SourceType);

	switch( SourceType)
	{
		case Signal_Source_Source::T_Follower :
		{
			SetSourceFollower( SetupSourceId, &( Source->Body.Follower.Setup));
		}
		break;

		case Signal_Source_Source::T_Input :
		{
			SetSourceInput( SetupSourceId, &( Source->Body.Input.Setup));
		}
		break;

		case Signal_Source_Source::T_Map :
		{
			SetSourceMap( SetupSourceId, &( Source->Body.Map.Setup));
		}
		break;

		case Signal_Source_Source::T_Mix :
		{
			SetSourceMix( SetupSourceId, &( Source->Body.Mix.Setup));
		}
		break;

		case Signal_Source_Source::T_Proxy :
		{
			SetSourceProxy( SetupSourceId, &( Source->Body.Proxy.Setup));
		}
		break;

		case Signal_Source_Source::T_Store :
		{
			SetSourceStore( SetupSourceId, &( Source->Body.Store.Setup));
		}
		break;

		case Signal_Source_Source::T_Timer :
		{
			SetSourceTimer( SetupSourceId, &( Source->Body.Timer.Setup));
		}
		break;

		case Signal_Source_Source::T_Trimmer :
		{
			SetSourceTrimmer( SetupSourceId, &( Source->Body.Trimmer.Setup));
		}
		break;

		default: break;
	}
}

void Setup_Service::setSource( uint16_t SetupSourceId, const void* Source, size_t Size)
{
	if( SetupSourceId >= SETUP_SOURCES)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteBytes( EEPROM_Address( ExtensionEeprom.Source[ SetupSourceId].Body), Size, Source);
}

void Setup_Service::getSource( uint16_t SetupSourceId, void* Source, size_t Size)
{
	if( SetupSourceId >= SETUP_SOURCES)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::ReadBytes( EEPROM_Address( ExtensionEeprom.Source[ SetupSourceId].Body), Size, Source);
}

void Setup_Service::SetSourceFollower( uint16_t SetupSourceId, const Setup_Source_Follower* SourceFollower)
{
	setSource( SetupSourceId, SourceFollower, sizeof( Setup_Source_Follower));
}

void Setup_Service::GetSourceFollower( uint16_t SetupSourceId, Setup_Source_Follower* SourceFollower)
{
	getSource( SetupSourceId, SourceFollower, sizeof( Setup_Source_Follower));
}

void Setup_Service::SetSourceInput( uint16_t SetupSourceId, const Setup_Source_Input* SourceInput)
{
	setSource( SetupSourceId, SourceInput, sizeof( Setup_Source_Input));
}

void Setup_Service::GetSourceInput( uint16_t SetupSourceId, Setup_Source_Input* SourceInput)
{
	getSource( SetupSourceId, SourceInput, sizeof( Setup_Source_Input));
}

void Setup_Service::SetSourceMap( uint16_t SetupSourceId, const Setup_Source_Map* SourceMap)
{
	setSource( SetupSourceId, SourceMap, sizeof( Setup_Source_Map));
}

void Setup_Service::GetSourceMap( uint16_t SetupSourceId, Setup_Source_Map* SourceMap)
{
	getSource( SetupSourceId, SourceMap, sizeof( Setup_Source_Map));
}

void Setup_Service::SetSourceMix( uint16_t SetupSourceId, const Setup_Source_Mix* SourceMix)
{
	setSource( SetupSourceId, SourceMix, sizeof( Setup_Source_Mix));
}

void Setup_Service::GetSourceMix( uint16_t SetupSourceId, Setup_Source_Mix* SourceMix)
{
	getSource( SetupSourceId, SourceMix, sizeof( Setup_Source_Mix));
}

void Setup_Service::SetSourceProxy( uint16_t SetupSourceId, const Setup_Source_Proxy* SourceProxy)
{
	setSource( SetupSourceId, SourceProxy, sizeof( Setup_Source_Proxy));
}

void Setup_Service::GetSourceProxy( uint16_t SetupSourceId, Setup_Source_Proxy* SourceProxy)
{
	getSource( SetupSourceId, SourceProxy, sizeof( Setup_Source_Proxy));
}

void Setup_Service::SetSourceStore( uint16_t SetupSourceId, const Setup_Source_Store* SourceStore)
{
	setSource( SetupSourceId, SourceStore, sizeof( Setup_Source_Store));
}

void Setup_Service::GetSourceStore( uint16_t SetupSourceId, Setup_Source_Store* SourceStore)
{
	getSource( SetupSourceId, SourceStore, sizeof( Setup_Source_Store));
}

void Setup_Service::SetSourceTimer( uint16_t SetupSourceId, const Setup_Source_Timer* SourceTimer)
{
	setSource( SetupSourceId, SourceTimer, sizeof( Setup_Source_Timer));
}

void Setup_Service::GetSourceTimer( uint16_t SetupSourceId, Setup_Source_Timer* SourceTimer)
{
	getSource( SetupSourceId, SourceTimer, sizeof( Setup_Source_Timer));
}

void Setup_Service::SetSourceTrimmer( uint16_t SetupSourceId,
									  const Setup_Source_Trimmer* SourceTrimmer)
{
	setSource( SetupSourceId, SourceTrimmer, sizeof( Setup_Source_Trimmer));
}

void Setup_Service::GetSourceTrimmer( uint16_t SetupSourceId, Setup_Source_Trimmer* SourceTrimmer)
{
	getSource( SetupSourceId, SourceTrimmer, sizeof( Setup_Source_Trimmer));
}

void Setup_Service::SetSelectedModelId( uint8_t ModelId)
{
	selectedModelId = ModelId;

	eeprom_write_byte( &( Eeprom.SelectedModelId), ModelId);
}

uint8_t Setup_Service::GetSelectedModelId( void)
{
	return( selectedModelId);
}

void Setup_Service::SetSelectedTypeId( uint8_t ModelId, uint8_t TypeId)
{
	if( ModelId >= SETUP_MODELS)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteByte( EEPROM_Address( ExtensionEeprom.Model[ ModelId].SelectedTypeId), TypeId);
}

uint8_t Setup_Service::GetSelectedTypeId( uint8_t ModelId)
{
	uint8_t TypeId;
	
	avr::EEPROM::ReadByte( EEPROM_Address( ExtensionEeprom.Model[ ModelId].SelectedTypeId), &TypeId);
	
	if(( TypeId < SETUP_MODEL_TYPES_START) || ( TypeId >= ( SETUP_MODEL_TYPES_END)))
	{
		TypeId = SETUP_MODEL_TYPES_START;
	}
	
	return( TypeId);
}

void Setup_Service::SetRFMode( uint8_t ModelId, uint8_t RFMode)
{
	if( ModelId >= SETUP_MODELS)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteByte( EEPROM_Address( ExtensionEeprom.Model[ ModelId].RFMode), RFMode);
}

uint8_t Setup_Service::GetRFMode( uint8_t ModelId)
{
	uint8_t RFMode;

	avr::EEPROM::ReadByte( EEPROM_Address( ExtensionEeprom.Model[ ModelId].RFMode), &RFMode);

	if( RFMode >= RF_ModeCount)
	{
		RFMode = Setup_Service::RF_Both;
	}

	return( RFMode);
}

void Setup_Service::SetChannelName( uint8_t ModelId, uint8_t ChannelId, const char* ChannelName)
{
	if(( ModelId >= SETUP_MODELS) || ( ChannelId >= SIGNAL_PPM_CHANNELS))
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteString( EEPROM_Address( ExtensionEeprom.Model[ ModelId].ChannelName[ ChannelId]),
						SETUP_SOURCE_NAME_SIZE, ChannelName);
}

char* Setup_Service::GetChannelName( uint8_t ModelId, uint8_t ChannelId, char* ChannelName, uint8_t Size)
{
	if(( ModelId >= SETUP_MODELS) || ( ChannelId >= SIGNAL_PPM_CHANNELS))
	{
		// That index is not available.
		*ChannelName = 0;
	}
	else
	{
		avr::EEPROM::ReadString( EEPROM_Address( ExtensionEeprom.Model[ ModelId].ChannelName[ ChannelId]),
						   SETUP_SOURCE_NAME_SIZE, ChannelName, Size);
	}

	return( ChannelName);
}

void Setup_Service::SetChannelName( uint8_t ChannelId, const char* ChannelName)
{
	SetChannelName( selectedModelId, ChannelId, ChannelName);
}

char* Setup_Service::GetChannelName( uint8_t ChannelId, char* ChannelName, uint8_t Size)
{
	return( GetChannelName( selectedModelId, ChannelId, ChannelName, Size));
}

void Setup_Service::SetChannel( uint8_t ModelId, uint8_t ChannelId, const Setup_Channel* Channel)
{
	if(( ModelId >= SETUP_MODELS) || ( ChannelId >= SIGNAL_PPM_CHANNELS))
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteBytes( EEPROM_Address( ExtensionEeprom.Model[ ModelId].Channel[ ChannelId]),
					   sizeof( Setup_Channel), Channel);
}

void Setup_Service::GetChannel( uint8_t ModelId, uint8_t ChannelId, Setup_Channel* Channel)
{
	if(( ModelId >= SETUP_MODELS) || ( ChannelId >= SIGNAL_PPM_CHANNELS))
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::ReadBytes( EEPROM_Address( ExtensionEeprom.Model[ ModelId].Channel[ ChannelId]),
					  sizeof( Setup_Channel), Channel);
}

void Setup_Service::SetChannel( uint8_t ChannelId, const Setup_Channel* Channel)
{
	SetChannel( selectedModelId, ChannelId, Channel);
}

void Setup_Service::GetChannel( uint8_t ChannelId, Setup_Channel* Channel)
{
	GetChannel( selectedModelId, ChannelId, Channel);
}

void Setup_Service::SetStatusTimerId( uint8_t ModelId, uint8_t StatusId, uint16_t SetupSourceId)
{
	if(( ModelId >= SETUP_MODELS) || ( StatusId >= SETUP_MODEL_STATUS_TIMERS))
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteBytes( EEPROM_Address( ExtensionEeprom.Model[ ModelId].StatusTimer[ StatusId]),
					   sizeof( SetupSourceId), &SetupSourceId);
}

uint16_t Setup_Service::GetStatusTimerId( uint8_t ModelId, uint8_t StatusId)
{
	if(( ModelId >= SETUP_MODELS) || ( StatusId >= SETUP_MODEL_STATUS_TIMERS))
	{
		// That index is not available.
		return( SETUP_SOURCE_NONE);
	}

	uint16_t SetupSourceId;

	avr::EEPROM::ReadBytes( EEPROM_Address( ExtensionEeprom.Model[ ModelId].StatusTimer[ StatusId]),
					  sizeof( SetupSourceId), &SetupSourceId);

	return( SetupSourceId);
}

void Setup_Service::SetStatusTimerId( uint8_t StatusId, uint16_t SetupSourceId)
{
	SetStatusTimerId( selectedModelId, StatusId, SetupSourceId);
}

uint16_t Setup_Service::GetStatusTimerId( uint8_t StatusId)
{
	return( GetStatusTimerId( selectedModelId, StatusId));
}

void Setup_Service::SetStatusSourceId( uint8_t ModelId, uint8_t StatusId, uint16_t SetupSourceId)
{
	if(( ModelId >= SETUP_MODELS) || ( StatusId >= SETUP_MODEL_STATUS_SOURCES))
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteBytes( EEPROM_Address( ExtensionEeprom.Model[ ModelId].StatusSource[ StatusId]),
					   sizeof( SetupSourceId), &SetupSourceId);
}

uint16_t Setup_Service::GetStatusSourceId( uint8_t ModelId, uint8_t StatusId)
{
	if(( ModelId >= SETUP_MODELS) || ( StatusId >= SETUP_MODEL_STATUS_SOURCES))
	{
		// That index is not available.
		return( SETUP_SOURCE_NONE);
	}

	uint16_t SetupSourceId;

	avr::EEPROM::ReadBytes( EEPROM_Address( ExtensionEeprom.Model[ ModelId].StatusSource[ StatusId]),
					  sizeof( SetupSourceId), &SetupSourceId);

	return( SetupSourceId);
}

void Setup_Service::SetStatusSourceId( uint8_t StatusId, uint16_t SetupSourceId)
{
	SetStatusSourceId( selectedModelId, StatusId, SetupSourceId);
}

uint16_t Setup_Service::GetStatusSourceId( uint8_t StatusId)
{
	return( GetStatusSourceId( selectedModelId, StatusId));
}

void Setup_Service::SetProxyReference( uint8_t ModelId, uint8_t ProxyId, const Setup_Source_Tuple* ProxyReference)
{
	if( ProxyId >= SETUP_MODEL_PROXY_SOURCES)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::WriteBytes( EEPROM_Address( ExtensionEeprom.Model[ ModelId].ProxyReference[ ProxyId]),
					   sizeof( Setup_Source_Tuple), ProxyReference);
}

void Setup_Service::GetProxyReference( uint8_t ModelId, uint8_t ProxyId, Setup_Source_Tuple* ProxyReference)
{
	if( ProxyId >= SETUP_MODEL_PROXY_SOURCES)
	{
		// That index is not available.
		return;
	}

	avr::EEPROM::ReadBytes
	(
	    EEPROM_Address( ExtensionEeprom.Model[ ModelId].ProxyReference[ ProxyId]),
        sizeof( Setup_Source_Tuple),
        ProxyReference
    );
}

void Setup_Service::SetProxyReference( uint8_t ProxyId, const Setup_Source_Tuple* ProxyReference)
{
	SetProxyReference( selectedModelId, ProxyId, ProxyReference);
}

void Setup_Service::GetProxyReference( uint8_t ProxyId, Setup_Source_Tuple* ProxyReference)
{
	GetProxyReference( selectedModelId, ProxyId, ProxyReference);
}

bool Setup_Service::FindNextUnusedProxyId( uint8_t* ProxyId)
{
	bool AvailableProxies[ SETUP_MODEL_PROXY_SOURCES];

	for( int8_t CurrentProxyId = 0; CurrentProxyId < SETUP_MODEL_PROXY_SOURCES; CurrentProxyId++)
	{
		AvailableProxies[ CurrentProxyId] = true;
	}

	// Loop sources.
	for( uint8_t SignalSourceId = 0; SignalSourceId < SIGNAL_SOURCES; SignalSourceId++)
	{
		const Signal_Source_Source* Source = GLOBAL.SignalProcessor.GetSource( SignalSourceId);

		if( Source->GetType() == Signal_Source_Source::T_Proxy)
		{
			const Signal_Source_Proxy* SourceProxy = &( Source->Body.Proxy);

			uint8_t Slot = SourceProxy->Setup.Slot;

			// Better ignore invalid slots.
			if( Slot < SETUP_MODEL_PROXY_SOURCES)
			{
				AvailableProxies[ Slot] = false;
			}
		}
	}

	for( int8_t CurrentProxyId = 0; CurrentProxyId < SETUP_MODEL_PROXY_SOURCES; CurrentProxyId++)
	{
		if( AvailableProxies[ CurrentProxyId] == true)
		{
			*ProxyId = CurrentProxyId;

			return( true);
		}
	}
	
	return( false);
}

void Setup_Service::Reset( void)
{
	// Clear owner.
	SetOwner( "");

	// Delete all models.
	for( uint8_t CurrentModelId = 1; CurrentModelId < SETUP_MODELS; CurrentModelId++)
	{
		SetModelState( CurrentModelId, Setup_Service::MS_Empty);
	}

	// Delete all types.
	for( uint8_t CurrentTypeId = SETUP_MODEL_TYPES_START + 1; CurrentTypeId < SETUP_MODEL_TYPES_END;
		 CurrentTypeId++)
	{
		SetTypeState( CurrentTypeId, Setup_Service::TS_Empty);
	}

	// Delete all sources.
	for( uint16_t CurrentSourceId = 0; CurrentSourceId < SETUP_SOURCES; CurrentSourceId++)
	{
		SetSourceType( CurrentSourceId, Signal_Source_Source::T_Empty);
	}

	// Set type to used.
	SetTypeState( SETUP_MODEL_TYPES_START, Setup_Service::TS_Used);

	// Clear type name.
	char TypeName[ SETUP_MODEL_TYPE_NAME_SIZE + 1];

	snprintf_P( TypeName, sizeof( TypeName), Text::TypeNameFormat, 1);

	SetTypeName( SETUP_MODEL_TYPES_START, TypeName);
	SetSelectedModelId( SETUP_MODELS_START);
	SetSelectedTypeId( SETUP_MODELS_START, SETUP_MODEL_TYPES_START);

	ResetModel( SETUP_MODELS_START, SETUP_MODEL_TYPES_START, Setup_Service::MS_Used);

	SetSetupBacklight( LCD_65K_RGB_BACKLIGHT_FULL / 2);
	SetSetupBlankTime( 30);
	SetStatusBacklight( LCD_DOG_BACKLIGHT_FULL / 2);
	SetStatusContrast( LCD_DOG_CONTRAST_FULL / 2);
	SetStatusBlankTime( 30);

	Setup_Battery Battery;
	Battery.WarnLowVoltage = 66;
	Battery.WarnCriticalVoltage = 60;
	Battery.MinimumVoltage = 60;
	Battery.MaximumVoltage = 90;
	Battery.CalibrationValue = 150;

	SetBattery( &Battery);

	// Reset all calibrations.
	Setup_Calibration Calibration;

	Calibration.Value[ Setup_Calibration::V_High] = 0;
	Calibration.Value[ Setup_Calibration::V_Center] = 0;
	Calibration.Value[ Setup_Calibration::V_Low] = 0;

	for( uint8_t Index = 0; Index < SIGNAL_PROCESSOR_ANALOG_INPUTS; Index++)
	{
		SetCalibration( Index, &Calibration);
	}

	// Reset all ppms.
	Setup_PPM PPM;

	PPM.Center = 0;
	PPM.Inverted = false;

	// Clear all channel mappings.
	for( uint8_t ChannelId = 0; ChannelId < SIGNAL_PPM_CHANNELS; ChannelId++)
	{
		PPM.ChannelMapping[ ChannelId] = ChannelId;
	}

	for( uint8_t PPMId = 0; PPMId < SIGNAL_SERVICE_PPMS; PPMId++)
	{
		char PPMName[ SETUP_PPM_NAME_SIZE + 1];

		snprintf_P( PPMName, sizeof( PPMName), Text::PPMNameFormat, PPMId + 1);

		SetPPMName( PPMId, PPMName);
		SetPPM( PPMId, &PPM);
	}
}

void Setup_Service::ResetModel( uint8_t ModelId, uint8_t SetupId, ModelState State)
{
	// Set model state.
	SetModelState( ModelId, State);

	SetSelectedTypeId( ModelId, SetupId);

	// Set model rf mode.
	SetRFMode( ModelId, RF_None);

	// Clear model name.
	char ModelName[ SETUP_MODEL_NAME_SIZE + 1];

	snprintf_P( ModelName, sizeof( ModelName), Text::ModelNameFormat, ModelId + 1);

	SetModelName( ModelId, ModelName);

	// Clear channels.
	Signal_Channel Channel;
	Channel.Reset();

	for( uint8_t ChannelId = 0; ChannelId < SIGNAL_PPM_CHANNELS; ChannelId++)
	{
		char ChannelName[ SETUP_CHANNEL_NAME_SIZE + 1];

		snprintf_P( ChannelName, sizeof( ChannelName), Text::ChannelNameFormat, ChannelId + 1);

		SetChannelName( ModelId, ChannelId, ChannelName);
		SetChannel( ModelId, ChannelId, &( Channel.Setup));
	}

	// Clear status timers.
	for( uint8_t TimerId = 0; TimerId < SETUP_MODEL_STATUS_TIMERS; TimerId++)
	{
		SetStatusTimerId( ModelId, TimerId, SETUP_SOURCE_NONE);
	}

	// Clear status sources.
	for( uint8_t StatusId = 0; StatusId < SETUP_MODEL_STATUS_SOURCES; StatusId++)
	{
		SetStatusSourceId( ModelId, StatusId, SETUP_SOURCE_NONE);
	}
	
	// Clear owned sources.
	for( uint16_t CurrentSourceId = 0; CurrentSourceId < SETUP_SOURCES; CurrentSourceId++)
	{
		if(( GetSourceModelId( CurrentSourceId) == ModelId) &&
		   ( GetSourceType( CurrentSourceId) != Signal_Source_Source::T_Empty))
		{
			SetSourceType( CurrentSourceId, Signal_Source_Source::T_Empty);
		}
	}

	// Clear proxy sources.
	Setup_Source_Tuple ProxyReference;
	ProxyReference.Source = SETUP_SOURCE_FIXED;
	ProxyReference.Volume = SIGNAL_MAXIMUM_VALUE;

	for( uint8_t ProxyId = 0; ProxyId < SETUP_MODEL_PROXY_SOURCES; ProxyId++)
	{
		SetProxyReference( ModelId, ProxyId, &ProxyReference);
	}
}

void Setup_Service::ResetType( uint8_t TypeId, TypeState State)
{
	// Set type state.
	SetTypeState( TypeId, State);

	// Clear type name.
	char TypeName[ SETUP_MODEL_NAME_SIZE + 1];

	snprintf_P( TypeName, sizeof( TypeName), Text::TypeNameFormat,
			    TypeId - SETUP_MODEL_TYPES_START + 1);

	SetTypeName( TypeId, TypeName);

	// Clear owned sources.
	for( uint16_t CurrentSourceId = 0; CurrentSourceId < SETUP_SOURCES; CurrentSourceId++)
	{
		if(( GetSourceModelId( CurrentSourceId) == TypeId) &&
		   ( GetSourceType( CurrentSourceId) != Signal_Source_Source::T_Empty))
		{
			SetSourceType( CurrentSourceId, Signal_Source_Source::T_Empty);
		}
	}
}

void Setup_Service::ResetSource( Signal_Source_Source* Source, uint8_t ModelId,
								 uint16_t SetupSourceId, Signal_Source_Source::Type SourceType)
{
	Source->Reset( SourceType, SetupSourceId, ModelId);

	SetSourceModelId( SetupSourceId, ModelId);
	SetSource( SetupSourceId, Source);

	// Clear source name.
	char SourceName[ SETUP_SOURCE_NAME_SIZE + 1];

	switch( SourceType)
	{
		case Signal_Source_Source::T_Follower :
		{
			snprintf_P( SourceName, sizeof( SourceName), Text::FollowerNameFormat, SetupSourceId);
		}
		break;

		case Signal_Source_Source::T_Input :
		{
			snprintf_P( SourceName, sizeof( SourceName), Text::InputNameFormat, SetupSourceId);
		}
		break;

		case Signal_Source_Source::T_Map :
		{
			snprintf_P( SourceName, sizeof( SourceName), Text::MapNameFormat, SetupSourceId);
		}
		break;

		case Signal_Source_Source::T_Mix :
		{
			snprintf_P( SourceName, sizeof( SourceName), Text::MixNameFormat, SetupSourceId);
		}
		break;

		case Signal_Source_Source::T_Proxy :
		{
			snprintf_P( SourceName, sizeof( SourceName), Text::ProxyNameFormat, SetupSourceId);
		}
		break;

		case Signal_Source_Source::T_Store :
		{
			snprintf_P( SourceName, sizeof( SourceName), Text::StoreNameFormat, SetupSourceId);
		}
		break;

		case Signal_Source_Source::T_Timer :
		{
			snprintf_P( SourceName, sizeof( SourceName), Text::TimerNameFormat, SetupSourceId);
		}
		break;

		case Signal_Source_Source::T_Trimmer :
		{
			snprintf_P( SourceName, sizeof( SourceName), Text::TrimmerNameFormat, SetupSourceId);
		}
		break;

		default :
		{
			snprintf_P( SourceName, sizeof( SourceName), Text::Int8Format, SetupSourceId);
		}
		break;
	}

	GLOBAL.SetupService.SetSourceName( SetupSourceId, SourceName);
}
