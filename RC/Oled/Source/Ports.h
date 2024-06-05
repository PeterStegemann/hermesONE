// Copyright 2010 Peter Stegemann

#pragma once

#include "System.h"

#include <avr/io.h>

// SPI
#define SPI_PORT						PORTB
#define SPI_DDR							DDRB

#define SPI_RESET						PB0			// Reset SPI devices
#define SPI_0_SELECT					PB1			// Cable Select S6E63D6
#define SPI_DC							PB2			// Data/Command S6E63D6 RS
#define SPI_SS							PB4
#define SPI_DATA_OUTPUT					PB5			// Send Data 
#define SPI_DATA_INPUT					PB6			// Receive Data
#define SPI_CLOCK						PB7			// Clock

// LCD S6E63D6
#define LCD_S6E63D6_SPI_ID				0			// SPI Select

#define LCD_S6E63D6_SPI_PORT			PORTB
#define LCD_S6E63D6_SPI_DDR				DDRB

#define LCD_S6E63D6_SPI_RESET			PB0			// Reset
#define LCD_S6E63D6_SPI_SELECT			PB1			// Cable Select
#define LCD_S6E63D6_SPI_RS				PB2			// RS
#define LCD_S6E63D6_SPI_RW_WRB			PB7			// Read/Write

#define LCD_S6E63D6_PORT				PORTD
#define LCD_S6E63D6_DDR					DDRD

#define LCD_S6E63D6_E_RDB				PD2
#define LCD_S6E63D6_SPB					PD3
#define LCD_S6E63D6_ID_MIB				PD4
#define LCD_S6E63D6_POWER				PD5

#define LCD_S6E63D6_DATA_LOW_PORT		PORTA
#define LCD_S6E63D6_DATA_LOW_DDR		DDRA

#define LCD_S6E63D6_DATA_HIGH_PORT		PORTC
#define LCD_S6E63D6_DATA_HIGH_DDR		DDRC

// Serial communication
#define SERIAL_PORT						PORTD
#define SERIAL_DDR						DDRD
#define SERIAL_PIN						PIND

#define SERIAL_RX						PD0
#define SERIAL_TX						PD1

// Status led
#define STATUS_PORT						PORTD
#define STATUS_DDR						DDRD

#define STATUS_RED						PD6
#define STATUS_GREEN					PD7
