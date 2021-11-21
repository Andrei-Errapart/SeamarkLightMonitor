#ifndef _USART_H
#define _USART_H

#include <stdint.h>				/* uint8_t */
#include <avr/pgmspace.h>	/* Program memory space. */


#define BAUD_RATE 57600
#define UBRR_RELOAD ((F_CPU / (BAUD_RATE * 16)) - 1)

void usart_Init(void);
void reset_seq_wdt(void);

/**
 * Print strings to debugging output.
 */
void uart_send_P( PGM_P					s);

/**
 * Print strings to debugging output.
 */
void uart_send(		const char*		s);

/**
 * Print newline (CRLF).
 */
void uart_send_crlf();

/**
 * Print unsigned byte \c x to debugging output in hexadecimal, prefixed by \c prefix.
 * A newline (CRLF) is appended.
 */
void println_hex08(
	PGM_P					prefix,
	const uint8_t	x);


/**
 * Print unsigned short \c x to debugging output, prefixed by \c prefix.
 * A newline (CRLF) is appended.
 */
void println_u16(
	PGM_P						prefix,
	const uint16_t	x);

void println_P( PGM_P					s);

#endif
