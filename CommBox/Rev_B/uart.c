/*
 * vim: ts=2
 * vim: shiftwidth=2
 */

#include "project.h"

#include <avr/io.h>	/* inp, outp */
#include <avr/interrupt.h>
#include <avr/sleep.h>


static u16	syncro_countdown = 0;
/** Ticks to start the syncro impulse. */
static s32	syncro_countdown_ticks = PRECISION_TICKS_PERIOD_SYNC;

/** Number of synchronizations so far. */
static volatile u08	syncrocount = 0;

/*****************************************************************************/
#define	SENDBUFFER_SIZE	64
typedef struct {
	volatile u08	begin_index;	///< pop index into data.
	volatile u08	end_index;		///< push index into data.
	volatile u08	bit_index;		///< 0=start, 1...8=data, 9=stop, 10,11,12=trail
	volatile u08	data[SENDBUFFER_SIZE];	///< data buffer.
} SENDBUFFER;

typedef struct {
	volatile u08	tick_count;	///< Tick counter. 6 per bit.
	volatile u08	bit_index;	///< 0=idle channel 1=start, 2...9=data, 10=stop
	volatile u08	data;				///< Character to be received.
} RECEIVEBUFFER;

static SENDBUFFER	modem_sendbuffer = { 0 };
#if (0)
static SENDBUFFER	smartflasher_sendbuffer = { 0 };

static RECEIVEBUFFER modem_receivebuffer = { 0 };
#endif
static RECEIVEBUFFER smartflasher_receivebuffer = { 0 };

/** Ticks of the day. */
static volatile s32	ticksoftheday = 0;
/** Is it valid? */
static volatile bool	ticksoftheday_valid = false;

static u08	gps_buffer[128];
static volatile u08	gps_begin_index = 0;
static volatile u08	gps_end_index = 0;

/*****************************************************************************/
/** signal handler for receive complete interrupt */
SIGNAL(SIG_USART_RECV)      
{
	for (;;) {
		const u08 flags = UCSR0A;
		if (flags & _BV(RXC0)) {
			const u08	udr = UDR0; /* read byte from receive register */
			if ((flags & _BV(FE0))==0 && (flags & _BV(DOR0))==0 && (flags & _BV(UPE0))==0) {
				const u08	next_end_index = (gps_end_index + 1) % (sizeof(gps_buffer) / sizeof(gps_buffer[0]));
				if (next_end_index != gps_begin_index) {
					gps_buffer[gps_end_index] = udr;
					gps_end_index = next_end_index;
				}
			}
		} else {
			break;
		}
	}
}

/*****************************************************************************/
/** signal handler for usb transmit buffer empty */
SIGNAL(SIG_USART_DATA)      
{
}

SIGNAL(SIG_USART_TRANS)      
{
}

/*****************************************************************************/
static inline void
emit_bitstream(	SENDBUFFER*		buffer,
								const u08	bitmask)
{
	if (buffer->begin_index != buffer->end_index) {
		const u08	c = buffer->data[buffer->begin_index];
		const u08	bit_index = buffer->bit_index;

		++(buffer->bit_index);

		switch (bit_index) {
			case 0:
				// START bit.
				PORTD |= bitmask;
				break;
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				// DATA bit.
				if ( (c & _BV(bit_index-1)) != 0) {
					// bit 1
					PORTD &= ~bitmask;
				} else {
					// bit 0
					PORTD |= bitmask;
				}
				break;
			case 9:
				// STOP bit.
				PORTD &= ~bitmask;
			case 10: // still stop 
			case 11: // still stop.
				break;
			default:
				// NEXT.
				buffer->bit_index = 0;
				buffer->begin_index = (buffer->begin_index + 1) % SENDBUFFER_SIZE;
				break;
		}
	}
}

/*****************************************************************************/
static inline s16
process_pin(	RECEIVEBUFFER*	buffer,
							const bool			pin)
{
	if (buffer->bit_index==0) {
		// Got start bit?
	 	if (pin) {
			buffer->tick_count = 0;
			buffer->bit_index = 1; // start bit
			buffer->data = 0;
		}
	} else {
		// Count ticks.
		const u08	tick_count = ++(buffer->tick_count);
		if (tick_count==1) {
			// Middle of the bit.
			const u08	bit_index = buffer->bit_index - 2;
			if (!pin && bit_index<8) {
				buffer->data |= (1 << bit_index);
			}
		} else if (tick_count>=3) {
			// End of bit.
			buffer->tick_count = 0;
			if (++(buffer->bit_index) >= 11) {
				buffer->bit_index = 0; // idle channel.
				return buffer->data;
			}
		}
	}

	return -1;
}

/*****************************************************************************/
SIGNAL(SIG_OUTPUT_COMPARE1A)
{
	static u08	uart_counter = 0;
	s16	r;

	if (++ticksoftheday >= PRECISION_TICKS_PER_DAY) {
		ticksoftheday = 0;
	}

	if (syncro_countdown_ticks==PRECISION_TICKS_GPS_LEAD) {
		gpspower_on();
	}

	if (--syncro_countdown_ticks <= 0) {
		syncro_countdown_ticks = PRECISION_TICKS_PERIOD_SYNC;
		if (ticksoftheday_valid) {
			syncro_countdown = PRECISION_TICKS_PER_SYNC;
			syncro_on();
			++syncrocount;
		}
	} else {
		if (syncro_countdown>0 && --syncro_countdown==0) {
			syncro_off();
		}
	}

	// UART TX.
	++uart_counter;
	if (uart_counter == 3) {
		emit_bitstream(&modem_sendbuffer,					_BV(UART1_PIN_TX));
		// emit_bitstream(&smartflasher_sendbuffer,	_BV(UART2_PIN_TX));
		uart_counter = 0;
	}

	// Level change on UART2/smartflasher
	r = process_pin(&smartflasher_receivebuffer, (PIND & _BV(UART2_PIN_RX))!=0);
	if (r>=0) {
		smartflasher_receive(r);
	}

#if defined(TEST_SYNC)
	// approx. 2 second period.
	if (((uint16_t)ticksoftheday) & 2048) {
		MODEMPOWER_PORT |= _BV(MODEMPOWER_SIGNAL_BIT);
	} else {
		MODEMPOWER_PORT &= ~_BV(MODEMPOWER_SIGNAL_BIT);
	}
#endif
}

/*****************************************************************************/
/** Invert UART2_PIN_RX / SmartFlasher (PD2) to UART1_PIN_TX (PD5)
 * Do it only when signal is low.
 */
SIGNAL(SIG_INTERRUPT0)
{
	if (modem_sendbuffer.begin_index == modem_sendbuffer.end_index) {
		if (UART2_PIN & _BV(UART2_PIN_RX)) {
			UART1_PORT &= ~_BV(UART1_PIN_TX);
		} else {
			UART1_PORT |= _BV(UART1_PIN_TX);
		}
	}
}

/*****************************************************************************/
/** Forward UART1_PIN_RX / Modem to UART2_PIN_TX */
SIGNAL(SIG_INTERRUPT1)
{
	if (UART1_PIN & _BV(UART1_PIN_RX)) {
		UART2_PORT |= _BV(UART2_PIN_TX);
	} else {
		UART2_PORT &= ~_BV(UART2_PIN_TX);
	}
}

/*****************************************************************************/
static void
send(	SENDBUFFER*		buffer,
			const u08			c)
{
	const u08	next_index = (buffer->end_index + 1) % SENDBUFFER_SIZE;
	if (next_index == buffer->begin_index) {
		// don't wait if buffer full.
		return;
	}

	buffer->data[buffer->end_index] = c;
	buffer->end_index = next_index;
}

/*****************************************************************************/
void modem_send(					const u08	c)
{
	send(&modem_sendbuffer, c);
}

/*****************************************************************************/
void modem_send_P(			PGM_P	s)
{
	for (;;) {
		const char c = pgm_read_byte(s++);
		if (c==0) {
			break;
		}
		modem_send(c);
	}
}

/*****************************************************************************/
static uint8_t
hexchar_of_int(		const uint8_t	ii)
{
	return ii<10 ? ii + '0' : ii + 'A' - 10;
}

/*****************************************************************************/
void modem_send_hex(			const uint8_t	x)
{
	modem_send(hexchar_of_int(x>>4));
	modem_send(hexchar_of_int(x&0x0F));
}

/*****************************************************************************/
s16 gps_receive()
{
	if (gps_begin_index == gps_end_index) {
		return -1;
	} else {
		const u08	r = gps_buffer[gps_begin_index];
		gps_begin_index = (gps_begin_index + 1) % (sizeof(gps_buffer) / sizeof(gps_buffer[0]));
		return r;
	}
}

/*****************************************************************************/
s32	getticksoftheday()
{
	const bool	interrupts_enabled = (SREG & 0x80) == 0;
	s32	r;
	if (interrupts_enabled) {
		cli();
		r = ticksoftheday;
		sei();
		return r;
	}
	return ticksoftheday;
}

/*****************************************************************************/
void addticksoftheday(	const s32		extra_ticks)
{
	// Are we allowed to add?
	if (ticksoftheday_valid) {
		setticksoftheday(ticksoftheday + extra_ticks);
	}
}

/*****************************************************************************/
void setticksoftheday(	const s32		ticks)
{
	const bool	interrupts_enabled = (SREG & 0x80) == 0;
	if (interrupts_enabled) {
		cli();
	}

	/* ticksoftheday. */
	ticksoftheday = (ticks + PRECISION_TICKS_PER_DAY) % PRECISION_TICKS_PER_DAY;
	if (ticksoftheday >= 0) {
		ticksoftheday_valid = true;

		/* syncro_countdown_ticks */
		syncro_countdown_ticks =
			PRECISION_TICKS_PERIOD_SYNC
		- ((ticks + PRECISION_TICKS_PER_DAY - PRECISION_TICKS_PER_SYNC - PRECISION_TICKS_EXTRA_SYNC) % PRECISION_TICKS_PERIOD_SYNC);
	}

	if (interrupts_enabled) {
		sei();
	}
}

/*****************************************************************************/
bool is_ticksoftheday_valid()
{
	return ticksoftheday_valid;
}

/*****************************************************************************/
u08 getsyncrocount()
{
	return syncrocount;
}

