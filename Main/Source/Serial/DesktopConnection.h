// Copyright 2008 Peter Stegemann

#ifndef SERIAL_DESKTOPCONNECTION_H
#define SERIAL_DESKTOPCONNECTION_H

#include "DesktopProtocol.h"
#include "Setup/PPM.h"
#include "Setup/Source/Tupel.h"

#include "AVR/Components/String.h"
#include "AVR/Components/Types.h"
#include "AVR/Components/Serial/Serial_TypedConnection.h"

template< uint8_t DeviceId>
class Serial_DesktopConnection
{
	private:
		SERIAL_TypedConnection< DeviceId> connection;

		void sendSetupSourceTupel( uint8_t Id, Setup_Source_Tupel* Value)
		{
			connection.SendComplexOpen( Id);

				connection.SendInteger( Serial_DesktopProtocol::I_SourceTupelSource, Value->Source);
				connection.SendInteger( Serial_DesktopProtocol::I_SourceTupelVolume, Value->Volume);

			connection.SendComplexClose();
		}

		void sendConfiguration( void)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_Configuration);

				connection.SendInteger( Serial_DesktopProtocol::I_AnalogInputs,
					SIGNAL_PROCESSOR_ANALOG_INPUTS);
				connection.SendInteger( Serial_DesktopProtocol::I_DigitalInputs,
					SIGNAL_PROCESSOR_DIGITAL_INPUTS);
				connection.SendInteger( Serial_DesktopProtocol::I_OutputChannels,
					SIGNAL_PPM_CHANNELS);
				connection.SendInteger( Serial_DesktopProtocol::I_Outputs,
					SIGNAL_SERVICE_PPMS);

				connection.SendString( Serial_DesktopProtocol::I_Owner,
					GLOBAL.SetupService.GetOwner( connection.stringBuffer, SERIAL_STRING_SIZE));
				connection.SendInteger( Serial_DesktopProtocol::I_SetupBlankTime,
					GLOBAL.SetupService.GetSetupBlankTime());
				connection.SendInteger( Serial_DesktopProtocol::I_StatusBlankTime,
					GLOBAL.SetupService.GetStatusBlankTime());
				connection.SendInteger( Serial_DesktopProtocol::I_SetupBacklight,
					GLOBAL.SetupService.GetSetupBacklight());
				connection.SendInteger( Serial_DesktopProtocol::I_StatusBacklight,
					GLOBAL.SetupService.GetStatusBacklight());
				connection.SendInteger( Serial_DesktopProtocol::I_StatusContrast,
					GLOBAL.SetupService.GetStatusContrast());
				connection.SendBoolean( Serial_DesktopProtocol::I_StatusInverted,
					GLOBAL.SetupService.GetStatusInverted());
				connection.SendInteger( Serial_DesktopProtocol::I_SelectedModel,
					GLOBAL.SetupService.GetSelectedModelId());

				sendCalibrations();
				sendBattery();
				sendPPMs();
				sendModels();
				sendTypes();
				sendSources();

			connection.SendComplexClose();
		}

		void sendCalibrations( void)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_Calibrations);

				for( uint8_t CalibrationId = 0; CalibrationId < SIGNAL_PROCESSOR_ANALOG_INPUTS;
					 CalibrationId++)
				{
					connection.SendComplexOpen( Serial_DesktopProtocol::I_Calibration);

						Setup_Calibration Calibration;

						GLOBAL.SetupService.GetCalibration( CalibrationId, &Calibration);

						connection.SendInteger( Serial_DesktopProtocol::I_CalibrationHigh,
									 Calibration.Value[ Setup_Calibration::V_High]);
						connection.SendInteger( Serial_DesktopProtocol::I_CalibrationCenter,
									 Calibration.Value[ Setup_Calibration::V_Center]);
						connection.SendInteger( Serial_DesktopProtocol::I_CalibrationLow,
									 Calibration.Value[ Setup_Calibration::V_Low]);

					connection.SendComplexClose();
				}

			connection.SendComplexClose();
		}

		void sendBattery( void)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_Battery);

				Setup_Battery Battery;

				GLOBAL.SetupService.GetBattery( &Battery);

				connection.SendInteger( Serial_DesktopProtocol::I_BatteryWarnLowVoltage,
					Battery.WarnLowVoltage);
				connection.SendInteger( Serial_DesktopProtocol::I_BatteryWarnCriticalVoltage,
					Battery.WarnCriticalVoltage);
				connection.SendInteger( Serial_DesktopProtocol::I_BatteryMinimumVoltage,
					Battery.MinimumVoltage);
				connection.SendInteger( Serial_DesktopProtocol::I_BatteryMaximumVoltage,
					Battery.MaximumVoltage);
				connection.SendInteger( Serial_DesktopProtocol::I_BatteryCalibrationVoltage,
					Battery.CalibrationVoltage);

			connection.SendComplexClose();
		}

		void sendPPMs( void)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_PPMs);

			for( uint8_t PPMId = 0; PPMId < SIGNAL_SERVICE_PPMS; PPMId++)
			{
				connection.SendComplexOpen( Serial_DesktopProtocol::I_PPM);

					Setup_PPM PPM;

					GLOBAL.SetupService.GetPPM( PPMId, &PPM);

					connection.SendBoolean( Serial_DesktopProtocol::I_PPMInverted, PPM.Inverted);
					connection.SendInteger( Serial_DesktopProtocol::I_PPMCenter, PPM.Center);
					connection.SendString( Serial_DesktopProtocol::I_PPMName,
						GLOBAL.SetupService.GetPPMName( PPMId, connection.stringBuffer,
							SERIAL_STRING_SIZE));

					sendChannelMappings( PPM.ChannelMapping);

				connection.SendComplexClose();
			}

			connection.SendComplexClose();
		}

		void sendChannelMappings( uint8_t ChannelMapping[ SIGNAL_PPM_CHANNELS])
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_ChannelMappings);

				for( uint8_t ChannelId = 0; ChannelId < SIGNAL_PPM_CHANNELS; ChannelId++)
				{
					connection.SendInteger( Serial_DesktopProtocol::I_ChannelMapping,
						ChannelMapping[ ChannelId]);
				}

			connection.SendComplexClose();
		}

		void sendModels( void)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_Models);

				for( uint8_t ModelId = 0; ModelId < SETUP_MODELS; ModelId++)
				{
					connection.SendComplexOpen( Serial_DesktopProtocol::I_Model);

						connection.SendInteger( Serial_DesktopProtocol::I_ModelState,
							GLOBAL.SetupService.GetModelState( ModelId));

						if( GLOBAL.SetupService.GetModelState( ModelId) != Setup_Service::MS_Empty)
						{
							connection.SendString( Serial_DesktopProtocol::I_ModelName,
								GLOBAL.SetupService.GetModelName( ModelId, connection.stringBuffer,
									SERIAL_STRING_SIZE));

							connection.SendInteger( Serial_DesktopProtocol::I_ModelType,
								GLOBAL.SetupService.GetSelectedTypeId( ModelId));

							connection.SendInteger( Serial_DesktopProtocol::I_ModelRFMode,
								GLOBAL.SetupService.GetRFMode( ModelId));

							sendChannels( ModelId);

							sendStatusTimers( ModelId);
							sendStatusSources( ModelId);
							sendProxyReferences( ModelId);
						}

					connection.SendComplexClose();
				}

			connection.SendComplexClose();
		}

		void sendChannels( uint8_t ModelId)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_Channels);

				for( uint8_t ChannelId = 0; ChannelId < SIGNAL_PPM_CHANNELS; ChannelId++)
				{
					connection.SendComplexOpen( Serial_DesktopProtocol::I_Channel);

						connection.SendString( Serial_DesktopProtocol::I_ChannelName,
							GLOBAL.SetupService.GetChannelName( ModelId, ChannelId,
								connection.stringBuffer, SERIAL_STRING_SIZE));

						Setup_Channel Channel;

						GLOBAL.SetupService.GetChannel( ModelId, ChannelId, &Channel);

						sendSetupSourceTupel( Serial_DesktopProtocol::I_ChannelInputSource,
							&( Channel.InputSource));
						sendSetupSourceTupel( Serial_DesktopProtocol::I_ChannelTrimSource,
							&( Channel.TrimSource));
						sendSetupSourceTupel( Serial_DesktopProtocol::I_ChannelLimitSource,
							&( Channel.LimitSource));

						connection.SendBoolean( Serial_DesktopProtocol::I_ChannelReverse,
							Channel.Reverse);
						connection.SendInteger( Serial_DesktopProtocol::I_ChannelMode, Channel.Mode);

						connection.SendComplexOpen( Serial_DesktopProtocol::I_ChannelPoints);

							for( uint8_t PointId = 0; PointId < Setup_Channel::PV_PointVolumeCount;
								 PointId++)
							{
								connection.SendInteger( Serial_DesktopProtocol::I_ChannelPoint,
											 Channel.PointVolume[ PointId]);
							}

						connection.SendComplexClose();

					connection.SendComplexClose();
				}

			connection.SendComplexClose();
		}

		void sendStatusTimers( uint8_t ModelId)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_ModelStatusTimers);

			for( uint8_t StatusTimerId = 0; StatusTimerId < SETUP_MODEL_STATUS_TIMERS; StatusTimerId++)
			{
				connection.SendInteger( Serial_DesktopProtocol::I_ModelStatusTimer,
							 GLOBAL.SetupService.GetStatusTimerId( ModelId, StatusTimerId));
			}

			connection.SendComplexClose();
		}

		void sendStatusSources( uint8_t ModelId)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_ModelStatusSources);

				for( uint8_t StatusSourceId = 0; StatusSourceId < SETUP_MODEL_STATUS_SOURCES;
					 StatusSourceId++)
				{
					connection.SendInteger( Serial_DesktopProtocol::I_ModelStatusSource,
								 GLOBAL.SetupService.GetStatusSourceId( ModelId, StatusSourceId));
				}

			connection.SendComplexClose();
		}

		void sendProxyReferences( uint8_t ModelId)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_ModelProxyReferences);

				for( uint8_t ProxyId = 0; ProxyId < SETUP_MODEL_PROXY_SOURCES; ProxyId++)
				{
					Setup_Source_Tupel ProxyReference;

					GLOBAL.SetupService.GetProxyReference( ModelId, ProxyId, &ProxyReference);

					sendSetupSourceTupel( Serial_DesktopProtocol::I_ModelProxyReference,
						&ProxyReference);
				}

			connection.SendComplexClose();
		}

		void sendTypes( void)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_Types);

				for( uint8_t TypeId = SETUP_MODEL_TYPES_START; TypeId < SETUP_MODEL_TYPES_END; TypeId++)
				{
					connection.SendComplexOpen( Serial_DesktopProtocol::I_Type);

						connection.SendInteger( Serial_DesktopProtocol::I_TypeState,
									 GLOBAL.SetupService.GetTypeState( TypeId));

						if( GLOBAL.SetupService.GetTypeState( TypeId) != Setup_Service::TS_Empty)
						{
							connection.SendString( Serial_DesktopProtocol::I_TypeName,
								GLOBAL.SetupService.GetTypeName( TypeId, connection.stringBuffer,
									SERIAL_STRING_SIZE));
						}

					connection.SendComplexClose();
				}

			connection.SendComplexClose();
		}

		void sendSources( void)
		{
			connection.SendComplexOpen( Serial_DesktopProtocol::I_Sources);

				for( uint16_t SourceId = 0; SourceId < SETUP_SOURCES; SourceId++)
				{
					connection.SendComplexOpen( Serial_DesktopProtocol::I_Source);

		//				connection.SendInteger( Serial_DesktopProtocol::I_SourceType,
		//							 GLOBAL.SetupService.GetSourceType( SourceId));

						if( GLOBAL.SetupService.GetSourceType( SourceId) != Signal_Source_Source::T_Empty)
						{
							connection.SendString( Serial_DesktopProtocol::I_SourceName,
										GLOBAL.SetupService.GetSourceName( SourceId, connection.stringBuffer,
																		   SERIAL_STRING_SIZE));

							connection.SendInteger( Serial_DesktopProtocol::I_SourceModel,
										 GLOBAL.SetupService.GetSourceModelId( SourceId));

							switch( GLOBAL.SetupService.GetSourceType( SourceId))
							{
								case Signal_Source_Source::T_Follower :
									sendSourceFollower( SourceId);
								break;
								case Signal_Source_Source::T_Input :
									sendSourceInput( SourceId);
								break;
								case Signal_Source_Source::T_Map :
										sendSourceMap( SourceId);
								break;
								case Signal_Source_Source::T_Mix :
										sendSourceMix( SourceId);
								break;
								case Signal_Source_Source::T_Proxy :
									sendSourceProxy( SourceId);
								break;
								case Signal_Source_Source::T_Store :
									sendSourceStore( SourceId);
								break;
								case Signal_Source_Source::T_Timer :
									sendSourceTimer( SourceId);
								break;
								case Signal_Source_Source::T_Trimmer :
									sendSourceTrimmer( SourceId);
								break;

								case Signal_Source_Source::T_Empty : break;
							}
						}

					connection.SendComplexClose();
				}
			
			connection.SendComplexClose();
		}

		void sendSourceFollower( uint8_t SourceId)
		{
			Setup_Source_Follower SourceFollower;

			GLOBAL.SetupService.GetSourceFollower( SourceId, &SourceFollower);
			
			connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceFollower);

			sendSetupSourceTupel( Serial_DesktopProtocol::I_SourceFollowerTargetSource,
								  &( SourceFollower.TargetSource));
			sendSetupSourceTupel( Serial_DesktopProtocol::I_SourceFollowerStepSource,
								  &( SourceFollower.StepSource));

			connection.SendInteger( Serial_DesktopProtocol::I_SourceFollowerTriggerSource,
						 SourceFollower.TriggerSource);
			connection.SendInteger( Serial_DesktopProtocol::I_SourceFollowerTriggerHighLimit,
						 SourceFollower.TriggerHighLimit);
			connection.SendInteger( Serial_DesktopProtocol::I_SourceFollowerTriggerLowLimit,
						 SourceFollower.TriggerLowLimit);
			
			connection.SendComplexClose();
		}

		void sendSourceInput( uint8_t SourceId)
		{
			Setup_Source_Input SourceInput;

			GLOBAL.SetupService.GetSourceInput( SourceId, &SourceInput);

			switch( SourceInput.Type)
			{
				case Setup_Source_Input::IT_Analog :
				{
					connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceInputAnalog);

						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputAnalogInput,
							SourceInput.InputIdA);

					connection.SendComplexClose();
				}
				break;

				case Setup_Source_Input::IT_Button :
				{
					connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceInputButton);

						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputButtonInput,
							SourceInput.InputIdA);

						connection.SendBoolean( Serial_DesktopProtocol::I_SourceInputButtonStore,
							SourceInput.Store);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputButtonInit,
							SourceInput.InitVolume);
						connection.SendBoolean( Serial_DesktopProtocol::I_SourceInputButtonToggle,
							SourceInput.Toggle);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputButtonTop,
							SourceInput.TopVolume);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputButtonBottom,
							SourceInput.BottomVolume);

					connection.SendComplexClose();
				}
				break;
					
				case Setup_Source_Input::IT_Switch :
				{
					connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceInputSwitch);

						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputSwitchLowInput,
									 SourceInput.InputIdA);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputSwitchHighInput,
									 SourceInput.InputIdB);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputSwitchTop,
									 SourceInput.TopVolume);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputSwitchBottom,
									 SourceInput.BottomVolume);

					connection.SendComplexClose();
				}
				break;
					
				case Setup_Source_Input::IT_Ticker :
				{
					connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceInputTicker);

						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputTickerLowInput,
							SourceInput.InputIdA);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputTickerHighInput,
							SourceInput.InputIdB);

						connection.SendBoolean( Serial_DesktopProtocol::I_SourceInputTickerStore,
							SourceInput.Store);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputTickerInit,
							SourceInput.InitVolume);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputTickerStep,
							SourceInput.StepVolume);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputTickerTop,
							SourceInput.TopVolume);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputTickerBottom,
							SourceInput.BottomVolume);

					connection.SendComplexClose();
				}
				break;
					
				case Setup_Source_Input::IT_Rotary :
				{
					connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceInputRotary);

						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputRotaryAInput,
							SourceInput.InputIdA);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputRotaryBInput,
							SourceInput.InputIdB);

						connection.SendBoolean( Serial_DesktopProtocol::I_SourceInputRotaryStore,
							SourceInput.Store);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputRotaryInit,
							SourceInput.InitVolume);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputRotaryStep,
							SourceInput.StepVolume);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputRotaryTop,
							SourceInput.TopVolume);
						connection.SendInteger( Serial_DesktopProtocol::I_SourceInputRotaryBottom,
							SourceInput.BottomVolume);

					connection.SendComplexClose();
				}
				break;
			}
		}

		void sendSourceMap( uint8_t SourceId)
		{
			Setup_Source_Map SourceMap;
			
			GLOBAL.SetupService.GetSourceMap( SourceId, &SourceMap);

			connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceMap);

				sendSetupSourceTupel( Serial_DesktopProtocol::I_SourceMapInputSource,
					&( SourceMap.InputSource));

				connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceMapPoints);

					for( uint8_t PointId = 0; PointId < SETUP_SOURCE_MAP_POINTS; PointId++)
					{
						sendSetupSourceTupel( Serial_DesktopProtocol::I_SourceMapPoint,
							&( SourceMap.PointSource[ PointId]));
					}

				connection.SendComplexClose();

			connection.SendComplexClose();
		}

		void sendSourceMix( uint8_t SourceId)
		{
			Setup_Source_Mix SourceMix;
			
			GLOBAL.SetupService.GetSourceMix( SourceId, &SourceMix);

			connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceMix);
			
				connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceMixInputs);

					for( uint8_t InputId = 0; InputId < SETUP_SOURCE_MIX_INPUTS; InputId++)
					{
						sendSetupSourceTupel( Serial_DesktopProtocol::I_SourceMixInputSource,
							&( SourceMix.InputSource[ InputId]));
					}

				connection.SendComplexClose();

			connection.SendComplexClose();
		}

		void sendSourceProxy( uint8_t SourceId)
		{
			Setup_Source_Proxy SourceProxy;

			GLOBAL.SetupService.GetSourceProxy( SourceId, &SourceProxy);

			connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceProxy);

				connection.SendInteger( Serial_DesktopProtocol::I_SourceProxySlot, SourceProxy.Slot);

			connection.SendComplexClose();
		}

		void sendSourceStore( uint8_t SourceId)
		{
			Setup_Source_Store SourceStore;

			GLOBAL.SetupService.GetSourceStore( SourceId, &SourceStore);

			connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceStore);

				connection.SendInteger( Serial_DesktopProtocol::I_SourceStoreInputSource,
					SourceStore.InputSource);
				connection.SendInteger( Serial_DesktopProtocol::I_SourceStoreInit,
					SourceStore.InitVolume);

			connection.SendComplexClose();
		}

		void sendSourceTimer( uint8_t SourceId)
		{
			Setup_Source_Timer SourceTimer;

			GLOBAL.SetupService.GetSourceTimer( SourceId, &SourceTimer);

			connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceTimer);

				connection.SendInteger( Serial_DesktopProtocol::I_SourceTimerInitTime,
					SourceTimer.InitTime);
				connection.SendInteger( Serial_DesktopProtocol::I_SourceTimerCurrentTime,
					SourceTimer.CurrentTime);
				connection.SendBoolean( Serial_DesktopProtocol::I_SourceTimerStore, SourceTimer.Store);
				connection.SendBoolean( Serial_DesktopProtocol::I_SourceTimerReverse,
					SourceTimer.Reverse);
				connection.SendInteger( Serial_DesktopProtocol::I_SourceTimerTriggerSource,
					SourceTimer.TriggerSource);
				connection.SendInteger( Serial_DesktopProtocol::I_SourceTimerTriggerHighLimit,
					SourceTimer.TriggerHighLimit);
				connection.SendInteger( Serial_DesktopProtocol::I_SourceTimerTriggerLowLimit,
					SourceTimer.TriggerLowLimit);
				connection.SendInteger( Serial_DesktopProtocol::I_SourceTimerWarnLowTime,
					SourceTimer.WarnLowTime);
				connection.SendInteger( Serial_DesktopProtocol::I_SourceTimerWarnCriticalTime,
					SourceTimer.WarnCriticalTime);
				connection.SendInteger( Serial_DesktopProtocol::I_SourceTimerWarnPauseTime,
					SourceTimer.WarnPauseTime);

			connection.SendComplexClose();
		}

		void sendSourceTrimmer( uint8_t SourceId)
		{
			Setup_Source_Trimmer SourceTrimmer;

			GLOBAL.SetupService.GetSourceTrimmer( SourceId, &SourceTrimmer);

			connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceTrimmer);

				sendSetupSourceTupel( Serial_DesktopProtocol::I_SourceTrimmerInputSource,
					&( SourceTrimmer.InputSource));
				sendSetupSourceTupel( Serial_DesktopProtocol::I_SourceTrimmerTrimSource,
					&( SourceTrimmer.TrimSource));
				sendSetupSourceTupel( Serial_DesktopProtocol::I_SourceTrimmerLimitSource,
					&( SourceTrimmer.LimitSource));

				connection.SendBoolean( Serial_DesktopProtocol::I_SourceTrimmerReverse,
					SourceTrimmer.Reverse);
		//		connection.SendInteger( Serial_DesktopProtocol::I_SourceTrimmerMode, SourceTrimmer.Mode);

				connection.SendComplexOpen( Serial_DesktopProtocol::I_SourceTrimmerPoints);
			
					for( uint8_t PointId = 0; PointId < Setup_Channel::PV_PointVolumeCount; PointId++)
					{
						connection.SendInteger( Serial_DesktopProtocol::I_SourceTrimmerPoint,
							SourceTrimmer.PointVolume[ PointId]);
					}

				connection.SendComplexClose();

			connection.SendComplexClose();	
		}

		void receiveConfiguration( void)
		{
			connection.SendState( Serial_DesktopProtocol::I_Ok);

			uint8_t Type = connection.ReceiveByte();
			uint8_t Id = connection.ReceiveByte();

			switch( Type)
			{
				case SERIAL_Protocol::T_State : break;

				case SERIAL_Protocol::T_Value : connection.ConsumeValue(); break;

				case SERIAL_Protocol::T_Complex :
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
									case SERIAL_Protocol::T_ComplexEnd :
									{
										Loop = false;
									}
									break;

									case SERIAL_Protocol::T_Value :
									{
										Id = connection.ReceiveByte();

										connection.ReceiveValue( connection.stringBuffer,
											SERIAL_STRING_SIZE);

										switch( Id)
										{
											case Serial_DesktopProtocol::I_Owner :
											{
												GLOBAL.SetupService.SetOwner(
													connection.stringBuffer);
											}
											break;

											case Serial_DesktopProtocol::I_SetupBlankTime :
											{
												GLOBAL.SetupService.SetSetupBlankTime(
													atol( connection.stringBuffer));
											}
											break;

											case Serial_DesktopProtocol::I_StatusBlankTime :
											{
												GLOBAL.SetupService.SetStatusBlankTime(
													atol( connection.stringBuffer));
											}
											break;

											case Serial_DesktopProtocol::I_StatusInverted :
											{
												GLOBAL.SetupService.SetStatusInverted(
													STRING::String2Boolean(
														connection.stringBuffer));
											}
											break;

											case Serial_DesktopProtocol::I_SetupBacklight :
											{
												GLOBAL.SetupService.SetSetupBacklight(
													atol( connection.stringBuffer));
											}
											break;

											case Serial_DesktopProtocol::I_StatusBacklight :
											{
												GLOBAL.SetupService.SetStatusBacklight(
													atol( connection.stringBuffer));
											}
											break;

											case Serial_DesktopProtocol::I_StatusContrast :
											{
												GLOBAL.SetupService.SetStatusContrast(
													atol( connection.stringBuffer));
											}
											break;

											case Serial_DesktopProtocol::I_SelectedModel :
											{
												GLOBAL.SetupService.SetSelectedModelId(
													atol( connection.stringBuffer));
											}
											break;

											default : break;
										}
									}
									break;

									case SERIAL_Protocol::T_Complex :
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Complex :
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_CalibrationHigh :
							{
								Calibration.Value[ Setup_Calibration::V_High] =
									atol( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_CalibrationCenter :
							{
								Calibration.Value[ Setup_Calibration::V_Center] =
									atol( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_CalibrationLow :
							{
								Calibration.Value[ Setup_Calibration::V_Low] =
									atol( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);
						
						switch( Id)
						{
							case Serial_DesktopProtocol::I_BatteryWarnLowVoltage :
							{
								Battery.WarnLowVoltage = atol( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_BatteryWarnCriticalVoltage :
							{
								Battery.WarnCriticalVoltage = atol( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_BatteryMinimumVoltage :
							{
								Battery.MinimumVoltage = atol( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_BatteryMaximumVoltage :
							{
								Battery.MaximumVoltage = atol( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_BatteryCalibrationVoltage :
							{
								Battery.CalibrationVoltage = atol( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Complex :
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_PPMInverted :
							{
								PPM.Inverted = STRING::String2Boolean( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_PPMCenter :
							{
								PPM.Center = atol( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_PPMName :
							{
								GLOBAL.SetupService.SetPPMName( PPMId, connection.stringBuffer);
							}
							break;

							default : break;
						}
					}
					break;

					case SERIAL_Protocol::T_Complex :
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_ChannelMapping :
							{
								ChannelMapping[ ChannelId] = atol( connection.stringBuffer);

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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Complex :
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_ModelState :
							{
								Setup_Service::ModelState Value =
									( Setup_Service::ModelState) atoi( connection.stringBuffer);
								GLOBAL.SetupService.SetModelState( ModelId, Value);
							}
							break;

							case Serial_DesktopProtocol::I_ModelName :
							{
								GLOBAL.SetupService.SetModelName( ModelId, connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_ModelType :
							{
								GLOBAL.SetupService.SetSelectedTypeId( ModelId, atoi( connection.stringBuffer));
							}
							break;

							case Serial_DesktopProtocol::I_ModelRFMode :
							{
								GLOBAL.SetupService.SetRFMode( ModelId, atoi( connection.stringBuffer));
							}
							break;

							default : break;
						}
					}
					break;

					case SERIAL_Protocol::T_Complex :
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Complex :
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_ChannelName :
							{
								GLOBAL.SetupService.SetChannelName( ModelId, ChannelId,
									connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_ChannelReverse :
							{
								Channel.Reverse = STRING::String2Boolean( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_ChannelMode :
							{
								Channel.Mode = atol( connection.stringBuffer);
							}
							break;

							default : break;
						}
					}
					break;

					case SERIAL_Protocol::T_Complex :
					{
						uint8_t Id = connection.ReceiveByte();

						switch( Id)
						{
							case Serial_DesktopProtocol::I_ChannelInputSource :
							{
								receiveSetupSourceTupel( &( Channel.InputSource));
							}
							break;

							case Serial_DesktopProtocol::I_ChannelTrimSource :
							{
								receiveSetupSourceTupel( &( Channel.TrimSource));
							}
							break;

							case Serial_DesktopProtocol::I_ChannelLimitSource :
							{
								receiveSetupSourceTupel( &( Channel.LimitSource));
							}
							break;

							case Serial_DesktopProtocol::I_ChannelPoints :
							{
								receiveVolumes( Channel.PointVolume,
									Setup_Channel::PV_PointVolumeCount,
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						if( Id == ValueId)
						{
							// Don't read to many.
							if( PointId < NumberOfValues)
							{
								Values[ PointId++] = atol( connection.stringBuffer);
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
			uint8_t SourceId = 0;

			bool Loop = true;

			while( Loop)
			{
				uint8_t Type = connection.ReceiveByte();

				switch( Type)
				{
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_ModelStatusTimer :
							{
								GLOBAL.SetupService.SetStatusTimerId( ModelId, SourceId,
									atoi( connection.stringBuffer));

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
			uint8_t SourceId = 0;

			bool Loop = true;

			while( Loop)
			{
				uint8_t Type = connection.ReceiveByte();

				switch( Type)
				{
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_ModelStatusSource :
							{
								GLOBAL.SetupService.SetStatusSourceId( ModelId, SourceId,
									atol( connection.stringBuffer));

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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Complex :
					{
						uint8_t Id = connection.ReceiveByte();

						switch( Id)
						{
							case Serial_DesktopProtocol::I_ModelProxyReference :
							{
								Setup_Source_Tupel ProxyReference;

								receiveSetupSourceTupel( &ProxyReference);

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

		void receiveSetupSourceTupels( Setup_Source_Tupel* Values, uint8_t NumberOfValues,
			uint16_t ValueId)
		{
			uint8_t TupelId = 0;

			bool Loop = true;

			while( Loop)
			{
				uint8_t Type = connection.ReceiveByte();

				switch( Type)
				{
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Complex :
					{
						uint8_t Id = connection.ReceiveByte();

						if( Id == ValueId)
						{
							// Don't read to many.
							if( TupelId < NumberOfValues)
							{
								receiveSetupSourceTupel( &Values[ TupelId]);
							}
							else
							{
								connection.ConsumeComplex();
							}
							
							TupelId++;
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

		void receiveSetupSourceTupel( Setup_Source_Tupel* Value)
		{
			bool Loop = true;

			while( Loop)
			{
				uint8_t Type = connection.ReceiveByte();

				switch( Type)
				{
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceTupelSource :
							{
								Value->Source = atol( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceTupelVolume :
							{
								Value->Volume = atol( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Complex :
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_TypeState :
							{
								Setup_Service::TypeState Value =
									( Setup_Service::TypeState) atoi( connection.stringBuffer);
								GLOBAL.SetupService.SetTypeState( TypeId, Value);
							}
							break;

							case Serial_DesktopProtocol::I_TypeName :
							{
								GLOBAL.SetupService.SetTypeName( TypeId, connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Complex :
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceName :
							{
								GLOBAL.SetupService.SetSourceName( SourceId, connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceModel :
							{
								GLOBAL.SetupService.SetSourceModelId( SourceId, atoi( connection.stringBuffer));
							}
							break;

							default : break;
						}
					}
					break;

					case SERIAL_Protocol::T_Complex :
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;
						
					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceFollowerTriggerSource :
							{
								SourceFollower.TriggerSource = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceFollowerTriggerHighLimit :
							{
								SourceFollower.TriggerHighLimit = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceFollowerTriggerLowLimit :
							{
								SourceFollower.TriggerLowLimit = atoi( connection.stringBuffer);
							}
							break;

							default : break;
						}
					}
					break;

					case SERIAL_Protocol::T_Complex :
					{
						uint8_t Id = connection.ReceiveByte();

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceFollowerTargetSource :
							{
								receiveSetupSourceTupel( &( SourceFollower.TargetSource));
							}
							break;

							case Serial_DesktopProtocol::I_SourceFollowerStepSource :
							{
								receiveSetupSourceTupel( &( SourceFollower.StepSource));
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceInputAnalogInput :
							{
								SourceInput.InputIdA = atoi( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceInputButtonInput :
							{
								SourceInput.InputIdA = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputButtonStore :
							{
								SourceInput.Store = STRING::String2Boolean( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputButtonInit :
							{
								SourceInput.InitVolume = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputButtonToggle :
							{
								SourceInput.Toggle = STRING::String2Boolean( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputButtonTop :
							{
								SourceInput.TopVolume = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputButtonBottom :
							{
								SourceInput.BottomVolume = atoi( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceInputSwitchLowInput :
							{
								SourceInput.InputIdA = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputSwitchHighInput :
							{
								SourceInput.InputIdB = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputSwitchTop :
							{
								SourceInput.TopVolume = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputSwitchBottom :
							{
								SourceInput.BottomVolume = atoi( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);
						
						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceInputTickerLowInput :
							{
								SourceInput.InputIdA = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputTickerHighInput :
							{
								SourceInput.InputIdB = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputTickerStore :
							{
								SourceInput.Store = STRING::String2Boolean( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputTickerInit :
							{
								SourceInput.InitVolume = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputTickerStep :
							{
								SourceInput.StepVolume = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputTickerTop :
							{
								SourceInput.TopVolume = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputTickerBottom :
							{
								SourceInput.BottomVolume = atoi( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);
						
						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceInputRotaryAInput :
							{
								SourceInput.InputIdA = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputRotaryBInput :
							{
								SourceInput.InputIdB = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputRotaryStore :
							{
								SourceInput.Store = STRING::String2Boolean( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputRotaryInit :
							{
								SourceInput.InitVolume = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputRotaryStep :
							{
								SourceInput.StepVolume = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputRotaryTop :
							{
								SourceInput.TopVolume = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceInputRotaryBottom :
							{
								SourceInput.BottomVolume = atoi( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Complex :
					{
						uint8_t Id = connection.ReceiveByte();

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceMapInputSource :
							{
								receiveSetupSourceTupel( &( SourceMap.InputSource));
							}
							break;

							case Serial_DesktopProtocol::I_SourceMapPoints :
							{
								receiveSetupSourceTupels( SourceMap.PointSource,
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Complex :
					{
						uint8_t Id = connection.ReceiveByte();

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceMixInputs :
							{
								receiveSetupSourceTupels( SourceMix.InputSource, SETUP_SOURCE_MIX_INPUTS,
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceStoreInputSource :
							{
								SourceStore.InputSource = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceStoreInit :
							{
								SourceStore.InitVolume = atoi( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();
			
						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceProxySlot :
							{
								SourceProxy.Slot = atoi( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceTimerInitTime :
							{
								SourceTimer.InitTime = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceTimerCurrentTime :
							{
								SourceTimer.CurrentTime = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceTimerStore :
							{
								SourceTimer.Store = STRING::String2Boolean( connection.stringBuffer);
							}
							break;
								
							case Serial_DesktopProtocol::I_SourceTimerReverse :
							{
								SourceTimer.Reverse = STRING::String2Boolean( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceTimerTriggerSource :
							{
								SourceTimer.TriggerSource = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceTimerTriggerHighLimit :
							{
								SourceTimer.TriggerHighLimit = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceTimerTriggerLowLimit :
							{
								SourceTimer.TriggerLowLimit = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceTimerWarnLowTime :
							{
								SourceTimer.WarnLowTime = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceTimerWarnCriticalTime :
							{
								SourceTimer.WarnCriticalTime = atoi( connection.stringBuffer);
							}
							break;

							case Serial_DesktopProtocol::I_SourceTimerWarnPauseTime :
							{
								SourceTimer.WarnPauseTime = atoi( connection.stringBuffer);
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
					case SERIAL_Protocol::T_ComplexEnd :
					{
						Loop = false;
					}
					break;

					case SERIAL_Protocol::T_Value :
					{
						uint8_t Id = connection.ReceiveByte();

						connection.ReceiveValue( connection.stringBuffer, SERIAL_STRING_SIZE);

						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceTrimmerReverse :
							{
								SourceTrimmer.Reverse = STRING::String2Boolean(
									connection.stringBuffer);
							}
							break;
		/*
							case Serial_DesktopProtocol::I_SourceTrimmerMode :
							{
								SourceTrimmer.Mode = atol( connection.stringBuffer);
							}
							break;
		*/
							default : break;
						}
					}
					break;

					case SERIAL_Protocol::T_Complex :
					{
						uint8_t Id = connection.ReceiveByte();
						
						switch( Id)
						{
							case Serial_DesktopProtocol::I_SourceTrimmerInputSource :
							{
								receiveSetupSourceTupel( &( SourceTrimmer.InputSource));
							}
							break;

							case Serial_DesktopProtocol::I_SourceTrimmerTrimSource :
							{
								receiveSetupSourceTupel( &( SourceTrimmer.TrimSource));
							}
							break;

							case Serial_DesktopProtocol::I_SourceTrimmerLimitSource :
							{
								receiveSetupSourceTupel( &( SourceTrimmer.LimitSource));
							}
							break;

							case Serial_DesktopProtocol::I_SourceTrimmerPoints :
							{
								receiveVolumes( SourceTrimmer.PointVolume,
												Setup_Source_Trimmer::PV_PointVolumeCount,
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
						case SERIAL_Protocol::T_Command :
						{
							uint8_t Command = connection.ReceiveByte();
							
							switch( Command)
							{
								case Serial_DesktopProtocol::I_Ping :
								{
									connection.SendState( Serial_DesktopProtocol::I_Ok);
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
									UTILITY::HardReset();
									// No comin' back.
								}
								break;

								default :
								{
									connection.SendState( Serial_DesktopProtocol::I_Error);
								}
								break;
							}
						}
						break;

						default :
						{
							connection.SendState( Serial_DesktopProtocol::I_Error);
						}
						break;
					}
				}

				UTILITY::Pause( 5);

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

					GLOBAL.Lcd.PrintFormat( 0, 10, FONT::FI_Mini, LCD::White, LCD::Black,
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

					GLOBAL.Lcd.PrintFormat( 0, 20, FONT::FI_Mini, LCD::White, LCD::Black,
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

#endif
