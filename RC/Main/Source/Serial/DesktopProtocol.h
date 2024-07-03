// Copyright 2008 Peter Stegemann

#pragma once

#include "AVR/Source/Serial/Protocol.h"

class Serial_DesktopProtocol : public avr::serial::Protocol
{
  public:
	enum Id		// uint8
	{
		I_Ok,
		I_Error,

		I_Ping,
		I_ReadConfiguration,
		I_WriteConfiguration,

		I_Configuration,

		I_Owner,
		I_SelectedModel,

		I_PPMs,
		I_PPM,
		I_PPMName,

		I_PPMInverted,
		I_PPMCenter,

		I_Outputs,

		I_AnalogInputs,
		I_DigitalInputs,
		I_OutputChannels,

		I_SetupBacklight,
		I_SetupBlankTime,
		I_StatusBacklight,
		I_StatusContrast,
		I_StatusBlankTime,
		I_StatusInverted,

		I_ChannelMappings,
		I_ChannelMapping,

		I_ChannelMode,

		I_Calibrations,
		I_Calibration,
		I_CalibrationHigh,
		I_CalibrationCenter,
		I_CalibrationLow,

		I_Battery,
		I_BatteryWarnLowVoltage,
		I_BatteryWarnCriticalVoltage,
		I_BatteryMinimumVoltage,
		I_BatteryMaximumVoltage,
		I_BatteryCalibrationValue,

		I_Models,
		I_Model,
		I_ModelState,
		I_ModelName,
		I_ModelType,
		I_ModelRFMode,

		I_ModelStatusTimers,
		I_ModelStatusTimer,
		I_ModelStatusSources,
		I_ModelStatusSource,

		I_ModelProxyReferences,
		I_ModelProxyReference,

		I_Types,
		I_Type,
		I_TypeState,
		I_TypeName,

		I_Channels,
		I_Channel,
		I_ChannelName,
		I_ChannelInputSource,
		I_ChannelPoints,
		I_ChannelPoint,
		I_ChannelTrimSource,
		I_ChannelLimitSource,
		I_ChannelReverse,

		I_Sources,
		I_Source,
		I_SourceType,
		I_SourceName,
		I_SourceModel,
		I_SourceTupleSource,
		I_SourceTupleVolume,

		I_SourceInputAnalog,
		I_SourceInputAnalogInput,

		I_SourceInputButton,
		I_SourceInputButtonInput,
		I_SourceInputButtonStore,
		I_SourceInputButtonInit,
		I_SourceInputButtonToggle,
		I_SourceInputButtonTop,
		I_SourceInputButtonBottom,

		I_SourceInputRotary,
		I_SourceInputRotaryAInput,
		I_SourceInputRotaryBInput,
		I_SourceInputRotaryStore,
		I_SourceInputRotaryInit,
		I_SourceInputRotaryStep,
		I_SourceInputRotaryTop,
		I_SourceInputRotaryBottom,

		I_SourceInputSwitch,
		I_SourceInputSwitchLowInput,
		I_SourceInputSwitchHighInput,
		I_SourceInputSwitchTop,
		I_SourceInputSwitchBottom,

		I_SourceInputTicker,
		I_SourceInputTickerLowInput,
		I_SourceInputTickerHighInput,
		I_SourceInputTickerStore,
		I_SourceInputTickerInit,
		I_SourceInputTickerStep,
		I_SourceInputTickerTop,
		I_SourceInputTickerBottom,

		I_SourceMap,
		I_SourceMapInputSource,
		I_SourceMapPoints,
		I_SourceMapPoint,

		I_SourceMix,
		I_SourceMixInputs,
		I_SourceMixInputSource,

		I_SourceStore,
		I_SourceStoreInputSource,
		I_SourceStoreInit,

		I_SourceFollower,
		I_SourceFollowerTargetSource,
		I_SourceFollowerStepSource,
		I_SourceFollowerTriggerSource,
		I_SourceFollowerTriggerHighLimit,
		I_SourceFollowerTriggerLowLimit,

		I_SourceTimer,
		I_SourceTimerInitTime,
		I_SourceTimerCurrentTime,
		I_SourceTimerStore,
		I_SourceTimerReverse,
		I_SourceTimerTriggerSource,
		I_SourceTimerTriggerHighLimit,
		I_SourceTimerTriggerLowLimit,
		I_SourceTimerWarnLowTime,
		I_SourceTimerWarnCriticalTime,
		I_SourceTimerWarnPauseTime,

		I_SourceProxy,
		I_SourceProxySlot,

		I_SourceTrimmer,
		I_SourceTrimmerName,
		I_SourceTrimmerInputSource,
		I_SourceTrimmerPoints,
		I_SourceTrimmerPoint,
		I_SourceTrimmerTrimSource,
		I_SourceTrimmerLimitSource,
		I_SourceTrimmerReverse,

		I_StorageTypes,
		I_StorageModels,
        I_StorageSources,

		I_IdCount
	};
};
