// Copyright 2008 Peter Stegemann

#pragma once

#include "Gauge.h"
#include "Label.h"
#include "Marker.h"

#include "Setup/Defines.h"

#include "AVR/Source/Types.h"

class GUI_Setup_Select
{
  private:
    static void select( GUI_Setup_Marker* Marker, GUI_Setup_Label* Label)
    {
        Marker->ForegroundColor = LCD_65K_RGB::C_Red;
        Marker->Display();
    }

    static void unSelect( GUI_Setup_Marker* Marker, GUI_Setup_Label* Label)
    {
        Marker->ForegroundColor = LCD_65K_RGB::C_WarmYellow;
        Marker->Display();

        Label->SetInverted( false);
        Label->Display();
    }

    static void blink( GUI_Setup_Label* Label, uint16_t* Millis, bool* Inverted)
    {
        if( Blink( Millis, Inverted))
        {
            Label->SetInverted( *Inverted);
            Label->Display();
        }
    }

    static int8_t adjustToBoundaries8( int8_t NewValue, int8_t LowerLimit, int8_t UpperLimit, int8_t StepWidth)
    {
        while( true)
        {
            if( NewValue > UpperLimit)
            {
                NewValue -= (( int16_t) UpperLimit - ( int16_t) LowerLimit) + StepWidth;
            }
            else if( NewValue < LowerLimit)
            {
                NewValue += (( int16_t) UpperLimit - ( int16_t) LowerLimit) + StepWidth;
            }
            else
            {
                return( NewValue);
            }
        }
    }

    static int16_t adjustToBoundaries16( int16_t NewValue, int16_t LowerLimit, int16_t UpperLimit, int16_t StepWidth)
    {
        while( true)
        {
            if( NewValue > UpperLimit)
            {
                NewValue -= (( int32_t) UpperLimit - ( int32_t) LowerLimit) + StepWidth;
            }
            else if( NewValue < LowerLimit)
            {
                NewValue += (( int32_t) UpperLimit - ( int32_t) LowerLimit) + StepWidth;
            }
            else
            {
                return( NewValue);
            }
        }
    }

    static int16_t adjustToBoundariesTime( int16_t NewValue, int16_t LowerLimit, int16_t UpperLimit)
    {
        if( NewValue > UpperLimit)
        {
            NewValue = UpperLimit;
        }
        else if( NewValue < LowerLimit)
        {
            NewValue = LowerLimit;
        }

        return( NewValue);
    }

  public:
    static bool Blink( uint16_t* Millis, bool* Inverted)
    {
        uint16_t NewMillis = GLOBAL.InterruptService.GetMillis();
        int32_t Diff = NewMillis;
        Diff -= *Millis;

        if( Diff < 0)
        {
            Diff += INTERRUPT_SERVICE_MAXIMUM_MILLIS;
        }

        if( Diff > GUI_SETUP_BLINK_DELAY)
        {
            *Millis = NewMillis;
            *Inverted = !*Inverted;

            return( true);
        }

        return( false);
    }

    static bool DoSelect8
    (
        int8_t* Value, int8_t LowerLimit, int8_t UpperLimit, int8_t StepWidth,
        GUI_Setup_Marker* Marker, GUI_Setup_Label* Label, void* Object,
        void ( *Update)( void* Object), void ( *UpdateLabel)( void* Object, GUI_Setup_Label* Label, int8_t Value)
    )
    {
        select( Marker, Label);

        // Saves some write cycles with this flag.
        bool ValueChanged = false;

        uint16_t Millis = 0;
        bool Inverted = false;
        bool Exit = false;

        while( Exit == false)
        {
            int16_t NewValue = *Value;

            // Align to step width.
            NewValue -= NewValue % StepWidth;

            if( Update != NULL)
            {
                Update( Object);
            }

            int8_t RotarySelect;
            uint8_t RotaryButton;

            GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

            if( RotaryButton > 0)
            {
                Exit = true;
            }

            // Set new position.
            if( RotarySelect != 0)
            {
                ValueChanged = true;

                NewValue += RotarySelect * StepWidth;
                NewValue = adjustToBoundaries8( NewValue, LowerLimit, UpperLimit, StepWidth);

                *Value = NewValue;

                // Refresh label.
                UpdateLabel( Object, Label, *Value);
            }

            // Blink cursor.
            blink( Label, &Millis, &Inverted);
        }

        unSelect( Marker, Label);

        return( ValueChanged);
    }

    static bool DoSelect16
    (
        int16_t* Value, int16_t LowerLimit, int16_t UpperLimit, int16_t StepWidth,
        GUI_Setup_Marker* Marker, GUI_Setup_Label* Label, void* Object,
        void ( *Update)( void* Object), void ( *UpdateLabel)( void* Object, GUI_Setup_Label* Label, int16_t Value)
    )
    {
        select( Marker, Label);

        // Saves some write cycles with this flag.
        bool ValueChanged = false;

        uint16_t Millis = 0;
        bool Inverted = false;
        bool Exit = false;

        while( Exit == false)
        {
            int32_t NewValue = *Value;

            // Align to step width.
            NewValue -= NewValue % StepWidth;

            if( Update != NULL)
            {
                Update( Object);
            }

            int8_t RotarySelect;
            uint8_t RotaryButton;

            GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

            if( RotaryButton > 0)
            {
                Exit = true;
            }

            // Set new position.
            if( RotarySelect != 0)
            {
                ValueChanged = true;

                NewValue += RotarySelect * StepWidth;
                NewValue = adjustToBoundaries16( NewValue, LowerLimit, UpperLimit, StepWidth);

                *Value = NewValue;

                // Refresh label.
                UpdateLabel( Object, Label, *Value);
            }

            // Blink cursor.
            blink( Label, &Millis, &Inverted);
        }

        unSelect( Marker, Label);

        return( ValueChanged);
    }

	static bool DoSelectTime
	(
	    int16_t* Value, int16_t LowerLimit, int16_t UpperLimit, uint8_t StepWidth,
        GUI_Setup_Marker* Marker, GUI_Setup_Label* Label, void* Object,
        void ( *UpdateLabel)( void* Object, GUI_Setup_Label* Label, int16_t Value)
    )
    {
        select( Marker, Label);

        // Saves some write cycles with this flag.
        bool ValueChanged = false;

        uint16_t Millis = 0;
        bool Inverted = false;
        bool Exit = false;

        while( Exit == false)
        {
            int32_t NewValue = *Value;

            // Align to step width.
            NewValue -= NewValue % StepWidth;

            int8_t RotarySelect;
            uint8_t RotaryButton;

            GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

            if( RotaryButton > 0)
            {
                Exit = true;
            }

            // Set new position.
            if( RotarySelect != 0)
            {
                ValueChanged = true;

                NewValue += RotarySelect * StepWidth;
                NewValue = adjustToBoundariesTime( NewValue, LowerLimit, UpperLimit);

                *Value = NewValue;

                // Refresh label.
                UpdateLabel( Object, Label, *Value);
            }

            // Blink cursor.
            blink( Label, &Millis, &Inverted);
        }

        unSelect( Marker, Label);

        return( ValueChanged);
    }

	// Gauge may be null. T_Empty means "All"
	static bool DoSourceSelect
	(
	    uint8_t* SignalSourceId, uint16_t* SetupSourceId,
	    GUI_Setup_Marker* Marker, GUI_Setup_Label* Label, GUI_Setup_Gauge* Gauge,
        char SourceName[ SETUP_SOURCE_NAME_SIZE + 1], void* Object, void ( *Update)( void* Object), bool HasFixed,
        Signal_Source_Source::Level SourceLevel, Signal_Source_Source::Type SourceType = Signal_Source_Source::T_Empty
    )
    {
        select( Marker, Label);

        // Saves some write cycles with this flag.
        bool ValueChanged = false;

        uint16_t Millis = 0;
        bool Inverted = false;
        bool Exit = false;

        while( Exit == false)
        {
            if( Update != NULL)
            {
                Update( Object);
            }

            int8_t RotarySelect;
            uint8_t RotaryButton;

            GLOBAL.InputService.GetRotary( &RotarySelect, &RotaryButton);

            if( RotaryButton > 0)
            {
                Exit = true;
            }

            // Set new position.
            if( RotarySelect != 0)
            {
                ValueChanged = true;

                *SignalSourceId = GLOBAL.SignalProcessor.FindNextSource
                (
                    *SignalSourceId, RotarySelect, HasFixed, SourceLevel, SourceType
                );

                // Refresh label.
                Label->Clear();

                if( Gauge != NULL)
                {
                    Gauge->Clear();
                }

                if( *SignalSourceId == SIGNAL_SOURCE_NONE)
                {
                    *SetupSourceId = SETUP_SOURCE_NONE;

                    Label->SetText_P( Text::None);
                }
                else if( *SignalSourceId == SIGNAL_SOURCE_FIXED)
                {
                    *SetupSourceId = SETUP_SOURCE_FIXED;

                    Label->SetText_P( Text::Fixed);
                }
                else
                {
                    const Signal_Source_Source* SignalSource = GLOBAL.SignalProcessor.GetSource( *SignalSourceId);

                    *SetupSourceId = SignalSource->GetSetupSourceId();

                    GLOBAL.SetupService.GetSourceName
                    (
                        SignalSource->GetSetupSourceId(), SourceName, SETUP_SOURCE_NAME_SIZE + 1
                    );
                    Label->SetText( SourceName);
                }

                Label->Display();
            }

            // Blink cursor.
            blink( Label, &Millis, &Inverted);
        }

        unSelect( Marker, Label);

        return( ValueChanged);
    }
};
