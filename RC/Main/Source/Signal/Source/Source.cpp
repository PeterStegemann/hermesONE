// Copyright 2008 Peter Stegemann

#include "Source.h"

#include "Main/Global.h"
#include "Setup/Defines.h"
#include "Signal/Defines.h"

void Signal_Source_Source::SetType( Type SourceType)
{
	sourceType = SourceType;
}

Signal_Source_Source::Type Signal_Source_Source::GetType( void) const
{
	return( sourceType);
}

Signal_Source_Source::Level Signal_Source_Source::GetLevel( void) const
{
	if( modelId < SETUP_MODELS)
	{
		return( Signal_Source_Source::L_Model);
	}
	else if(( modelId >= SETUP_MODEL_TYPES_START) &&
			( modelId < ( SETUP_MODEL_TYPES_START + SETUP_MODEL_TYPES)))
	{
		return( Signal_Source_Source::L_Type);
	}
	else if( modelId == SETUP_MODEL_GLOBAL)
	{
		return( Signal_Source_Source::L_Global);
	}

	return( Signal_Source_Source::L_Model);
}

uint8_t Signal_Source_Source::GetModelId( void) const
{
	return( modelId);
}

uint16_t Signal_Source_Source::GetSetupSourceId( void) const
{
	return( setupSourceId);
}

void Signal_Source_Source::Initialize
(
    Input_Service* InputService, Status_Service* StatusService, Status_Time* StatusTime
)
{
	cachedValue = SIGNAL_NEUTRAL_VALUE;
	valueCached = false;
	recursion = false;

	switch( sourceType)
	{
		case Signal_Source_Source::T_Follower :	Body.Follower.Initialize();	break;

		case Signal_Source_Source::T_Input :
        {
            Body.Input.Initialize( InputService, StatusService);
        }
        break;

		case Signal_Source_Source::T_Map :		Body.Map.Initialize();		break;
		case Signal_Source_Source::T_Mix :		Body.Mix.Initialize();		break;
		case Signal_Source_Source::T_Proxy :	Body.Proxy.Initialize();	break;
		case Signal_Source_Source::T_Store :	Body.Store.Initialize();	break;

		case Signal_Source_Source::T_Timer :
        {
            Body.Timer.Initialize( StatusService, StatusTime);
        }
        break;

		case Signal_Source_Source::T_Trimmer :	Body.Trimmer.Initialize();	break;

		default : break;
	}
}

void Signal_Source_Source::Reset( Type SourceType, uint16_t SetupSourceId, uint8_t ModelId)
{
	sourceType = SourceType;
	setupSourceId = SetupSourceId;
	modelId = ModelId;

	switch( sourceType)
	{
		case Signal_Source_Source::T_Follower :	Body.Follower.Reset();	break;
		case Signal_Source_Source::T_Input :	Body.Input.Reset();		break;
		case Signal_Source_Source::T_Map :		Body.Map.Reset();		break;
		case Signal_Source_Source::T_Mix :		Body.Mix.Reset();		break;
		case Signal_Source_Source::T_Proxy :	Body.Proxy.Reset();		break;
		case Signal_Source_Source::T_Store :	Body.Store.Reset();		break;
		case Signal_Source_Source::T_Timer :	Body.Timer.Reset();		break;
		case Signal_Source_Source::T_Trimmer :	Body.Trimmer.Reset();	break;

		default : break;
	}
}

bool Signal_Source_Source::LoadSetup( uint16_t SetupSourceId, uint8_t ModelId)
{
	sourceType = GLOBAL.SetupService.GetSourceType( SetupSourceId);
	setupSourceId = SetupSourceId;
	modelId = ModelId;

	switch( sourceType)
	{
		case Signal_Source_Source::T_Follower :	Body.Follower.LoadSetup( setupSourceId);	break;
		case Signal_Source_Source::T_Input :	Body.Input.LoadSetup( setupSourceId);		break;
		case Signal_Source_Source::T_Map :		Body.Map.LoadSetup( setupSourceId);			break;
		case Signal_Source_Source::T_Mix :		Body.Mix.LoadSetup( setupSourceId);			break;
		case Signal_Source_Source::T_Proxy :	Body.Proxy.LoadSetup( setupSourceId);		break;
		case Signal_Source_Source::T_Store :	Body.Store.LoadSetup( setupSourceId);		break;
		case Signal_Source_Source::T_Timer :	Body.Timer.LoadSetup( setupSourceId);		break;
		case Signal_Source_Source::T_Trimmer :	Body.Trimmer.LoadSetup( setupSourceId);		break;

		default : return( false);
	}

	return( true);
}

void Signal_Source_Source::StoreIfModified( void)
{
	switch( sourceType)
	{
		case Signal_Source_Source::T_Input :	Body.Input.StoreIfModified( setupSourceId);	break;
		case Signal_Source_Source::T_Store :	Body.Store.StoreIfModified( setupSourceId);	break;
		case Signal_Source_Source::T_Timer :	Body.Timer.StoreIfModified( setupSourceId);	break;

		default : break;
	}
}

void Signal_Source_Source::FlushCache( void)
{
	valueCached = false;
}

int16_t Signal_Source_Source::calculateValue( Signal_Processor* SignalProcessor)
{
	// Protect us from recursive calls by returning the old value.
	if( recursion == true)
	{
		return( cachedValue);
	}

	// Safe mark our own source access.
	recursion = true;

	int16_t ChannelValue = SIGNAL_NEUTRAL_VALUE;

	// Get the signal value.
	switch( sourceType)
	{
		case Signal_Source_Source::T_Follower :
		{
			ChannelValue = Body.Follower.CalculateValue( SignalProcessor);
		}
		break;

		case Signal_Source_Source::T_Input :
		{
			ChannelValue = Body.Input.CalculateValue( SignalProcessor);
		}
		break;

		case Signal_Source_Source::T_Map :
		{
			ChannelValue = Body.Map.CalculateValue( SignalProcessor);
		}
		break;

		case Signal_Source_Source::T_Mix :
		{
			ChannelValue = Body.Mix.CalculateValue( SignalProcessor);
		}
		break;

		case Signal_Source_Source::T_Proxy :
		{
			ChannelValue = Body.Proxy.CalculateValue( SignalProcessor);
		}
		break;

		case Signal_Source_Source::T_Store :
		{
			ChannelValue = Body.Store.CalculateValue( SignalProcessor);
		}
		break;

		case Signal_Source_Source::T_Timer :
		{
			ChannelValue = Body.Timer.CalculateValue( SignalProcessor);
		}
		break;

		case Signal_Source_Source::T_Trimmer :
		{
			ChannelValue = Body.Trimmer.CalculateValue( SignalProcessor);
		}
		break;

		default : break;
	}

	// Free access on us again.
	recursion = false;

	cachedValue = ChannelValue;
	valueCached = true;

	return( ChannelValue);
}

int16_t Signal_Source_Source::GetValue( Signal_Processor* SignalProcessor)
{
	if( valueCached == true)
	{
		return cachedValue;
	}
	else
	{
		return( calculateValue( SignalProcessor));
	}
}
