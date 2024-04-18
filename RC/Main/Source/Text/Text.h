// Copyright 2008 Peter Stegemann

#pragma once

#include <avr/pgmspace.h>

#if( SYSTEM_MCU_TYPE == SYSTEM_MCU_2561)
	#define SYSTEM_MCU_NAME "ATmega 2561"
#elif( SYSTEM_MCU_TYPE == SYSTEM_MCU_2560)
	#define SYSTEM_MCU_NAME "ATmega 2561"
#else
	#error "Unknown system mcu."
#endif

class Text
{
  public:
    static const flash_char ThreeDigitPercentFormat[];
    static const flash_char FourDigitPercentFormat[];
    static const flash_char EmptyDigitPercent[];
    static const flash_char Int8Format[];
    static const flash_char UInt8Format[];
    static const flash_char FourDigitInt16Format[];
    static const flash_char SixDigitInt16Format[];
    static const flash_char CharacterFormat[];
    static const flash_char TwinCharacterFormat[];
    static const flash_char TimeFormat[];
    static const flash_char NegativeTimeFormat[];
    static const flash_char VoltageFormat[];
    static const flash_char MillisecondFormat[];

    static const flash_char Plus100Percent[];
    static const flash_char PaddedZeroPercent[];
    static const flash_char Minus100Percent[];
    static const flash_char ZeroPercent[];
    static const flash_char PaddedOff[];

    static const flash_char hermesONE[];

    static const flash_char Add[];
    static const flash_char Analog[];
    static const flash_char Backlight[];
    static const flash_char Battery[];
    static const flash_char BlankTime[];
    static const flash_char Button[];
    static const flash_char Calibration[];
    static const flash_char Cancel[];
    static const flash_char Channel[];
    static const flash_char ChannelMapping[];
    static const flash_char Channels[];
    static const flash_char Clip[];
    static const flash_char Contrast[];
    static const flash_char Critical[];
    static const flash_char Delete[];
    static const flash_char Follower[];
    static const flash_char Followers[];
    static const flash_char Display[];
    static const flash_char Empty[];
    static const flash_char Exit[];
    static const flash_char Fixed[];
    static const flash_char Global[];
    static const flash_char Info[];
    static const flash_char Init[];
    static const flash_char Input[];
    static const flash_char InputA[];
    static const flash_char InputB[];
    static const flash_char Inputs[];
    static const flash_char Inverted[];
    static const flash_char Limit[];
    static const flash_char Low[];
    static const flash_char Main[];
    static const flash_char Map[];
    static const flash_char Maps[];
    static const flash_char Maximum[];
    static const flash_char Minimum[];
    static const flash_char Mix[];
    static const flash_char Mixers[];
    static const flash_char Mode[];
    static const flash_char ModeWarp[];
    static const flash_char ModeClip[];
    static const flash_char Model[];
    static const flash_char Models[];
    static const flash_char Name[];
    static const flash_char None[];
    static const flash_char Ok[];
    static const flash_char Owner[];
    static const flash_char PPM[];
    static const flash_char Proxies[];
    static const flash_char Proxy[];
    static const flash_char Range[];
    static const flash_char Reset[];
    static const flash_char ResetCheck[];
    static const flash_char Reverse[];
    static const flash_char RFEnabled[];
    static const flash_char RFDisabled[];
    static const flash_char RFMode[];
    static const flash_char RFNone[];
    static const flash_char RFModule0[];
    static const flash_char RFModule1[];
    static const flash_char RFBoth[];
    static const flash_char Rotary[];
    static const flash_char Serial[];
    static const flash_char SerialShort[];
    static const flash_char SerialAsk[];
    static const flash_char Setup[];
    static const flash_char Slot[];
    static const flash_char Source[];
    static const flash_char Sources[];
    static const flash_char Status[];
    static const flash_char StatusInverted[];
    static const flash_char Step[];
    static const flash_char Stop[];
    static const flash_char Store[];
    static const flash_char Stores[];
    static const flash_char Switch[];
    static const flash_char System[];
    static const flash_char SystemTime[];
    static const flash_char Target[];
    static const flash_char Ticker[];
    static const flash_char Time[];
    static const flash_char Timer[];
    static const flash_char Timers[];
    static const flash_char Toggle[];
    static const flash_char Trigger[];
    static const flash_char TriggerLowLimit[];
    static const flash_char TriggerHighLimit[];
    static const flash_char Trim[];
    static const flash_char Trimmer[];
    static const flash_char Trimmers[];
    static const flash_char Type[];
    static const flash_char Types[];
    static const flash_char Unknown[];
    static const flash_char Volume[];
    static const flash_char Warn[];
    static const flash_char WarnCritical[];
    static const flash_char WarnLow[];
    static const flash_char WarnPause[];
    static const flash_char Warp[];

    static const flash_char Yes[];
    static const flash_char No[];

    static const flash_char Bottom[];
    static const flash_char Center[];
    static const flash_char Top[];

    static const flash_char ChannelNameFormat[];
    static const flash_char PPMNameFormat[];
    static const flash_char FollowerNameFormat[];
    static const flash_char InputNameFormat[];
    static const flash_char MapNameFormat[];
    static const flash_char MixNameFormat[];
    static const flash_char ProxyNameFormat[];
    static const flash_char StoreNameFormat[];
    static const flash_char TimerNameFormat[];
    static const flash_char TrimmerNameFormat[];
    static const flash_char ModelNameFormat[];
    static const flash_char TypeNameFormat[];

    static const flash_char StatusUsageFormat[];

    static const flash_char CantDeleteCurrentModel[];
    static const flash_char CantDeleteCurrentType[];
    static const flash_char SwitchTypeWarning[];

    static const flash_char DeleteModelFormat[];
    static const flash_char DeleteSourceFollowerFormat[];
    static const flash_char DeleteSourceInputFormat[];
    static const flash_char DeleteSourceMapFormat[];
    static const flash_char DeleteSourceMixFormat[];
    static const flash_char DeleteSourceProxyFormat[];
    static const flash_char DeleteSourceStoreFormat[];
    static const flash_char DeleteSourceTimerFormat[];
    static const flash_char DeleteSourceTrimmerFormat[];
    static const flash_char DeleteSourceUnknownFormat[];
    static const flash_char DeleteTypeFormat[];

    static const flash_char NoProxyId[];
    static const flash_char NoModelStorage[];
    static const flash_char NoSystemStorage[];

    static const flash_char ModelsCount[];
    static const flash_char TypesCount[];
    static const flash_char SourcesCount[];
    static const flash_char InternalEEPROMSize[];
    static const flash_char ExternalEEPROMSize[];
    static const flash_char CPUType[];

    static const flash_char LeftSide[];
    static const flash_char LeftBottom[];
    static const flash_char RightSide[];
    static const flash_char RightBottom[];
    static const flash_char TimerTop[];
    static const flash_char TimerBottom[];
};
