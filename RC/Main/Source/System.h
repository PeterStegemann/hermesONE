// Copyright 2009 Peter Stegemann

#ifndef SYSTEM_H
#define SYSTEM_H

//#define SYSTEM_FRAME_DELAY				100		// 10 frames per second.
#define SYSTEM_LONG_PRESS_DURATION		250		// Long press quarter of a second.
#define SYSTEM_STORE_MODIFIED_DELAY		5		// Every 5 seconds.

#define SYSTEM_EEPROM_TYPE_AT24C512		0
#define SYSTEM_EEPROM_TYPE_FM24C512		1
#define SYSTEM_EEPROM_TYPE_AT24C1024	2

#if( SYSTEM_VERSION == SYSTEM_VERSION_MINI_V1)
	#define SYSTEM_EEPROM_TYPE			SYSTEM_EEPROM_TYPE_AT24C512
#elif( SYSTEM_VERSION == SYSTEM_VERSION_V1)
	#define SYSTEM_EEPROM_TYPE			SYSTEM_EEPROM_TYPE_FM24C512
#else
	#error "Unknown system version."
#endif

// UBRR = ( Frequency / 16 * Baudrate) - 1
//
// ( 14745600 / ( 16 * 115200)) - 1 = 7
// ( 14745600 / ( 16 * 76800)) - 1 = 11
// ( 14745600 / ( 16 * 57600)) - 1 = 15
// ( 14745600 / ( 16 * 38400)) - 1 = 23
// ( 14745600 / ( 16 * 19200)) - 1 = 47
// ( 14745600 / ( 16 * 9600)) - 1 = 95
#define SERIAL_0_UBBR_VALUE				7
#define SERIAL_1_UBBR_VALUE				7

#define SYSTEM_SERIAL_ID				0
#define OLED_SERIAL_ID					1

#define SERIAL_0_UBBR				UBRR0
#define SERIAL_0_UDR				UDR0
#define SERIAL_0_UCSRA				UCSR0A
#define SERIAL_0_UCSRB				UCSR0B
#define SERIAL_0_UCSRC				UCSR0C
#define SERIAL_0_RXC				RXC0
#define SERIAL_0_UDRE				UDRE0

// Single speed
#define SERIAL_0_UCSRA_VALUE		( 0 << U2X0)
// No interrupts, Enable receiving & sending, 8 bit data.
#define SERIAL_0_UCSRB_VALUE		( 0 << RXCIE0) | ( 0 << TXCIE0) | ( 0 << UDRIE0) | ( 1 << RXEN0) | ( 1 << TXEN0) | ( 0 << UCSZ02)
// Asynchronous, No parity, 1 stop bit, 8 bit data
#define SERIAL_0_UCSRC_VALUE		( 0 << UMSEL01) | ( 0 << UMSEL00) | ( 0 << UPM01) | ( 0 << UPM00) | ( 0 << USBS0) | ( 1 << UCSZ01) | ( 1 << UCSZ00)

#define SERIAL_1_UBBR				UBRR1
#define SERIAL_1_UDR				UDR1
#define SERIAL_1_UCSRA				UCSR1A
#define SERIAL_1_UCSRB				UCSR1B
#define SERIAL_1_UCSRC				UCSR1C
#define SERIAL_1_RXC				RXC1
#define SERIAL_1_UDRE				UDRE1

// Single speed
#define SERIAL_1_UCSRA_VALUE		( 0 << U2X1)
// No interrupts, Enable receiving & sending, 8 bit data.
#define SERIAL_1_UCSRB_VALUE		( 0 << RXCIE1) | ( 0 << TXCIE1) | ( 0 << UDRIE1) | ( 1 << RXEN1) | ( 1 << TXEN1) | ( 0 << UCSZ12)
// Asynchronous, No parity, 1 stop bit, 8 bit data
#define SERIAL_1_UCSRC_VALUE		( 0 << UMSEL11) | ( 0 << UMSEL10) | ( 0 << UPM11) | ( 0 << UPM10) | ( 0 << USBS1) | ( 1 << UCSZ11) | ( 1 << UCSZ10)

#endif
