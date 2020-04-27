// Copyright 2008 Peter Stegemann

#ifndef SETUP_DEFINES_H
#define SETUP_DEFINES_H

#define SETUP_OWNER_SIZE					30

#define SETUP_MODELS						40
#define SETUP_MODELS_START					0
#define SETUP_MODELS_END					( SETUP_MODELS_START + SETUP_MODELS)
#define SETUP_MODEL_TYPES					10
#define SETUP_MODEL_TYPES_START				100
#define SETUP_MODEL_TYPES_END				( SETUP_MODEL_TYPES_START + SETUP_MODEL_TYPES)
#define SETUP_MODEL_GLOBAL					( SETUP_MODEL_NONE - 1)
#define SETUP_MODEL_NONE					0xff
#define SETUP_MODEL_TYPE_NONE				0xff

#define SETUP_MODEL_STATUS_TIMERS			2
#define SETUP_MODEL_STATUS_SOURCES			4
#define SETUP_MODEL_PROXY_SOURCES			25

#define SETUP_MODEL_NAME_SIZE				30
#define SETUP_MODEL_TYPE_NAME_SIZE			30

#define SETUP_PPM_NAME_SIZE					30

#define SETUP_SOURCES						700
#define SETUP_SOURCE_NONE					0xffff
#define SETUP_SOURCE_FIXED					0xfffe

#define SETUP_SOURCE_NAME_SIZE				25
#define SETUP_CHANNEL_NAME_SIZE				SETUP_SOURCE_NAME_SIZE

#define SETUP_STATUS_TIMER_TOP				0
#define SETUP_STATUS_TIMER_BOTTOM			1

#define SETUP_STATUS_SOURCE_LEFT_SIDE		0
#define SETUP_STATUS_SOURCE_LEFT_BOTTOM		1
#define SETUP_STATUS_SOURCE_RIGHT_SIDE		2
#define SETUP_STATUS_SOURCE_RIGHT_BOTTOM	3

#endif
