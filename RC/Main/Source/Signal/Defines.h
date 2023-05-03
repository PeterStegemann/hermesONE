// Copyright 2007 Peter Stegemann

#pragma once

// Number of sources per model.
#define SIGNAL_SOURCES				50
// No source set.
#define SIGNAL_SOURCE_NONE			0xff
// Fixed value.
#define SIGNAL_SOURCE_FIXED			0xfe

#define SIGNAL_MAXIMUM_VALUE		30000L
#define SIGNAL_NEUTRAL_VALUE		0L
#define SIGNAL_MINIMUM_VALUE		( - SIGNAL_MAXIMUM_VALUE)
#define SIGNAL_VALUE_RANGE			( SIGNAL_MAXIMUM_VALUE - SIGNAL_MINIMUM_VALUE)
