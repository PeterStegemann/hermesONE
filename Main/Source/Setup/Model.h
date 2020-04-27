// Copyright 2008 Peter Stegemann

#ifndef SETUP_MODEL_H
#define SETUP_MODEL_H

#include "Channel.h"
#include "Defines.h"

#include "Signal/Service.h"

class Setup_Model
{
  public:
	uint8_t State;
	uint8_t SelectedTypeId;

	uint8_t RFMode;

	char Name[ SETUP_MODEL_NAME_SIZE];

	uint16_t StatusTimer[ SETUP_MODEL_STATUS_TIMERS];
	uint16_t StatusSource[ SETUP_MODEL_STATUS_SOURCES];

	Setup_Channel Channel[ SIGNAL_PPM_CHANNELS];
	char ChannelName[ SIGNAL_PPM_CHANNELS][ SETUP_CHANNEL_NAME_SIZE];

	Setup_Source_Tupel ProxyReference[ SETUP_MODEL_PROXY_SOURCES];
};

#endif
