// Copyright 2010 Peter Stegemann

#pragma once

#include "Base.h"
#include "GUI/Setup/Gauge.h"
#include "GUI/Setup/Label.h"
#include "GUI/Setup/Select.h"
//#include "GUI/Setup/RangeGauge.h"
#include "GUI/Setup/TextInput.h"
#include "Setup/Source/Source.h"
#include "Signal/Utility.h"

class Screen_Setup_Source_Follower : public Screen_Setup_Source_Base
{
  private:
    Signal_Source_Follower* sourceFollower;

    GUI_Setup_Label targetNameLabel;
    char targetName[ SETUP_SOURCE_NAME_SIZE + 1];
    GUI_Setup_Label targetVolumeLabel;

    GUI_Setup_Gauge targetGauge;

    GUI_Setup_Label stepNameLabel;
    char stepName[ SETUP_SOURCE_NAME_SIZE + 1];
    GUI_Setup_Label stepVolumeLabel;

    GUI_Setup_Gauge stepGauge;

    GUI_Setup_Label triggerNameLabel;
    char triggerName[ SETUP_SOURCE_NAME_SIZE + 1];

    GUI_Setup_Gauge triggerGauge;

    GUI_Setup_Label triggerHighLimitLabel;
    GUI_Setup_Label triggerLowLimitLabel;

    virtual void display( void)
    {
    	Screen_Setup_Source_Base::display();

    	// Adjust gauges to frame and set them up.
	    const FONT_Type* Font = FONT::GetFont( SCREEN_SETUP_BASE_MAIN_FONT);

    	uint16_t ValueLeft = menuLeft + 12 * Font->CellWidth;
        uint16_t VolumeLeft = ValueLeft + ( SETUP_SOURCE_NAME_SIZE + 1) * Font->CellWidth;
        uint16_t GaugeWidth = frameWidth - ( ValueLeft - frameLeft) - 1;

        uint16_t StepLeft = ValueLeft + Font->CellWidth;
        uint16_t StepGaugeWidth = frameWidth - ( StepLeft - frameLeft) - 1;

        uint16_t LowLeft = menuLeft;
        uint16_t LowValueLeft = LowLeft + 12 * Font->CellWidth;
        uint16_t HighLeft = LowValueLeft + ( 10 + 1) * Font->CellWidth;
        uint16_t HighValueLeft = HighLeft + 6 * Font->CellWidth;

        uint8_t Line = 4;

        GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
                                     SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
                                     LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Target);
        targetNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

        SetSourceLabel( &targetNameLabel, targetName, sourceFollower->TargetSignalSourceId);

        targetVolumeLabel.SetDimensions( VolumeLeft,
                                         frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
        targetVolumeLabel.SetValue( sourceFollower->Setup.TargetSource.Volume /
                                    SIGNAL_SOURCE_FOLLOWER_TARGET_SIGNAL_PER_VALUE);
        targetVolumeLabel.Display();

        Line++;

        targetGauge.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
                                   GaugeWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);
        Line++;

        GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
                                     SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
                                     LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Step);
        stepNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

        SetSourceLabel( &stepNameLabel, stepName, sourceFollower->StepSignalSourceId);

        stepVolumeLabel.SetDimensions( VolumeLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
        stepVolumeLabel.SetValue( sourceFollower->Setup.StepSource.Volume /
                                  SIGNAL_SOURCE_FOLLOWER_STEP_SIGNAL_PER_VALUE);
        stepVolumeLabel.Display();

        Line++;

        stepGauge.SetDimensions( StepLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
                                 StepGaugeWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);

        Line += 2;

        GLOBAL.SetupDisplay.Print_P( menuLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
                                     SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
                                     LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::Trigger);
        triggerNameLabel.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));

        SetSourceLabel( &triggerNameLabel, triggerName, sourceFollower->TriggerSignalSourceId);

        Line++;

        triggerGauge.SetDimensions( ValueLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
                                    GaugeWidth, SCREEN_SETUP_BASE_GAUGE_THICKNESS);

        Line++;

        GLOBAL.SetupDisplay.Print_P( LowLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
                                     SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
                                     LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::TriggerLowLimit);

        triggerLowLimitLabel.SetDimensions( LowValueLeft,
                                            frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT));
        triggerLowLimitLabel.SetValue( sourceFollower->Setup.TriggerLowLimit /
                                      SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE);
        triggerLowLimitLabel.Display();

        GLOBAL.SetupDisplay.Print_P( HighLeft, frameTop + ( Line * SCREEN_SETUP_BASE_LINE_HEIGHT),
                                     SCREEN_SETUP_BASE_MAIN_FONT, LCD_65K_RGB::C_White,
                                     LCD_65K_RGB::C_Black, LCD::PO_Proportional, Text::TriggerHighLimit);

        triggerHighLimitLabel.SetDimensions( HighValueLeft,
                                             frameTop + ( Line++ * SCREEN_SETUP_BASE_LINE_HEIGHT));
        triggerHighLimitLabel.SetValue( sourceFollower->Setup.TriggerHighLimit /
                                        SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE);
        triggerHighLimitLabel.Display();
    }

    virtual void update( void)
    {
        Screen_Setup_Source_Base::update();

        targetGauge.Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
                             Signal_Utility::GetVolumizedSourceValue(
                                 &( GLOBAL.SignalProcessor), sourceFollower->TargetSignalSourceId,
                                 SIGNAL_SOURCE_FOLLOWER_TARGET_100_PERCENT_VALUE,
                                 sourceFollower->Setup.TargetSource.Volume));
        stepGauge.Display( SIGNAL_NEUTRAL_VALUE, SIGNAL_MAXIMUM_VALUE, UTILITY_Absolute(
                               Signal_Utility::GetVolumizedSourceValue(
                                   &( GLOBAL.SignalProcessor), sourceFollower->StepSignalSourceId,
                                   SIGNAL_SOURCE_FOLLOWER_STEP_100_PERCENT_VALUE,
                                   sourceFollower->Setup.StepSource.Volume)));
        triggerGauge.Display( SIGNAL_MINIMUM_VALUE, SIGNAL_MAXIMUM_VALUE,
                              GLOBAL.SignalProcessor.GetSourceValue(
                                sourceFollower->TriggerSignalSourceId));
    }

    virtual bool processMenu( DoMenuResult Result)
    {
        switch( Result)
        {
            case DMR_Selected :
            {
                bool ValueChanged = false;

                switch( currentMenuEntry)
                {
                    case 4 :
                    {
                        bool SourceChanged = GUI_Setup_Select::DoSourceSelect(
                            &( sourceFollower->TargetSignalSourceId),
                            &( sourceFollower->Setup.TargetSource.Source), &menuMarker,
                            &targetNameLabel, NULL, targetName, this, &staticUpdate, false,
                            source->GetLevel());

                        bool VolumeChanged = GUI_Setup_Select::DoSelect16(
                            &( sourceFollower->Setup.TargetSource.Volume), SIGNAL_MINIMUM_VALUE,
                            SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_FOLLOWER_TARGET_SIGNAL_PER_VALUE,
                            &menuMarker, &targetVolumeLabel, this, &staticUpdate, &updateTargetVolume);

                        ValueChanged = SourceChanged || VolumeChanged;
                    }
                    break;

                    case 6 :
                    {
                        bool SourceChanged = GUI_Setup_Select::DoSourceSelect(
                            &( sourceFollower->StepSignalSourceId),
                            &( sourceFollower->Setup.StepSource.Source), &menuMarker, &stepNameLabel,
                            NULL, stepName, this, &staticUpdate, false, source->GetLevel());

                        bool VolumeChanged = GUI_Setup_Select::DoSelect16(
                            &( sourceFollower->Setup.StepSource.Volume), SIGNAL_NEUTRAL_VALUE,
                            SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_FOLLOWER_STEP_SIGNAL_PER_VALUE,
                            &menuMarker, &stepVolumeLabel, this, &staticUpdate, &updateStepVolume);

                        ValueChanged = SourceChanged || VolumeChanged;
                    }
                    break;

                    case 9 :
                    {
                        ValueChanged = GUI_Setup_Select::DoSourceSelect(
                            &( sourceFollower->TriggerSignalSourceId),
                            &( sourceFollower->Setup.TriggerSource), &menuMarker,
                            &triggerNameLabel, NULL, triggerName, this, &staticUpdate, false,
                            source->GetLevel());
                    }
                    break;

                    case 11 :
                    {
                        bool LowChanged = GUI_Setup_Select::DoSelect16(
                            &( sourceFollower->Setup.TriggerLowLimit), SIGNAL_MINIMUM_VALUE,
                            SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE,
                            &menuMarker, &triggerLowLimitLabel, this, &staticUpdate,
                            &updateTriggerLimit);

                        bool HighChanged = GUI_Setup_Select::DoSelect16(
                            &( sourceFollower->Setup.TriggerHighLimit), SIGNAL_MINIMUM_VALUE,
                            SIGNAL_MAXIMUM_VALUE, SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE,
                            &menuMarker, &triggerHighLimitLabel, this, &staticUpdate,
                            &updateTriggerLimit);

                        ValueChanged = HighChanged || LowChanged;
                    }
                    break;

                    default :
                    {
                        return( Screen_Setup_Source_Base::processMenu( Result));
                    }
                    break;
                }

                // Follower new values.
                if( ValueChanged)
                {
                    GLOBAL.SetupService.SetSourceFollower( setupSourceId, &( sourceFollower->Setup));
                }
            }
            break;

            case DMR_Changed :	break;

            default : break;
        }

        return( true);
    }

    static void updateTargetVolume( void* Object, GUI_Setup_Label* Label, int16_t Value)
    {
        // Refresh label.
        (( Screen_Setup_Source_Follower*) Object)->
            updateVolume( Label, Value, SIGNAL_SOURCE_FOLLOWER_TARGET_SIGNAL_PER_VALUE);
    }

    static void updateStepVolume( void* Object, GUI_Setup_Label* Label, int16_t Value)
    {
        // Refresh label.
        (( Screen_Setup_Source_Follower*) Object)->
            updateVolume( Label, Value, SIGNAL_SOURCE_FOLLOWER_STEP_SIGNAL_PER_VALUE);
    }

    static void updateTriggerLimit( void* Object, GUI_Setup_Label* Label, int16_t Value)
    {
        // Refresh label.
        (( Screen_Setup_Source_Follower*) Object)->
            updateVolume( Label, Value, SIGNAL_SOURCE_FOLLOWER_TRIGGER_SIGNAL_PER_VALUE);
    }

    void updateVolume( GUI_Setup_Label* Label, int16_t Value, int16_t SignalPerValue)
    {
        // Refresh label.
        Label->SetValue( Value / SignalPerValue);
        Label->Display();
    }

  public:
    Screen_Setup_Source_Follower( uint8_t SignalSourceId)
        : Screen_Setup_Source_Base( SignalSourceId, 0b101001010101, Text::Follower)
        , sourceFollower( NULL)
    {
        sourceFollower = &( source->Body.Follower);

        targetVolumeLabel.SetText_P( Text::FourDigitPercentFormat);
        targetVolumeLabel.SetOptions( GUI_Setup_Label::O_Fixed);

        targetGauge.SetOptions(( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
                                                            GUI_Setup_Gauge::O_DualPercentage |
                                                            GUI_Setup_Gauge::O_CenterLine |
                                                            GUI_Setup_Gauge::O_Marker));

        stepVolumeLabel.SetText_P( Text::FourDigitPercentFormat);
        stepVolumeLabel.SetOptions( GUI_Setup_Label::O_Fixed);

        stepGauge.SetOptions(( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
                                                          GUI_Setup_Gauge::O_SinglePercentage |
                                                          GUI_Setup_Gauge::O_Marker));

        triggerGauge.SetOptions(( GUI_Setup_Gauge::Options)( GUI_Setup_Gauge::O_Percentage |
                                                             GUI_Setup_Gauge::O_DualPercentage |
                                                             GUI_Setup_Gauge::O_CenterLine |
                                                             GUI_Setup_Gauge::O_Marker));

        triggerHighLimitLabel.SetText_P( Text::FourDigitPercentFormat);
        triggerHighLimitLabel.SetOptions( GUI_Setup_Label::O_Fixed);
    }
};
