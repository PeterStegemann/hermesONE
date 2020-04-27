// Copyright 2009 Peter Stegemann

#ifndef SYSTEM_H
#define SYSTEM_H

#define SYSTEM_SERIAL_ID		0

// UBRR = ( Frequency / 16 * Baudrate) - 1
//
// ( 14745600 / ( 16 * 115200)) - 1 = 7
// ( 14745600 / ( 16 * 9600)) - 1 = 95
// ( 12902400 / ( 16 * 115200)) - 1 = 6
// ( 11059200 / ( 16 * 115200)) - 1 = 5
// ( 9216000 / ( 16 * 115200)) - 1 = 4
// ( 9216000 / ( 16 *  76800)) - 1 = 6.5
// ( 9216000 / ( 16 *  57600)) - 1 = 9
// ( 9216000 / ( 16 *  38400)) - 1 = 14
// ( 9216000 / ( 16 *  19200)) - 1 = 29
// ( 9216000 / ( 16 *   9600)) - 1 = 59
#if( SYSTEM_VERSION == SYSTEM_VERSION_V0)
	#define SERIAL_0_UBBR_VALUE			4
#elif( SYSTEM_VERSION == SYSTEM_VERSION_V1)
	#define SERIAL_0_UBBR_VALUE			7
#else
	#error "Unknown system version."
#endif

#define SERIAL_0_UBBR			UBRR0
#define SERIAL_0_UDR			UDR0
#define SERIAL_0_UCSRA			UCSR0A
#define SERIAL_0_UCSRB			UCSR0B
#define SERIAL_0_UCSRC			UCSR0C
#define SERIAL_0_RXC			RXC0
#define SERIAL_0_UDRE			UDRE0

// Single speed
#define SERIAL_0_UCSRA_VALUE	( 0 << U2X0)
// No interrupts, Enable receiving & sending, 8 bit data.
#define SERIAL_0_UCSRB_VALUE	( 0 << RXCIE0) | ( 0 << TXCIE0) | ( 0 << UDRIE0) | ( 1 << RXEN0) | ( 1 << TXEN0) | ( 0 << UCSZ02)
// Asynchronous, No parity, 1 stop bit, 8 bit data
#define SERIAL_0_UCSRC_VALUE	( 0 << UMSEL01) | ( 0 << UMSEL00) | ( 0 << UPM01) | ( 0 << UPM00) | ( 0 << USBS0) | ( 1 << UCSZ01) | ( 1 << UCSZ00)

#endif
