package net.stegemann.io.serial.base;

import java.util.Arrays;
import java.util.List;

public class DesktopProtocol
{
    private static final List< Id> idValues = Arrays.asList( Id.values());

    public static Id enumFromByte( byte byteValue)
    {
        int enumPosition = byteValue & 0x00ff;

        Id enumValue;

        if( enumPosition < idValues.size())
        {
            enumValue = idValues.get( enumPosition);
        }
        else
        {
            enumValue = Id.Unknown;
        }

        // We keep the original byte value for debugging reasons, for example to identify values that get mapped to
        // unknown.
        enumValue.byteValue( byteValue);

        return enumValue;
    }

    public static byte byteFromEnum( Id enumValue)
    {
        int enumPosition = idValues.indexOf( enumValue);

        return ( byte) enumPosition;
    }

    public enum Id
    {
        Ok,
        Error,

        Ping,
        ReadConfiguration,
        WriteConfiguration,

        Configuration,

        Owner,
        SelectedModel,

        PPMs,
        PPM,
        PPMName,

        PPMInverted,
        PPMCenter,

        Outputs,

        AnalogInputs,
        DigitalInputs,
        OutputChannels,

        SetupBacklight,
        SetupBlankTime,
        StatusBacklight,
        StatusContrast,
        StatusBlankTime,
        StatusInverted,

        ChannelMappings,
        ChannelMapping,

        ChannelMode,

        Calibrations,
        Calibration,
        CalibrationHigh,
        CalibrationCenter,
        CalibrationLow,

        Battery,
        BatteryWarnLowVoltage,
        BatteryWarnCriticalVoltage,
        BatteryMinimumVoltage,
        BatteryMaximumVoltage,
        BatteryCalibrationValue,

        Models,
        Model,
        ModelState,
        ModelName,
        ModelType,
        ModelRFMode,

        StatusTimers,
        StatusTimer,
        StatusSources,
        StatusSource,

        ModelProxyReferences,
        ModelProxyReference,

        Types,
        Type,
        TypeState,
        TypeName,

        Channels,
        Channel,
        ChannelName,
        ChannelInput,
        ChannelPoints,
        ChannelPoint,
        ChannelTrim,
        ChannelLimit,
        ChannelReverse,

        Sources,
        Source,
        SourceType,
        SourceName,
        SourceModel,
        SourceTupleSource,
        SourceTupleVolume,

        SourceInputAnalog,
        SourceInputAnalogInput,

        SourceInputButton,
        SourceInputButtonInput,
        SourceInputButtonStore,
        SourceInputButtonInit,
        SourceInputButtonToggle,
        SourceInputButtonTop,
        SourceInputButtonBottom,

        SourceInputRotary,
        SourceInputRotaryAInput,
        SourceInputRotaryBInput,
        SourceInputRotaryStore,
        SourceInputRotaryInit,
        SourceInputRotaryStep,
        SourceInputRotaryTop,
        SourceInputRotaryBottom,

        SourceInputSwitch,
        SourceInputSwitchLowInput,
        SourceInputSwitchHighInput,
        SourceInputSwitchTop,
        SourceInputSwitchBottom,

        SourceInputTicker,
        SourceInputTickerLowInput,
        SourceInputTickerHighInput,
        SourceInputTickerStore,
        SourceInputTickerInit,
        SourceInputTickerStep,
        SourceInputTickerTop,
        SourceInputTickerBottom,

        SourceMap,
        SourceMapInput,
        SourceMapPoints,
        SourceMapPoint,

        SourceMix,
        SourceMixInputs,
        SourceMixInput,

        SourceStore,
        SourceStoreInput,
        SourceStoreInit,

        SourceFollower,
        SourceFollowerTarget,
        SourceFollowerStep,
        SourceFollowerTrigger,
        SourceFollowerTriggerHighLimit,
        SourceFollowerTriggerLowLimit,

        SourceTimer,
        SourceTimerInitTime,
        SourceTimerCurrentTime,
        SourceTimerStore,
        SourceTimerReverse,
        SourceTimerTrigger,
        SourceTimerTriggerHighLimit,
        SourceTimerTriggerLowLimit,
        SourceTimerWarnLowTime,
        SourceTimerWarnCriticalTime,
        SourceTimerWarnPauseTime,

        SourceProxy,
        SourceProxySlot,

        SourceTrimmer,
        SourceTrimmerName,
        SourceTrimmerInput,
        SourceTrimmerPoints,
        SourceTrimmerPoint,
        SourceTrimmerTrim,
        SourceTrimmerLimit,
        SourceTrimmerReverse,

        StorageTypes,
        StorageModels,
        StorageSources,

        // Always keep this at the end. It doesn't really exist in the protocol.
        Unknown;

        public byte byteValue;

        public void byteValue( byte byteValue)
        {
            this.byteValue = byteValue;
        }

        public byte byteValue()
        {
            return byteValue;
        }
    }
}
