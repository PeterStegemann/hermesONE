// Copyright 2008 Peter Stegemann

#pragma once

#include "DesktopProtocol.h"
#include "Setup/PPM.h"
#include "Setup/Source/Tuple.h"

#include "AVR/Source/String.h"
#include "AVR/Source/Types.h"
#include "AVR/Source/Serial/TypedConnection.h"

template< uint8_t DeviceId>
class Serial_DesktopConnection
{
  private:
    avr::serial::TypedConnection< DeviceId> connection;

    void openComplex( uint8_t Id)
    {
        connection.SendComplexOpen( Id);
    }

    void closeComplex()
    {
        connection.SendComplexClose();
    }

    void sendBoolean( uint8_t Id, bool Value)
    {
        connection.SendBoolean( Id, Value);
    }

    void sendLong( uint8_t Id, int32_t Value)
    {
        connection.SendLong( Id, Value);
    }

    void sendState( Serial_DesktopProtocol::Id Id)
    {
        connection.SendState( Id);
    }

    void sendString( uint8_t Id, const char* Value)
    {
        connection.SendString( Id, Value);
    }

    void sendSetupSourceTuple( uint8_t Id, Setup_Source_Tuple* Value)
    {
        openComplex( Id);

            sendLong( Serial_DesktopProtocol::I_SourceTupleSource, Value->Source);
            sendLong( Serial_DesktopProtocol::I_SourceTupleVolume, Value->Volume);

        closeComplex();
    }

    void sendConfiguration( void)
    {
        openComplex( Serial_DesktopProtocol::I_Configuration);

            sendLong( Serial_DesktopProtocol::I_AnalogInputs, SIGNAL_PROCESSOR_ANALOG_INPUTS);
            sendLong( Serial_DesktopProtocol::I_DigitalInputs, SIGNAL_PROCESSOR_DIGITAL_INPUTS);
            sendLong( Serial_DesktopProtocol::I_OutputChannels, SIGNAL_PPM_CHANNELS);
            sendLong( Serial_DesktopProtocol::I_Outputs, SIGNAL_SERVICE_PPMS);

            sendString( Serial_DesktopProtocol::I_Owner,
               GLOBAL.SetupService.GetOwner( connection.GetStringBuffer(), SERIAL_STRING_SIZE));
            sendLong( Serial_DesktopProtocol::I_SetupBlankTime, GLOBAL.SetupService.GetSetupBlankTime());
            sendLong( Serial_DesktopProtocol::I_StatusBlankTime, GLOBAL.SetupService.GetStatusBlankTime());
            sendLong( Serial_DesktopProtocol::I_SetupBacklight, GLOBAL.SetupService.GetSetupBacklight());
            sendLong( Serial_DesktopProtocol::I_StatusBacklight, GLOBAL.SetupService.GetStatusBacklight());
            sendLong( Serial_DesktopProtocol::I_StatusContrast, GLOBAL.SetupService.GetStatusContrast());
            sendBoolean( Serial_DesktopProtocol::I_StatusInverted, GLOBAL.SetupService.GetStatusInverted());
            sendLong( Serial_DesktopProtocol::I_SelectedModel, GLOBAL.SetupService.GetSelectedModelId());

            sendCalibrations();
            sendBattery();
            sendPPMs();
            sendModels();
            sendTypes();
            sendSources();

        closeComplex();
    }

    void sendCalibrations( void)
    {
        openComplex( Serial_DesktopProtocol::I_Calibrations);

            for( uint8_t CalibrationId = 0; CalibrationId < SIGNAL_PROCESSOR_ANALOG_INPUTS; CalibrationId++)
            {
                openComplex( Serial_DesktopProtocol::I_Calibration);

                    Setup_Calibration Calibration;

                    GLOBAL.SetupService.GetCalibration( CalibrationId, &Calibration);

                    sendLong( Serial_DesktopProtocol::I_CalibrationHigh,
                        Calibration.Value[ Setup_Calibration::V_High]);
                    sendLong( Serial_DesktopProtocol::I_CalibrationCenter,
                        Calibration.Value[ Setup_Calibration::V_Center]);
                    sendLong( Serial_DesktopProtocol::I_CalibrationLow,
                        Calibration.Value[ Setup_Calibration::V_Low]);

                closeComplex();
            }

        closeComplex();
    }

    void sendBattery( void)
    {
        openComplex( Serial_DesktopProtocol::I_Battery);

            Setup_Battery Battery;

            GLOBAL.SetupService.GetBattery( &Battery);

            sendLong( Serial_DesktopProtocol::I_BatteryWarnLowVoltage, Battery.WarnLowVoltage);
            sendLong( Serial_DesktopProtocol::I_BatteryWarnCriticalVoltage, Battery.WarnCriticalVoltage);
            sendLong( Serial_DesktopProtocol::I_BatteryMinimumVoltage, Battery.MinimumVoltage);
            sendLong( Serial_DesktopProtocol::I_BatteryMaximumVoltage, Battery.MaximumVoltage);
            sendLong( Serial_DesktopProtocol::I_BatteryCalibrationVoltage, Battery.CalibrationVoltage);

        closeComplex();
    }

    void sendPPMs( void)
    {
        openComplex( Serial_DesktopProtocol::I_PPMs);

        for( uint8_t PPMId = 0; PPMId < SIGNAL_SERVICE_PPMS; PPMId++)
        {
            openComplex( Serial_DesktopProtocol::I_PPM);

                Setup_PPM PPM;

                GLOBAL.SetupService.GetPPM( PPMId, &PPM);

                sendBoolean( Serial_DesktopProtocol::I_PPMInverted, PPM.Inverted);
                sendLong( Serial_DesktopProtocol::I_PPMCenter, PPM.Center);
                sendString( Serial_DesktopProtocol::I_PPMName,
                    GLOBAL.SetupService.GetPPMName( PPMId, connection.GetStringBuffer(), SERIAL_STRING_SIZE));

                sendChannelMappings( PPM.ChannelMapping);

            closeComplex();
        }

        closeComplex();
    }

    void sendChannelMappings( uint8_t ChannelMapping[ SIGNAL_PPM_CHANNELS])
    {
        openComplex( Serial_DesktopProtocol::I_ChannelMappings);

            for( uint8_t ChannelId = 0; ChannelId < SIGNAL_PPM_CHANNELS; ChannelId++)
            {
                sendLong( Serial_DesktopProtocol::I_ChannelMapping, ChannelMapping[ ChannelId]);
            }

        closeComplex();
    }

    void sendModels( void)
    {
        openComplex( Serial_DesktopProtocol::I_Models);

            for( uint8_t ModelId = 0; ModelId < SETUP_MODELS; ModelId++)
            {
                openComplex( Serial_DesktopProtocol::I_Model);

                    sendLong( Serial_DesktopProtocol::I_ModelState, GLOBAL.SetupService.GetModelState( ModelId));

                    if( GLOBAL.SetupService.GetModelState( ModelId) != Setup_Service::MS_Empty)
                    {
                        sendString( Serial_DesktopProtocol::I_ModelName,
                            GLOBAL.SetupService.GetModelName( ModelId, connection.GetStringBuffer(), SERIAL_STRING_SIZE));

                        sendLong( Serial_DesktopProtocol::I_ModelType, GLOBAL.SetupService.GetSelectedTypeId( ModelId));

                        sendLong( Serial_DesktopProtocol::I_ModelRFMode, GLOBAL.SetupService.GetRFMode( ModelId));

                        sendChannels( ModelId);

                        sendStatusTimers( ModelId);
                        sendStatusSources( ModelId);
                        sendProxyReferences( ModelId);
                    }

                closeComplex();
            }

        closeComplex();
    }

    void sendChannels( uint8_t ModelId)
    {
        openComplex( Serial_DesktopProtocol::I_Channels);

            for( uint8_t ChannelId = 0; ChannelId < SIGNAL_PPM_CHANNELS; ChannelId++)
            {
                openComplex( Serial_DesktopProtocol::I_Channel);

                    sendString( Serial_DesktopProtocol::I_ChannelName,
                        GLOBAL.SetupService.GetChannelName( ModelId, ChannelId,
                            connection.GetStringBuffer(), SERIAL_STRING_SIZE));

                    Setup_Channel Channel;

                    GLOBAL.SetupService.GetChannel( ModelId, ChannelId, &Channel);

                    sendSetupSourceTuple( Serial_DesktopProtocol::I_ChannelInputSource, &( Channel.InputSource));
                    sendSetupSourceTuple( Serial_DesktopProtocol::I_ChannelTrimSource, &( Channel.TrimSource));
                    sendSetupSourceTuple( Serial_DesktopProtocol::I_ChannelLimitSource, &( Channel.LimitSource));

                    sendBoolean( Serial_DesktopProtocol::I_ChannelReverse, Channel.Reverse);
                    sendLong( Serial_DesktopProtocol::I_ChannelMode, Channel.Mode);

                    openComplex( Serial_DesktopProtocol::I_ChannelPoints);

                        for( uint8_t PointId = 0; PointId < Setup_Channel::PV_PointVolumeCount; PointId++)
                        {
                            sendLong( Serial_DesktopProtocol::I_ChannelPoint, Channel.PointVolume[ PointId]);
                        }

                    closeComplex();

                closeComplex();
            }

        closeComplex();
    }

    void sendStatusTimers( uint8_t ModelId)
    {
        openComplex( Serial_DesktopProtocol::I_ModelStatusTimers);

        for( uint8_t StatusTimerId = 0; StatusTimerId < SETUP_MODEL_STATUS_TIMERS; StatusTimerId++)
        {
            sendLong( Serial_DesktopProtocol::I_ModelStatusTimer,
                GLOBAL.SetupService.GetStatusTimerId( ModelId, StatusTimerId));
        }

        closeComplex();
    }

    void sendStatusSources( uint8_t ModelId)
    {
        openComplex( Serial_DesktopProtocol::I_ModelStatusSources);

            for( uint8_t StatusSourceId = 0; StatusSourceId < SETUP_MODEL_STATUS_SOURCES; StatusSourceId++)
            {
                sendLong( Serial_DesktopProtocol::I_ModelStatusSource,
                    GLOBAL.SetupService.GetStatusSourceId( ModelId, StatusSourceId));
            }

        closeComplex();
    }

    void sendProxyReferences( uint8_t ModelId)
    {
        openComplex( Serial_DesktopProtocol::I_ModelProxyReferences);

            for( uint8_t ProxyId = 0; ProxyId < SETUP_MODEL_PROXY_SOURCES; ProxyId++)
            {
                Setup_Source_Tuple ProxyReference;

                GLOBAL.SetupService.GetProxyReference( ModelId, ProxyId, &ProxyReference);

                sendSetupSourceTuple( Serial_DesktopProtocol::I_ModelProxyReference, &ProxyReference);
            }

        closeComplex();
    }

    void sendTypes( void)
    {
        openComplex( Serial_DesktopProtocol::I_Types);

            for( uint8_t TypeId = SETUP_MODEL_TYPES_START; TypeId < SETUP_MODEL_TYPES_END; TypeId++)
            {
                openComplex( Serial_DesktopProtocol::I_Type);

                    sendLong( Serial_DesktopProtocol::I_TypeState, GLOBAL.SetupService.GetTypeState( TypeId));

                    if( GLOBAL.SetupService.GetTypeState( TypeId) != Setup_Service::TS_Empty)
                    {
                        sendString( Serial_DesktopProtocol::I_TypeName,
                            GLOBAL.SetupService.GetTypeName( TypeId, connection.GetStringBuffer(), SERIAL_STRING_SIZE));
                    }

                closeComplex();
            }

        closeComplex();
    }

    void sendSources( void)
    {
        openComplex( Serial_DesktopProtocol::I_Sources);

            for( uint16_t SourceId = 0; SourceId < SETUP_SOURCES; SourceId++)
            {
                openComplex( Serial_DesktopProtocol::I_Source);

    //				sendLong( Serial_DesktopProtocol::I_SourceType,
    //							 GLOBAL.SetupService.GetSourceType( SourceId));

                    if( GLOBAL.SetupService.GetSourceType( SourceId) != Signal_Source_Source::T_Empty)
                    {
                        sendString( Serial_DesktopProtocol::I_SourceName, GLOBAL.SetupService.GetSourceName(
                            SourceId, connection.GetStringBuffer(), SERIAL_STRING_SIZE));

                        sendLong( Serial_DesktopProtocol::I_SourceModel,
                            GLOBAL.SetupService.GetSourceModelId( SourceId));

                        switch( GLOBAL.SetupService.GetSourceType( SourceId))
                        {
                            case Signal_Source_Source::T_Follower : sendSourceFollower( SourceId);
                            break;
                            case Signal_Source_Source::T_Input : sendSourceInput( SourceId);
                            break;
                            case Signal_Source_Source::T_Map : sendSourceMap( SourceId);
                            break;
                            case Signal_Source_Source::T_Mix : sendSourceMix( SourceId);
                            break;
                            case Signal_Source_Source::T_Proxy : sendSourceProxy( SourceId);
                            break;
                            case Signal_Source_Source::T_Store : sendSourceStore( SourceId);
                            break;
                            case Signal_Source_Source::T_Timer : sendSourceTimer( SourceId);
                            break;
                            case Signal_Source_Source::T_Trimmer : sendSourceTrimmer( SourceId);
                            break;

                            case Signal_Source_Source::T_Empty : break;
                        }
                    }

                closeComplex();
            }

        closeComplex();
    }

    void sendSourceFollower( uint8_t SourceId)
    {
        Setup_Source_Follower SourceFollower;

        GLOBAL.SetupService.GetSourceFollower( SourceId, &SourceFollower);

        openComplex( Serial_DesktopProtocol::I_SourceFollower);

            sendSetupSourceTuple( Serial_DesktopProtocol::I_SourceFollowerTargetSource,
                &( SourceFollower.TargetSource));
            sendSetupSourceTuple( Serial_DesktopProtocol::I_SourceFollowerStepSource,
                &( SourceFollower.StepSource));

            sendLong( Serial_DesktopProtocol::I_SourceFollowerTriggerSource, SourceFollower.TriggerSource);
            sendLong( Serial_DesktopProtocol::I_SourceFollowerTriggerHighLimit, SourceFollower.TriggerHighLimit);
            sendLong( Serial_DesktopProtocol::I_SourceFollowerTriggerLowLimit, SourceFollower.TriggerLowLimit);

        closeComplex();
    }

    void sendSourceInput( uint16_t SourceId)
    {
        Setup_Source_Input SourceInput;

        GLOBAL.SetupService.GetSourceInput( SourceId, &SourceInput);

        switch( SourceInput.Type)
        {
            case Setup_Source_Input::IT_Analog :
            {
                openComplex( Serial_DesktopProtocol::I_SourceInputAnalog);

                    sendLong( Serial_DesktopProtocol::I_SourceInputAnalogInput, SourceInput.InputIdA);

                closeComplex();
            }
            break;

            case Setup_Source_Input::IT_Button :
            {
                openComplex( Serial_DesktopProtocol::I_SourceInputButton);

                    sendLong( Serial_DesktopProtocol::I_SourceInputButtonInput, SourceInput.InputIdA);

                    sendBoolean( Serial_DesktopProtocol::I_SourceInputButtonStore, SourceInput.Store);
                    sendLong( Serial_DesktopProtocol::I_SourceInputButtonInit, SourceInput.InitVolume);
                    sendBoolean( Serial_DesktopProtocol::I_SourceInputButtonToggle, SourceInput.Toggle);
                    sendLong( Serial_DesktopProtocol::I_SourceInputButtonTop, SourceInput.TopVolume);
                    sendLong( Serial_DesktopProtocol::I_SourceInputButtonBottom, SourceInput.BottomVolume);

                closeComplex();
            }
            break;

            case Setup_Source_Input::IT_Switch :
            {
                openComplex( Serial_DesktopProtocol::I_SourceInputSwitch);

                    sendLong( Serial_DesktopProtocol::I_SourceInputSwitchLowInput, SourceInput.InputIdA);
                    sendLong( Serial_DesktopProtocol::I_SourceInputSwitchHighInput, SourceInput.InputIdB);
                    sendLong( Serial_DesktopProtocol::I_SourceInputSwitchTop, SourceInput.TopVolume);
                    sendLong( Serial_DesktopProtocol::I_SourceInputSwitchBottom, SourceInput.BottomVolume);

                closeComplex();
            }
            break;

            case Setup_Source_Input::IT_Ticker :
            {
                openComplex( Serial_DesktopProtocol::I_SourceInputTicker);

                    sendLong( Serial_DesktopProtocol::I_SourceInputTickerLowInput, SourceInput.InputIdA);
                    sendLong( Serial_DesktopProtocol::I_SourceInputTickerHighInput, SourceInput.InputIdB);

                    sendBoolean( Serial_DesktopProtocol::I_SourceInputTickerStore, SourceInput.Store);
                    sendLong( Serial_DesktopProtocol::I_SourceInputTickerInit, SourceInput.InitVolume);
                    sendLong( Serial_DesktopProtocol::I_SourceInputTickerStep, SourceInput.StepVolume);
                    sendLong( Serial_DesktopProtocol::I_SourceInputTickerTop, SourceInput.TopVolume);
                    sendLong( Serial_DesktopProtocol::I_SourceInputTickerBottom, SourceInput.BottomVolume);

                closeComplex();
            }
            break;

            case Setup_Source_Input::IT_Rotary :
            {
                openComplex( Serial_DesktopProtocol::I_SourceInputRotary);

                    sendLong( Serial_DesktopProtocol::I_SourceInputRotaryAInput, SourceInput.InputIdA);
                    sendLong( Serial_DesktopProtocol::I_SourceInputRotaryBInput, SourceInput.InputIdB);

                    sendBoolean( Serial_DesktopProtocol::I_SourceInputRotaryStore, SourceInput.Store);
                    sendLong( Serial_DesktopProtocol::I_SourceInputRotaryInit, SourceInput.InitVolume);
                    sendLong( Serial_DesktopProtocol::I_SourceInputRotaryStep, SourceInput.StepVolume);
                    sendLong( Serial_DesktopProtocol::I_SourceInputRotaryTop, SourceInput.TopVolume);
                    sendLong( Serial_DesktopProtocol::I_SourceInputRotaryBottom, SourceInput.BottomVolume);

                closeComplex();
            }
            break;
        }
    }

    void sendSourceMap( uint16_t SourceId)
    {
        Setup_Source_Map SourceMap;

        GLOBAL.SetupService.GetSourceMap( SourceId, &SourceMap);

        openComplex( Serial_DesktopProtocol::I_SourceMap);

            sendSetupSourceTuple( Serial_DesktopProtocol::I_SourceMapInputSource, &( SourceMap.InputSource));

            openComplex( Serial_DesktopProtocol::I_SourceMapPoints);

                for( uint8_t PointId = 0; PointId < SETUP_SOURCE_MAP_POINTS; PointId++)
                {
                    sendSetupSourceTuple( Serial_DesktopProtocol::I_SourceMapPoint,
                        &( SourceMap.PointSource[ PointId]));
                }

            closeComplex();

        closeComplex();
    }

    void sendSourceMix( uint16_t SourceId)
    {
        Setup_Source_Mix SourceMix;

        GLOBAL.SetupService.GetSourceMix( SourceId, &SourceMix);

        openComplex( Serial_DesktopProtocol::I_SourceMix);

            openComplex( Serial_DesktopProtocol::I_SourceMixInputs);

                for( uint8_t InputId = 0; InputId < SETUP_SOURCE_MIX_INPUTS; InputId++)
                {
                    sendSetupSourceTuple( Serial_DesktopProtocol::I_SourceMixInputSource,
                        &( SourceMix.InputSource[ InputId]));
                }

            closeComplex();

        closeComplex();
    }

    void sendSourceProxy( uint16_t SourceId)
    {
        Setup_Source_Proxy SourceProxy;

        GLOBAL.SetupService.GetSourceProxy( SourceId, &SourceProxy);

        openComplex( Serial_DesktopProtocol::I_SourceProxy);

            sendLong( Serial_DesktopProtocol::I_SourceProxySlot, SourceProxy.Slot);

        closeComplex();
    }

    void sendSourceStore( uint16_t SourceId)
    {
        Setup_Source_Store SourceStore;

        GLOBAL.SetupService.GetSourceStore( SourceId, &SourceStore);

        openComplex( Serial_DesktopProtocol::I_SourceStore);

            sendLong( Serial_DesktopProtocol::I_SourceStoreInputSource, SourceStore.InputSource);
            sendLong( Serial_DesktopProtocol::I_SourceStoreInit, SourceStore.InitVolume);

        closeComplex();
    }

    void sendSourceTimer( uint16_t SourceId)
    {
        Setup_Source_Timer SourceTimer;

        GLOBAL.SetupService.GetSourceTimer( SourceId, &SourceTimer);

        openComplex( Serial_DesktopProtocol::I_SourceTimer);

            sendLong( Serial_DesktopProtocol::I_SourceTimerInitTime, SourceTimer.InitTime);
            sendLong( Serial_DesktopProtocol::I_SourceTimerCurrentTime, SourceTimer.CurrentTime);
            sendBoolean( Serial_DesktopProtocol::I_SourceTimerStore, SourceTimer.Store);
            sendBoolean( Serial_DesktopProtocol::I_SourceTimerReverse, SourceTimer.Reverse);
            sendLong( Serial_DesktopProtocol::I_SourceTimerTriggerSource, SourceTimer.TriggerSource);
            sendLong( Serial_DesktopProtocol::I_SourceTimerTriggerHighLimit,
                                 SourceTimer.TriggerHighLimit);
            sendLong( Serial_DesktopProtocol::I_SourceTimerTriggerLowLimit, SourceTimer.TriggerLowLimit);
            sendLong( Serial_DesktopProtocol::I_SourceTimerWarnLowTime, SourceTimer.WarnLowTime);
            sendLong( Serial_DesktopProtocol::I_SourceTimerWarnCriticalTime,
                                 SourceTimer.WarnCriticalTime);
            sendLong( Serial_DesktopProtocol::I_SourceTimerWarnPauseTime, SourceTimer.WarnPauseTime);

        closeComplex();
    }

    void sendSourceTrimmer( uint16_t SourceId)
    {
        Setup_Source_Trimmer SourceTrimmer;

        GLOBAL.SetupService.GetSourceTrimmer( SourceId, &SourceTrimmer);

        openComplex( Serial_DesktopProtocol::I_SourceTrimmer);

            sendSetupSourceTuple( Serial_DesktopProtocol::I_SourceTrimmerInputSource,
                                  &( SourceTrimmer.InputSource));
            sendSetupSourceTuple( Serial_DesktopProtocol::I_SourceTrimmerTrimSource, &( SourceTrimmer.TrimSource));
            sendSetupSourceTuple( Serial_DesktopProtocol::I_SourceTrimmerLimitSource,
                                  &( SourceTrimmer.LimitSource));

            sendBoolean( Serial_DesktopProtocol::I_SourceTrimmerReverse, SourceTrimmer.Reverse);
    //		sendLong( Serial_DesktopProtocol::I_SourceTrimmerMode, SourceTrimmer.Mode);

            openComplex( Serial_DesktopProtocol::I_SourceTrimmerPoints);

                for( uint8_t PointId = 0; PointId < Setup_Channel::PV_PointVolumeCount; PointId++)
                {
                    sendLong( Serial_DesktopProtocol::I_SourceTrimmerPoint, SourceTrimmer.PointVolume[ PointId]);
                }

            closeComplex();

        closeComplex();
    }

    void receiveConfiguration( void)
    {
        sendState( Serial_DesktopProtocol::I_Ok);

        uint8_t Type = connection.ReceiveByte();
        uint8_t Id = connection.ReceiveByte();

        switch( Type)
        {
            case avr::serial::Protocol::T_State : break;

            case avr::serial::Protocol::T_Value : connection.ConsumeValue(); break;

            case avr::serial::Protocol::T_Complex :
            {
                switch( Id)
                {
                    case Serial_DesktopProtocol::I_Configuration :
                    {
                        bool Loop = true;

                        while( Loop)
                        {
                            Type = connection.ReceiveByte();

                            switch( Type)
                            {
                                case avr::serial::Protocol::T_ComplexEnd :
                                {
                                    Loop = false;
                                }
                                break;

                                case avr::serial::Protocol::T_Value :
                                {
                                    Id = connection.ReceiveByte();

                                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                                    switch( Id)
                                    {
                                        case Serial_DesktopProtocol::I_Owner :
                                        {
                                            GLOBAL.SetupService.SetOwner( connection.GetStringBuffer());
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_SetupBlankTime :
                                        {
                                            GLOBAL.SetupService.SetSetupBlankTime( atol( connection.GetStringBuffer()));
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_StatusBlankTime :
                                        {
                                            GLOBAL.SetupService.SetStatusBlankTime( atol( connection.GetStringBuffer()));
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_StatusInverted :
                                        {
                                            GLOBAL.SetupService.SetStatusInverted(
                                                avr::String::String2Boolean( connection.GetStringBuffer()));
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_SetupBacklight :
                                        {
                                            GLOBAL.SetupService.SetSetupBacklight( atol( connection.GetStringBuffer()));
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_StatusBacklight :
                                        {
                                            GLOBAL.SetupService.SetStatusBacklight( atol( connection.GetStringBuffer()));
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_StatusContrast :
                                        {
                                            GLOBAL.SetupService.SetStatusContrast( atol( connection.GetStringBuffer()));
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_SelectedModel :
                                        {
                                            GLOBAL.SetupService.SetSelectedModelId( atol( connection.GetStringBuffer()));
                                        }
                                        break;

                                        default : break;
                                    }
                                }
                                break;

                                case avr::serial::Protocol::T_Complex :
                                {
                                    Id = connection.ReceiveByte();

                                    switch( Id)
                                    {
                                        case Serial_DesktopProtocol::I_Calibrations :
                                        {
                                            receiveCalibrations();
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_Battery :
                                        {
                                            receiveBattery();
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_PPMs :
                                        {
                                            receivePPMs();
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_Models :
                                        {
                                            receiveModels();
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_Types :
                                        {
                                            receiveTypes();
                                        }
                                        break;

                                        case Serial_DesktopProtocol::I_Sources :
                                        {
                                            receiveSources();
                                        }
                                        break;

                                        default : connection.ConsumeComplex(); break;
                                    }
                                }
                                break;

                                default : connection.ConsumeItem( Type); break;
                            }
                        }
                    }
                    break;

                    default : connection.ConsumeComplex(); break;
                }
            }
            break;
        }
    }

    void receiveCalibrations( void)
    {
        uint8_t CalibrationId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_Calibration :
                        {
                            receiveCalibration( CalibrationId);

                            CalibrationId++;
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveCalibration( uint8_t CalibrationId)
    {
        Setup_Calibration Calibration;

        GLOBAL.SetupService.GetCalibration( CalibrationId, &Calibration);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_CalibrationHigh :
                        {
                            Calibration.Value[ Setup_Calibration::V_High] = atol( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_CalibrationCenter :
                        {
                            Calibration.Value[ Setup_Calibration::V_Center] = atol( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_CalibrationLow :
                        {
                            Calibration.Value[ Setup_Calibration::V_Low] = atol( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetCalibration( CalibrationId, &Calibration);
    }

    void receiveBattery( void)
    {
        Setup_Battery Battery;

        GLOBAL.SetupService.GetBattery( &Battery);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_BatteryWarnLowVoltage :
                        {
                            Battery.WarnLowVoltage = atol( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_BatteryWarnCriticalVoltage :
                        {
                            Battery.WarnCriticalVoltage = atol( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_BatteryMinimumVoltage :
                        {
                            Battery.MinimumVoltage = atol( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_BatteryMaximumVoltage :
                        {
                            Battery.MaximumVoltage = atol( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_BatteryCalibrationVoltage :
                        {
                            Battery.CalibrationVoltage = atol( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetBattery( &Battery);
    }

    void receivePPMs( void)
    {
        uint8_t PPMId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_PPM :
                        {
                            receivePPM( PPMId);

                            PPMId++;
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receivePPM( uint8_t PPMId)
    {
        Setup_PPM PPM;

        GLOBAL.SetupService.GetPPM( PPMId, &PPM);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_PPMInverted :
                        {
                            PPM.Inverted = avr::String::String2Boolean( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_PPMCenter :
                        {
                            PPM.Center = atol( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_PPMName :
                        {
                            GLOBAL.SetupService.SetPPMName( PPMId, connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_ChannelMappings :
                        {
                            receiveChannelMappings( PPM.ChannelMapping);
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetPPM( PPMId, &PPM);
    }

    void receiveChannelMappings( uint8_t ChannelMapping[ SIGNAL_PPM_CHANNELS])
    {
        uint8_t ChannelId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_ChannelMapping :
                        {
                            ChannelMapping[ ChannelId] = atol( connection.GetStringBuffer());

                            ChannelId++;
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveModels( void)
    {
        uint8_t ModelId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_Model :
                        {
                            receiveModel( ModelId);

                            ModelId++;
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveModel( uint8_t ModelId)
    {
        // Clear model for the case it has been deleted.
        GLOBAL.SetupService.SetModelState( ModelId, Setup_Service::MS_Empty);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_ModelState :
                        {
                            Setup_Service::ModelState Value =
                                ( Setup_Service::ModelState) atoi( connection.GetStringBuffer());
                            GLOBAL.SetupService.SetModelState( ModelId, Value);
                        }
                        break;

                        case Serial_DesktopProtocol::I_ModelName :
                        {
                            GLOBAL.SetupService.SetModelName( ModelId, connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_ModelType :
                        {
                            GLOBAL.SetupService.SetSelectedTypeId( ModelId, atoi( connection.GetStringBuffer()));
                        }
                        break;

                        case Serial_DesktopProtocol::I_ModelRFMode :
                        {
                            GLOBAL.SetupService.SetRFMode( ModelId, atoi( connection.GetStringBuffer()));
                        }
                        break;

                        default : break;
                    }
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_Channels :
                        {
                            receiveChannels( ModelId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_ModelStatusTimers :
                        {
                            receiveStatusTimers( ModelId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_ModelStatusSources :
                        {
                            receiveStatusSources( ModelId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_ModelProxyReferences :
                        {
                            receiveProxyReferences( ModelId);
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveChannels( uint8_t ModelId)
    {
        uint8_t ChannelId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_Channel :
                        {
                            receiveChannel( ModelId, ChannelId);

                            ChannelId++;
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveChannel( uint8_t ModelId, uint8_t ChannelId)
    {
        Setup_Channel Channel;

        GLOBAL.SetupService.GetChannel( ModelId, ChannelId, &Channel);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_ChannelName :
                        {
                            GLOBAL.SetupService.SetChannelName( ModelId, ChannelId, connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_ChannelReverse :
                        {
                            Channel.Reverse = avr::String::String2Boolean( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_ChannelMode :
                        {
                            Channel.Mode = atol( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_ChannelInputSource :
                        {
                            receiveSetupSourceTuple( &( Channel.InputSource));
                        }
                        break;

                        case Serial_DesktopProtocol::I_ChannelTrimSource :
                        {
                            receiveSetupSourceTuple( &( Channel.TrimSource));
                        }
                        break;

                        case Serial_DesktopProtocol::I_ChannelLimitSource :
                        {
                            receiveSetupSourceTuple( &( Channel.LimitSource));
                        }
                        break;

                        case Serial_DesktopProtocol::I_ChannelPoints :
                        {
                            receiveVolumes( Channel.PointVolume, Setup_Channel::PV_PointVolumeCount,
                                            Serial_DesktopProtocol::I_ChannelPoint);
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetChannel( ModelId, ChannelId, &Channel);
    }

    void receiveVolumes( int16_t* Values, uint8_t NumberOfValues, uint16_t ValueId)
    {
        uint8_t PointId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    if( Id == ValueId)
                    {
                        // Don't read to many.
                        if( PointId < NumberOfValues)
                        {
                            Values[ PointId++] = atol( connection.GetStringBuffer());
                        }
                        else
                        {
                            // Skip
                        }
                    }
                    else
                    {
                        // Skip
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveStatusTimers( uint8_t ModelId)
    {
        uint16_t SourceId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_ModelStatusTimer :
                        {
                            GLOBAL.SetupService.SetStatusTimerId( ModelId, SourceId,
                                                                  atoi( connection.GetStringBuffer()));

                            SourceId++;
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveStatusSources( uint8_t ModelId)
    {
        uint16_t SourceId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_ModelStatusSource :
                        {
                            GLOBAL.SetupService.SetStatusSourceId( ModelId, SourceId,
                                atol( connection.GetStringBuffer()));

                            SourceId++;
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveProxyReferences( uint8_t ModelId)
    {
        uint8_t ProxyId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_ModelProxyReference :
                        {
                            Setup_Source_Tuple ProxyReference;

                            receiveSetupSourceTuple( &ProxyReference);

                            GLOBAL.SetupService.SetProxyReference( ModelId, ProxyId, &ProxyReference);

                            ProxyId++;
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveSetupSourceTuples( Setup_Source_Tuple* Values, uint8_t NumberOfValues, uint16_t ValueId)
    {
        uint8_t TupleId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    if( Id == ValueId)
                    {
                        // Don't read to many.
                        if( TupleId < NumberOfValues)
                        {
                            receiveSetupSourceTuple( &Values[ TupleId]);
                        }
                        else
                        {
                            connection.ConsumeComplex();
                        }

                        TupleId++;
                    }
                    else
                    {
                        connection.ConsumeComplex();
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveSetupSourceTuple( Setup_Source_Tuple* Value)
    {
        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceTupleSource :
                        {
                            Value->Source = atol( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTupleVolume :
                        {
                            Value->Volume = atol( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveTypes( void)
    {
        uint8_t TypeId = SETUP_MODEL_TYPES_START;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_Type :
                        {
                            receiveType( TypeId);

                            TypeId++;
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveType( uint8_t TypeId)
    {
        // Clear type for the case it has been deleted.
        GLOBAL.SetupService.SetTypeState( TypeId, Setup_Service::TS_Empty);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_TypeState :
                        {
                            Setup_Service::TypeState Value =
                                ( Setup_Service::TypeState) atoi( connection.GetStringBuffer());
                            GLOBAL.SetupService.SetTypeState( TypeId, Value);
                        }
                        break;

                        case Serial_DesktopProtocol::I_TypeName :
                        {
                            GLOBAL.SetupService.SetTypeName( TypeId, connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveSources( void)
    {
        uint16_t SourceId = 0;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_Source :
                        {
                            receiveSource( SourceId);

                            SourceId++;
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveSource( uint16_t SourceId)
    {
        // Clear source for the case it has been deleted.
        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Empty);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceName :
                        {
                            GLOBAL.SetupService.SetSourceName( SourceId, connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceModel :
                        {
                            GLOBAL.SetupService.SetSourceModelId( SourceId, atoi( connection.GetStringBuffer()));
                        }
                        break;

                        default : break;
                    }
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceFollower :
                        {
                            receiveSourceFollower( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputAnalog :
                        {
                            receiveSourceInputAnalog( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputButton :
                        {
                            receiveSourceInputButton( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputSwitch :
                        {
                            receiveSourceInputSwitch( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputTicker :
                        {
                            receiveSourceInputTicker( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputRotary :
                        {
                            receiveSourceInputRotary( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceMap :
                        {
                            receiveSourceMap( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceMix :
                        {
                            receiveSourceMix( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceProxy :
                        {
                            receiveSourceProxy( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceStore :
                        {
                            receiveSourceStore( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTimer :
                        {
                            receiveSourceTimer( SourceId);
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTrimmer :
                        {
                            receiveSourceTrimmer( SourceId);
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }
    }

    void receiveSourceFollower( uint16_t SourceId)
    {
        Setup_Source_Follower SourceFollower;

        GLOBAL.SetupService.GetSourceFollower( SourceId, &SourceFollower);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceFollowerTriggerSource :
                        {
                            SourceFollower.TriggerSource = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceFollowerTriggerHighLimit :
                        {
                            SourceFollower.TriggerHighLimit = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceFollowerTriggerLowLimit :
                        {
                            SourceFollower.TriggerLowLimit = atoi( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceFollowerTargetSource :
                        {
                            receiveSetupSourceTuple( &( SourceFollower.TargetSource));
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceFollowerStepSource :
                        {
                            receiveSetupSourceTuple( &( SourceFollower.StepSource));
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Follower);
        GLOBAL.SetupService.SetSourceFollower( SourceId, &SourceFollower);
    }

    void receiveSourceInputAnalog( uint16_t SourceId)
    {
        Setup_Source_Input SourceInput;

        GLOBAL.SetupService.GetSourceInput( SourceId, &SourceInput);

        SourceInput.Type = Setup_Source_Input::IT_Analog;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceInputAnalogInput :
                        {
                            SourceInput.InputIdA = atoi( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Input);
        GLOBAL.SetupService.SetSourceInput( SourceId, &SourceInput);
    }

    void receiveSourceInputButton( uint16_t SourceId)
    {
        Setup_Source_Input SourceInput;

        GLOBAL.SetupService.GetSourceInput( SourceId, &SourceInput);

        SourceInput.Type = Setup_Source_Input::IT_Button;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceInputButtonInput :
                        {
                            SourceInput.InputIdA = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputButtonStore :
                        {
                            SourceInput.Store = avr::String::String2Boolean( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputButtonInit :
                        {
                            SourceInput.InitVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputButtonToggle :
                        {
                            SourceInput.Toggle = avr::String::String2Boolean( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputButtonTop :
                        {
                            SourceInput.TopVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputButtonBottom :
                        {
                            SourceInput.BottomVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Input);
        GLOBAL.SetupService.SetSourceInput( SourceId, &SourceInput);
    }

    void receiveSourceInputSwitch( uint16_t SourceId)
    {
        Setup_Source_Input SourceInput;

        GLOBAL.SetupService.GetSourceInput( SourceId, &SourceInput);

        SourceInput.Type = Setup_Source_Input::IT_Switch;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceInputSwitchLowInput :
                        {
                            SourceInput.InputIdA = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputSwitchHighInput :
                        {
                            SourceInput.InputIdB = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputSwitchTop :
                        {
                            SourceInput.TopVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputSwitchBottom :
                        {
                            SourceInput.BottomVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Input);
        GLOBAL.SetupService.SetSourceInput( SourceId, &SourceInput);
    }

    void receiveSourceInputTicker( uint16_t SourceId)
    {
        Setup_Source_Input SourceInput;

        GLOBAL.SetupService.GetSourceInput( SourceId, &SourceInput);

        SourceInput.Type = Setup_Source_Input::IT_Ticker;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceInputTickerLowInput :
                        {
                            SourceInput.InputIdA = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputTickerHighInput :
                        {
                            SourceInput.InputIdB = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputTickerStore :
                        {
                            SourceInput.Store = avr::String::String2Boolean( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputTickerInit :
                        {
                            SourceInput.InitVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputTickerStep :
                        {
                            SourceInput.StepVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputTickerTop :
                        {
                            SourceInput.TopVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputTickerBottom :
                        {
                            SourceInput.BottomVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Input);
        GLOBAL.SetupService.SetSourceInput( SourceId, &SourceInput);
    }

    void receiveSourceInputRotary( uint16_t SourceId)
    {
        Setup_Source_Input SourceInput;

        GLOBAL.SetupService.GetSourceInput( SourceId, &SourceInput);

        SourceInput.Type = Setup_Source_Input::IT_Rotary;

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceInputRotaryAInput :
                        {
                            SourceInput.InputIdA = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputRotaryBInput :
                        {
                            SourceInput.InputIdB = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputRotaryStore :
                        {
                            SourceInput.Store = avr::String::String2Boolean( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputRotaryInit :
                        {
                            SourceInput.InitVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputRotaryStep :
                        {
                            SourceInput.StepVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputRotaryTop :
                        {
                            SourceInput.TopVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceInputRotaryBottom :
                        {
                            SourceInput.BottomVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Input);
        GLOBAL.SetupService.SetSourceInput( SourceId, &SourceInput);
    }

    void receiveSourceMap( uint16_t SourceId)
    {
        Setup_Source_Map SourceMap;

        GLOBAL.SetupService.GetSourceMap( SourceId, &SourceMap);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceMapInputSource :
                        {
                            receiveSetupSourceTuple( &( SourceMap.InputSource));
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceMapPoints :
                        {
                            receiveSetupSourceTuples( SourceMap.PointSource,
                                SETUP_SOURCE_MAP_POINTS,
                                Serial_DesktopProtocol::I_SourceMapPoint);
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Map);
        GLOBAL.SetupService.SetSourceMap( SourceId, &SourceMap);
    }

    void receiveSourceMix( uint16_t SourceId)
    {
        Setup_Source_Mix SourceMix;

        GLOBAL.SetupService.GetSourceMix( SourceId, &SourceMix);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceMixInputs :
                        {
                            receiveSetupSourceTuples( SourceMix.InputSource, SETUP_SOURCE_MIX_INPUTS,
                                                      Serial_DesktopProtocol::I_SourceMixInputSource);
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Mix);
        GLOBAL.SetupService.SetSourceMix( SourceId, &SourceMix);
    }

    void receiveSourceStore( uint16_t SourceId)
    {
        Setup_Source_Store SourceStore;

        GLOBAL.SetupService.GetSourceStore( SourceId, &SourceStore);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceStoreInputSource :
                        {
                            SourceStore.InputSource = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceStoreInit :
                        {
                            SourceStore.InitVolume = atoi( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Store);
        GLOBAL.SetupService.SetSourceStore( SourceId, &SourceStore);
    }

    void receiveSourceProxy( uint16_t SourceId)
    {
        Setup_Source_Proxy SourceProxy;

        GLOBAL.SetupService.GetSourceProxy( SourceId, &SourceProxy);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceProxySlot :
                        {
                            SourceProxy.Slot = atoi( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Proxy);
        GLOBAL.SetupService.SetSourceProxy( SourceId, &SourceProxy);
    }

    void receiveSourceTimer( uint16_t SourceId)
    {
        Setup_Source_Timer SourceTimer;

        GLOBAL.SetupService.GetSourceTimer( SourceId, &SourceTimer);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceTimerInitTime :
                        {
                            SourceTimer.InitTime = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTimerCurrentTime :
                        {
                            SourceTimer.CurrentTime = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTimerStore :
                        {
                            SourceTimer.Store = avr::String::String2Boolean( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTimerReverse :
                        {
                            SourceTimer.Reverse = avr::String::String2Boolean( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTimerTriggerSource :
                        {
                            SourceTimer.TriggerSource = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTimerTriggerHighLimit :
                        {
                            SourceTimer.TriggerHighLimit = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTimerTriggerLowLimit :
                        {
                            SourceTimer.TriggerLowLimit = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTimerWarnLowTime :
                        {
                            SourceTimer.WarnLowTime = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTimerWarnCriticalTime :
                        {
                            SourceTimer.WarnCriticalTime = atoi( connection.GetStringBuffer());
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTimerWarnPauseTime :
                        {
                            SourceTimer.WarnPauseTime = atoi( connection.GetStringBuffer());
                        }
                        break;

                        default : break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Timer);
        GLOBAL.SetupService.SetSourceTimer( SourceId, &SourceTimer);
    }

    void receiveSourceTrimmer( uint16_t SourceId)
    {
        Setup_Source_Trimmer SourceTrimmer;

        GLOBAL.SetupService.GetSourceTrimmer( SourceId, &SourceTrimmer);

        bool Loop = true;

        while( Loop)
        {
            uint8_t Type = connection.ReceiveByte();

            switch( Type)
            {
                case avr::serial::Protocol::T_ComplexEnd :
                {
                    Loop = false;
                }
                break;

                case avr::serial::Protocol::T_Value :
                {
                    uint8_t Id = connection.ReceiveByte();

                    connection.ReceiveValue( connection.GetStringBuffer(), SERIAL_STRING_SIZE);

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceTrimmerReverse :
                        {
                            SourceTrimmer.Reverse = avr::String::String2Boolean( connection.GetStringBuffer());
                        }
                        break;
    /*
                        case Serial_DesktopProtocol::I_SourceTrimmerMode :
                        {
                            SourceTrimmer.Mode = atol( connection.GetStringBuffer());
                        }
                        break;
    */
                        default : break;
                    }
                }
                break;

                case avr::serial::Protocol::T_Complex :
                {
                    uint8_t Id = connection.ReceiveByte();

                    switch( Id)
                    {
                        case Serial_DesktopProtocol::I_SourceTrimmerInputSource :
                        {
                            receiveSetupSourceTuple( &( SourceTrimmer.InputSource));
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTrimmerTrimSource :
                        {
                            receiveSetupSourceTuple( &( SourceTrimmer.TrimSource));
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTrimmerLimitSource :
                        {
                            receiveSetupSourceTuple( &( SourceTrimmer.LimitSource));
                        }
                        break;

                        case Serial_DesktopProtocol::I_SourceTrimmerPoints :
                        {
                            receiveVolumes( SourceTrimmer.PointVolume, Setup_Source_Trimmer::PV_PointVolumeCount,
                                            Serial_DesktopProtocol::I_SourceTrimmerPoint);
                        }
                        break;

                        default : connection.ConsumeComplex(); break;
                    }
                }
                break;

                default : connection.ConsumeItem( Type); break;
            }
        }

        GLOBAL.SetupService.SetSourceType( SourceId, Signal_Source_Source::T_Trimmer);
        GLOBAL.SetupService.SetSourceTrimmer( SourceId, &SourceTrimmer);
    }

  public:
    Serial_DesktopConnection( void)
    {
    }

    void Initialize()
    {
        connection.Initialize();
    }

    void DoSerialConnection( void)
    {
        while( true)
        {
            // Receive command.
            if( connection.ReadyToReceive())
            {
                uint8_t Type = connection.ReceiveByte();

                switch( Type)
                {
                    case avr::serial::Protocol::T_Command :
                    {
                        uint8_t Command = connection.ReceiveByte();

                        switch( Command)
                        {
                            case Serial_DesktopProtocol::I_Ping :
                            {
                                sendState( Serial_DesktopProtocol::I_Ok);
                            }
                            break;

                            case Serial_DesktopProtocol::I_ReadConfiguration :
                            {
                                sendConfiguration();
                            }
                            break;

                            case Serial_DesktopProtocol::I_WriteConfiguration :
                            {
                                receiveConfiguration();

                                GLOBAL.ClearScreens();
                                avr::Utility::HardReset();
                                // No comin' back.
                            }
                            break;

                            default :
                            {
                                sendState( Serial_DesktopProtocol::I_Error);
                            }
                            break;
                        }
                    }
                    break;

                    default :
                    {
                        sendState( Serial_DesktopProtocol::I_Error);
                    }
                    break;
                }
            }

            avr::Utility::Pause( 5);

            // Exit on button toggle.
            uint8_t RotaryButton;

            // Try rotary.
            GLOBAL.InputService.GetRotary( NULL, &RotaryButton, NULL);

            if( RotaryButton > 0)
            {
                break;
            }
        }
    }
    /*
    void DoSerialConnectionTest( void)
    {
        char DataToSend = 'A';

        while( true)
        {
            // Send
            if( GLOBAL.ReadyToSend())
            {
                connection.SendByte( DataToSend);

                GLOBAL.Lcd.PrintFormat( 0, 10, avr::font::FI_Mini, LCD::White, LCD::Black,
                LCD::PO_Proportional, "%c", DataToSend);

                DataToSend++;

                if( DataToSend == ( 'Z' + 1))
                {
                    DataToSend = 'A';
                }
            }

            // Receive
            if( readyToReceive())
            {
                char DataReceived = connection.ReceiveByte();

                GLOBAL.Lcd.PrintFormat( 0, 20, avr::font::FI_Mini, LCD::White, LCD::Black,
                                      LCD::PO_Proportional, "%c", DataReceived);
            }

            // Exit on button to be toggle.
            uint8_t RotaryButton;

            // Try rotary.
            GLOBAL.InputService.GetRotary( NULL, &RotaryButton, NULL);

            if( RotaryButton > 0)
            {
                break;
            }
        }
    }
    */
};
