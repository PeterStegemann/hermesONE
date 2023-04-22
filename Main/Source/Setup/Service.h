// Copyright 2007 Peter Stegemann

#pragma once

#include "Signal/Source/Source.h"

#define EEPROMAddress( Address)	(( uint16_t) &( Address))

class Setup_Battery;
class Setup_Calibration;
class Setup_Channel;
class Setup_PPM;

class Setup_Service
{
  public:
    enum ModelState
    {
        MS_Empty,
        MS_Used
    };

    enum TypeState
    {
        TS_Empty,
        TS_Used
    };

    enum RFMode
    {
        RF_None,
        RF_Module_0,
        RF_Module_1,
        RF_Both,

        RF_ModeCount
    };

  private:
    uint8_t selectedModelId;

    void writeString( const char* String, void* EepromString, uint8_t EepromSize);
    void readString( char* String, uint8_t Size, void* EepromString, uint8_t EepromSize);

    void setSource( uint16_t SetupSourceId, const void* Source, size_t Size);
    void getSource( uint16_t SetupSourceId, void* Source, size_t Size);

  public:
    Setup_Service( void);

    void SetOwner( const char* Owner);
    // Supply the size of the target buffer, including room for termination.
    char* GetOwner( char* Owner, uint8_t Size);

    void SetSetupBacklight( uint8_t StatusBacklight);
    uint8_t GetSetupBacklight( void);

    void SetSetupBlankTime( uint16_t StatusBlankTime);
    uint16_t GetSetupBlankTime( void);

    void SetStatusBacklight( uint8_t StatusBacklight);
    uint8_t GetStatusBacklight( void);

    void SetStatusContrast( uint8_t StatusContrast);
    uint8_t GetStatusContrast( void);

    void SetStatusBlankTime( uint16_t StatusBlankTime);
    uint16_t GetStatusBlankTime( void);

    void SetStatusInverted( bool StatusInverted);
    bool GetStatusInverted( void);

    void SetBattery( const Setup_Battery* Battery);
    void GetBattery( Setup_Battery* Battery);

    void SetCalibration( uint8_t Index, const Setup_Calibration* Calibration);
    void GetCalibration( uint8_t Index, Setup_Calibration* Calibration);

    void SetPPM( uint8_t Index, const Setup_PPM* PPM);
    void GetPPM( uint8_t Index, Setup_PPM* PPM);

    void SetPPMName( uint8_t PPMId, const char* PPMName);
    // Supply the size of the target buffer, including room for termination.
    char* GetPPMName( uint8_t PPMId, char* PPMName, uint8_t Size);

    // Find next free model slot in the setup memory. Returns false if all slots are used.
    bool FindNextEmptyModel( uint8_t* ModelId);
    // Find next free type slot in the setup memory. Returns false if all slots are used.
    bool FindNextEmptyType( uint8_t* TypeId);

    enum CountModelsOptions
    {
        CMO_Empty	= 0b00000000,
        CMO_Used	= 0b00000001
    };

    uint8_t CountModels( CountModelsOptions UseCountModelsOptions = CMO_Empty);
    uint8_t CountModels( uint8_t SetupTypeId,
                         CountModelsOptions UseCountModelsOptions = CMO_Empty);

    enum CountTypesOptions
    {
        CTO_Empty	= 0b00000000,
        CTO_Used	= 0b00000001
    };

    uint8_t CountTypes( CountTypesOptions UseCountTypesOptions = CTO_Empty);

    void SetModelName( uint8_t ModelId, const char* ModelName);
    // Supply the size of the target buffer, including room for termination.
    char* GetModelName( uint8_t ModelId, char* ModelName, uint8_t Size);

    void SetTypeName( uint8_t TypeId, const char* TypeName);
    // Supply the size of the target buffer, including room for termination.
    char* GetTypeName( uint8_t TypeId, char* TypeName, uint8_t Size);

    // State of model.
    void SetModelState( uint8_t ModelId, ModelState State);
    ModelState GetModelState( uint8_t ModelId);

    // State of type.
    void SetTypeState( uint8_t TypeId, TypeState State);
    TypeState GetTypeState( uint8_t TypeId);

    // Find next free source slot in the setup memory. Returns false if all slots are used.
    bool FindNextEmptySource( uint16_t* SetupSourceId);

    enum CountSourcesOptions
    {
        CSO_Empty	= 0b00000000,
        CSO_Used	= 0b00000001
    };

    uint16_t CountSources( CountSourcesOptions UseCountSourcesOptions = CSO_Empty);

    // Name of source.
    void SetSourceName( uint16_t SetupSourceId, const char* SourceName);
    // Supply the size of the target buffer, including room for termination.
    char* GetSourceName( uint16_t SetupSourceId, char* SourceName, uint8_t Size);

    // Type of source.
    void SetSourceType( uint16_t SetupSourceId, Signal_Source_Source::Type SourceType);
    Signal_Source_Source::Type GetSourceType( uint16_t SetupSourceId);

    // Model the source belongs to.
    void SetSourceModelId( uint16_t SetupSourceId, uint8_t ModelId);
    uint8_t GetSourceModelId( uint16_t SetupSourceId);

    void SetSourceFollower( uint16_t SetupSourceId, const Setup_Source_Follower* SourceFollower);
    void GetSourceFollower( uint16_t SetupSourceId, Setup_Source_Follower* SourceFollower);
    void SetSourceInput( uint16_t SetupSourceId, const Setup_Source_Input* SourceInput);
    void GetSourceInput( uint16_t SetupSourceId, Setup_Source_Input* SourceInput);
    void SetSourceMap( uint16_t SetupSourceId, const Setup_Source_Map* SourceMap);
    void GetSourceMap( uint16_t SetupSourceId, Setup_Source_Map* SourceMap);
    void SetSourceMix( uint16_t SetupSourceId, const Setup_Source_Mix* SourceMix);
    void GetSourceMix( uint16_t SetupSourceId, Setup_Source_Mix* SourceMix);
    void SetSourceProxy( uint16_t SetupSourceId, const Setup_Source_Proxy* SourceProxy);
    void GetSourceProxy( uint16_t SetupSourceId, Setup_Source_Proxy* SourceProxy);
    void SetSourceStore( uint16_t SetupSourceId, const Setup_Source_Store* SourceStore);
    void GetSourceStore( uint16_t SetupSourceId, Setup_Source_Store* SourceStore);
    void SetSourceTimer( uint16_t SetupSourceId, const Setup_Source_Timer* SourceTimer);
    void GetSourceTimer( uint16_t SetupSourceId, Setup_Source_Timer* SourceTimer);
    void SetSourceTrimmer( uint16_t SetupSourceId, const Setup_Source_Trimmer* SourceTrimmer);
    void GetSourceTrimmer( uint16_t SetupSourceId, Setup_Source_Trimmer* SourceTrimmer);

    void SetSource( uint16_t SetupSourceId, const Signal_Source_Source* SignalSource);

    void SetSelectedModelId( uint8_t ModelId);
    uint8_t GetSelectedModelId( void);

    void SetSelectedTypeId( uint8_t ModelId, uint8_t TypeId);
    uint8_t GetSelectedTypeId( uint8_t ModelId);

    void SetRFMode( uint8_t ModelId, uint8_t RFMode);
    uint8_t GetRFMode( uint8_t ModelId);

    void SetChannelName( uint8_t ModelId, uint8_t ChannelId, const char* ChannelName);
    // Supply the size of the target buffer, including room for termination.
    char* GetChannelName( uint8_t ModelId, uint8_t ChannelId, char* ChannelName, uint8_t Size);

    // Same as above for selected model id.
    void SetChannelName( uint8_t ChannelId, const char* ChannelName);
    // Supply the size of the target buffer, including room for termination.
    char* GetChannelName( uint8_t ChannelId, char* ChannelName, uint8_t Size);

    void SetChannel( uint8_t ModelId, uint8_t ChannelId, const Setup_Channel* Channel);
    void GetChannel( uint8_t ModelId, uint8_t ChannelId, Setup_Channel* Channel);
    // Same as above for selected model id.
    void SetChannel( uint8_t ChannelId, const Setup_Channel* Channel);
    void GetChannel( uint8_t ChannelId, Setup_Channel* Channel);

    // Set status timer source.
    void SetStatusTimerId( uint8_t ModelId, uint8_t StatusId, uint16_t SetupSourceId);
    uint16_t GetStatusTimerId( uint8_t ModelId, uint8_t StatusId);
    // Same as above for selected model id.
    void SetStatusTimerId( uint8_t StatusId, uint16_t SetupSourceId);
    uint16_t GetStatusTimerId( uint8_t StatusId);

    // Set status info source.
    void SetStatusSourceId( uint8_t ModelId, uint8_t StatusId, uint16_t SetupSourceId);
    uint16_t GetStatusSourceId( uint8_t ModelId, uint8_t StatusId);
    // Same as above for selected model id.
    void SetStatusSourceId( uint8_t StatusId, uint16_t SetupSourceId);
    uint16_t GetStatusSourceId( uint8_t StatusId);

    // Set proxy source.
    void SetProxyReference( uint8_t ModelId, uint8_t ProxyId, const Setup_Source_Tuple* ProxyReference);
    void GetProxyReference( uint8_t ModelId, uint8_t ProxyId, Setup_Source_Tuple* ProxyReference);
    // Same as above for selected model id.
    void SetProxyReference( uint8_t ProxyId, const Setup_Source_Tuple* ProxyReference);
    void GetProxyReference( uint8_t ProxyId, Setup_Source_Tuple* ProxyReference);

    // Find next free proxy slot in the setup memory. Returns false if all slots are used.
    bool FindNextUnusedProxyId( uint8_t* ProxyId);

    // Reset all settings.
    void Reset( void);
    // Reset model.
    void ResetModel( uint8_t ModelId, uint8_t TypeId, ModelState State);
    // Reset type.
    void ResetType( uint8_t TypeId, TypeState State);
    // Reset source.
    void ResetSource( Signal_Source_Source* SignalSource, uint8_t ModelId, uint16_t SetupSourceId,
                      Signal_Source_Source::Type SourceType);
};
