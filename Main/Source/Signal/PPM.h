// Copyright 2007 Peter Stegemann

#ifndef SIGNAL_PPM_H
#define SIGNAL_PPM_H

#include "AVR/Source/Types.h"

#define SIGNAL_PPM_CHANNELS					9

#define SIGNAL_PPM_CENTER_MINIMUM			-5
#define SIGNAL_PPM_CENTER_MAXIMUM			+5

class Signal_PPM
{
  private:
	// Id of ppm generator.
	uint8_t id;

	// RF enable state.
	volatile bool rfEnabled;

	// Operate in inverted ppm mode?
	volatile bool inverted;

	// Center value for ppm signal.
	uint16_t center;

	// This array holds all channel values that have to be transformed and copied into the signals
	// array.
	volatile uint16_t channel[ SIGNAL_PPM_CHANNELS];
	// This has to be set to true when the channels are ready to be used by the interrupt routine.
	// When the routine is done with the data, it sets the flag back to false, so new values can be
	// set.
	volatile bool channelsValid;
	// This array holds the mappings between signal channels and output channels for adaption to
	// different rf modules.
	volatile uint8_t channelMapping[ SIGNAL_PPM_CHANNELS];
	// This array holds all signals that are sent out.
	uint16_t signal[ SIGNAL_PPM_CHANNELS];
	// The sync rounds up the signals to a complete frame.
	uint32_t sync;

	// Which one we're currently sending.
	uint8_t currentSignal;
	// Here we remember the next signal state.
	bool signalState;

	volatile uint8_t waited;
	volatile uint16_t sequence;
	volatile uint32_t syncCopy;

	// The bit used to toggle the signal.
	uint8_t toggleBit;
	// OCR register used.
	volatile uint16_t* ocr;

	// Try to copy a new set of channels if it is available.
	void copyChannels( void);

  public:
	Signal_PPM( uint8_t Id);

	// This is for the interrupt, not for you.
	void ProcessSignal( void);

	// Start sending loop.
	void Start( void);

	// Turn RF module on/off.
	void SetRFEnabled( bool Enabled);
	bool GetRFEnabled( void);

	// Set inverted/normal.
	void SetInverted( bool Inverted);

	// Set center. 0 is default 1.5ms which is standard for PPM, so this value is an offset in 1/10
	// ms.
	void SetCenter( int8_t Center);

	// Set channel mapping.
	void SetChannelMapping( uint8_t Index, uint8_t ChannelId);

	// Set a specific channel to a new value. The changes won't be sent out before you call
	// Update(). This one waits until the sending routine has copied the last value set.
	void SetChannel( uint8_t ChannelId, int16_t Value);
	// This returns the channel value in the internal format, be aware that this is different from
	// the value for SetChannel().
	uint16_t GetChannel( uint8_t ChannelId);
	// Returns the internal sync value.
	uint32_t GetSync( void);

	// Marks the data as beeing complete, with the next cycle these values will be used.
	void SetChannelsValid( void);

	// Returns false when the ppm engine hasn't processed the last dataset yet. In that case, calls
	// to SetChannel() would block.
	bool ReadyForData( void);

	// Internal number of waits.
	uint8_t GetWaited( void);

	// Internal sequence id.
	uint16_t GetSequence( void);
};

#endif
