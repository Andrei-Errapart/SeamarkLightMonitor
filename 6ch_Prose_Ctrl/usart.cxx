#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "usart.h"
#include "CBuffer.h"


uint8_t reset_seq_cnt;
uint8_t reset_seq_tmo;


/*****************************************************/
void usart_Init(void)
/*****************************************************/
{
	reset_seq_cnt = reset_seq_tmo = 0;

	UCSRA = 0x00;
	UCSRB = _BV(RXCIE) | _BV(RXEN) | _BV(TXEN) | _BV(UDRIE);
	UCSRC = _BV(URSEL) | _BV(UCSZ1) | _BV(UCSZ0);

	UBRRH = UBRR_RELOAD >> 8;
	UBRRL = UBRR_RELOAD;
}

/*****************************************************/
void reset_seq_wdt(void)
/*****************************************************/
{
	if(reset_seq_tmo)
	{
		reset_seq_tmo--;
		if(!reset_seq_tmo)
			reset_seq_cnt = 0;
	}
}

/*****************************************************/
ISR (USART_RXC_vect)
/*****************************************************/
{
	uint8_t status, data;

	status = UCSRA;
	data = UDR;
	
	if(!(status & ((1<<FE) | (1<<PE) | (1<<DOR))))
	{

		if(data == 0x55)
		{
			reset_seq_tmo = 50;
			reset_seq_cnt--;
			if(!reset_seq_cnt)
			{
				asm volatile("cli\n\t"
							 "ldi r30, 0xA2\n\t"
							 "ldi r31, 0x0F\n\t"
							 "ijmp\n\t"
							 ::);

			}
		}
		else
			reset_seq_cnt = reset_seq_tmo = 0;
	}
}

/*****************************************************************************/
static CBuffer<uint8_t, 128>	tx_buffer;

/*****************************************************************************/
/** Data register empty: more work to do. */
ISR(USART_UDRE_vect)
{
	uint8_t	txchar;
	if (tx_buffer.Pop(txchar)) {
		UDR = txchar;
	} else {
		// Disable DataRegisterEmpty interrupt.
		UCSRB &= ~_BV(UDRIE);
	}
}

/*****************************************************************************/
#define	ENABLE_DATA_REGISTER_EMPTY() do { UCSRB |= _BV(UDRIE); } while(0)

/*****************************************************************************/
static void
uart_send(	const uint8_t	c)
{
	tx_buffer.Push(c);
	ENABLE_DATA_REGISTER_EMPTY();
}

/*****************************************************************************/
void
uart_send_P(	PGM_P	s)
{
	for (;; ++s) {
		const uint8_t	c = pgm_read_byte(s);
		if (c==0) {
			break;
		} else {
			tx_buffer.Push(c);
		}
	}

	ENABLE_DATA_REGISTER_EMPTY();
}

/*****************************************************************************/
void
uart_send(	const char*	s)
{
	for (;; ++s) {
		const uint8_t	c = *s;
		if (c==0) {
			break;
		} else {
			tx_buffer.Push(c);
		}
	}

	ENABLE_DATA_REGISTER_EMPTY();
}

/*****************************************************************************/
void
uart_send_crlf()
{
	uart_send_P(PSTR("\r\n"));
}

/*****************************************************************************/
static const char	hexchars[]		PROGMEM	= "0123456789ABCDEF";

/*****************************************************************************/
static void
uart_send_u16(	const uint16_t	x)
{
	char		buffer[6];
	uint8_t	i;

	for (i=0; i<sizeof(buffer); ++i) {
		buffer[i] = 0;
	}
	utoa(x, buffer, 10);
	for (i=0; i<sizeof(buffer) && buffer[i]!=0; ++i) {
		uart_send(buffer[i]);
	}
}

/*****************************************************************************/
void println_hex08(
	PGM_P					prefix,
	const uint8_t	x)
{
	uart_send_P(prefix);
	uart_send(':');
	uart_send(pgm_read_byte(hexchars + ((x >>  4) & 0x0F)));
	uart_send(pgm_read_byte(hexchars + ((x >>  0) & 0x0F)));
	uart_send_crlf();
}

/*****************************************************************************/
void println_u16(
	PGM_P						prefix,
	const uint16_t	x)
{
	uart_send_P(prefix);
	uart_send(':');
	uart_send_u16(x);
	uart_send_crlf();
}

/*****************************************************************************/
void println_P( PGM_P					s)
{
	uart_send_P(s);
	uart_send_crlf();
}

