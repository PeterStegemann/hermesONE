// Copyright 2008 Peter Stegemann

#ifndef SIGNAL_SOURCE_SOURCE_H
#define SIGNAL_SOURCE_SOURCE_H

#include "Follower.h"
#include "Input.h"
#include "Map.h"
#include "Mix.h"
#include "Proxy.h"
#include "Store.h"
#include "Timer.h"
#include "Trimmer.h"

#include "AVR/Components/Types.h"

union Signal_Source_Union
{
	Signal_Source_Follower Follower;
	Signal_Source_Input Input;
	Signal_Source_Map Map;
	Signal_Source_Mix Mix;
	Signal_Source_Proxy Proxy;
	Signal_Source_Store Store;
	Signal_Source_Timer Timer;
	Signal_Source_Trimmer Trimmer;
};

class Signal_Source_Source
{
	public:
		enum Type
		{
			T_Empty,
			T_All = T_Empty,
			T_Input,
			T_Map,
			T_Mix,
			T_Store,
			T_Follower,
			T_Timer,
			T_Trimmer,
			T_Proxy
		};

		enum Level
		{
			L_Model,
			L_Type,
			L_Global
		};
	
	private:
		Type sourceType;
		uint8_t modelId;
		uint16_t setupSourceId;

		// Value caching is used to reduce duplicate calculations. Each calculation round will reset
		// the cache flag first.
		volatile int16_t cachedValue;
		volatile bool valueCached;

		// This is to prevent recursive dead loops.
		bool recursion;

		// Calculate new value.
		int16_t calculateValue( Signal_Processor* SignalProcessor);

	public:
		// The union which "is" the real object.
		Signal_Source_Union Body;

	public:
		void SetType( Type SourceType);
		Type GetType( void) const;

		Level GetLevel( void) const;

//		void SetModelId( uint8_t ModelId);
		uint8_t GetModelId( void) const;

//		void SetSetupSourceId( uint16_t SetupSourceId);
		uint16_t GetSetupSourceId( void) const;

		// Be aware that to connect sources, they all of them have to be loaded by the signal
		// processor.
		void Initialize( void);
		void Reset( Type SourceType, uint16_t SetupSourceId, uint8_t ModelId);

		// Load setup for this source.
		bool LoadSetup( uint16_t SetupSourceId, uint8_t ModelId);

		// Store setup if modified.
		void StoreIfModified( void);

		// Remove cache flag, so next GetValue() call a new value will be calculated.
		void FlushCache( void);

		// Get last calculated value, calulate a new one if needed.
		int16_t GetValue( Signal_Processor* SignalProcessor);
};

#endif
