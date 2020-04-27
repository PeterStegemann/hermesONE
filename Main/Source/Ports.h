// Copyright 2007 Peter Stegemann

#ifndef PORTS_H
#define PORTS_H

#include "System.h"

#include <avr/io.h>

// EEPROM
#define EEPROM_PORT									PORTD
#define EEPROM_DDR									DDRD

#define EEPROM_SCL									PD0
#define EEPROM_SDA									PD1

// SPI
#define SPI_PORT									PORTB
#define SPI_DDR										DDRB

#define SPI_SS										PB0
#define SPI_CLOCK									PB1					// Clock
#define SPI_DATA_OUTPUT								PB2					// Send Data
#define SPI_RESET									PB3					// Reset SPI devices
#define SPI_DC										PB0					// Data/Command

#define SPI_0_SELECT								LCD_DOG_SELECT		// Cable Select

// LCD DOG
#define LCD_DOG_SPI_ID								0					// SPI Select

#define LCD_DOG_TIMERA								TCCR1A
#define LCD_DOG_TIMERB								TCCR1B

#define LCD_DOG_PORT								PORTB
#define LCD_DOG_DDR									DDRB

#define LCD_DOG_SELECT								PB4					// Cable Select
#define LCD_DOG_LIGHT								PB5					// Light on/off

// Beep
#define BUZZER_PORT									PORTD
#define BUZZER_DDR									DDRD

#define BUZZER_NOISE								PD4

// Input misc
#define INPUT_TIMERA								TCCR0A
#define INPUT_TIMERB								TCCR0B

#define INPUT_MENU_PORT								PORTD
#define INPUT_MENU_DDR								DDRD
#define INPUT_MENU_PIN								PIND

#define INPUT_MENU_ROTARY_BUTTON					PD5
#define INPUT_MENU_ROTARY_A							PD6
#define INPUT_MENU_ROTARY_B							PD7

#if( SYSTEM_VERSION == SYSTEM_VERSION_MINI_V1)
	#define INPUT_ANALOG_PORTS						8
#elif( SYSTEM_VERSION == SYSTEM_VERSION_V1)
	#define INPUT_ANALOG_PORTS						16
#endif

#define INPUT_ANALOG_PORT_LIGHT						6
#define INPUT_ANALOG_PORT_VOLTAGE					7

#define INPUT_ANALOG_A_D_PORT						PORTF
#define INPUT_ANALOG_A_D_DDR						DDRF
#define INPUT_ANALOG_A_D_PIN						PINF

#if( SYSTEM_VERSION == SYSTEM_VERSION_V1)
	#define INPUT_ANALOG_E_G_PORT					PORTK
	#define INPUT_ANALOG_E_G_DDR					DDRK
	#define INPUT_ANALOG_E_G_PIN					PINK
#endif

#if( SYSTEM_VERSION == SYSTEM_VERSION_MINI_V1)
	#define INPUT_DIGITAL_PORTS						16
#elif( SYSTEM_VERSION == SYSTEM_VERSION_V1)
	#define INPUT_DIGITAL_PORTS						32
#endif

#define INPUT_DIGITAL_H_I_PORT						PORTA
#define INPUT_DIGITAL_H_I_DDR						DDRA
#define INPUT_DIGITAL_H_I_PIN						PINA

#define INPUT_DIGITAL_J_K_PORT						PORTC
#define INPUT_DIGITAL_J_K_DDR						DDRC
#define INPUT_DIGITAL_J_K_PIN						PINC

#if( SYSTEM_VERSION == SYSTEM_VERSION_V1)
	#define INPUT_DIGITAL_L_M_PORT					PORTJ
	#define INPUT_DIGITAL_L_M_DDR					DDRJ
	#define INPUT_DIGITAL_L_M_PIN					PINJ

	#define INPUT_DIGITAL_N_O_PORT					PORTL
	#define INPUT_DIGITAL_N_O_DDR					DDRL
	#define INPUT_DIGITAL_N_O_PIN					PINL
#endif

// Serial communication
#define SERIAL_SYSTEM_PORT							PORTE
#define SERIAL_SYSTEM_DDR							DDRE
#define SERIAL_SYSTEM_PIN							PINE

#define SERIAL_SYSTEM_RX							PE1
#define SERIAL_SYSTEM_TX							PE0

#define SERIAL_DISPLAY_PORT							PORTD
#define SERIAL_DISPLAY_DDR							DDRD
#define SERIAL_DISPLAY_PIN							PIND

#define SERIAL_DISPLAY_RX							PD0
#define SERIAL_DISPLAY_TX							PD1

// PPM generation
#if( SYSTEM_VERSION == SYSTEM_VERSION_V1)
	#define SIGNAL_SERVICE_TIMERA					TCCR4A
	#define SIGNAL_SERVICE_PORT						PORTH
	#define SIGNAL_SERVICE_DDR						DDRH
	#define SIGNAL_SERVICE_PIN						PINH

	#define SIGNAL_SERVICE_PPM_ENABLE_1				PH2
	#define SIGNAL_SERVICE_PPM_ENABLE_2				PH3
	#define SIGNAL_SERVICE_PPM_OUTPUT_1				PH4
	#define SIGNAL_SERVICE_PPM_OUTPUT_2				PH5
#elif( SYSTEM_VERSION == SYSTEM_VERSION_MINI_V1)
	#define SIGNAL_SERVICE_TIMERA					TCCR3A
	#define SIGNAL_SERVICE_PORT						PORTE
	#define SIGNAL_SERVICE_DDR						DDRE
	#define SIGNAL_SERVICE_PIN						PINE

	#define SIGNAL_SERVICE_PPM_ENABLE_1				PE2
	#define SIGNAL_SERVICE_PPM_ENABLE_2				PE7
	#define SIGNAL_SERVICE_PPM_OUTPUT_1				PE3
	#define SIGNAL_SERVICE_PPM_OUTPUT_2				PE4
/*
	#define SIGNAL_SERVICE_PPM_ENABLE_1				PE2
	#define SIGNAL_SERVICE_PPM_ENABLE_2				PE3
	#define SIGNAL_SERVICE_PPM_OUTPUT_1				PE4
	#define SIGNAL_SERVICE_PPM_OUTPUT_2				PE5
*/
#endif

// Status
#define STATUS_SERVICE_PORT							PORTG
#define STATUS_SERVICE_DDR							DDRG

#define STATUS_SERVICE_RED							PG0
#define STATUS_SERVICE_GREEN						PG1

#endif
