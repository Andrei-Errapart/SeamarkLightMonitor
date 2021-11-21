/*
 * vim: ts=2
 * vim: shiftwidth=2
 */
/* Electronics board of the lampcontroller.

Communications
1.	Serial port connected to SmartFlasher,
	1200 baud, 8 bit, no parity, 1 stop bit
2.	Signal line connected to Modem.
3.	Power line connected to Modem

Operation

Event startup:
Event serial port stop (ten seconds on serial port
  without transmission of 0x00, except when there was
  no transmission even prior power-off):
Event timeout (30 minutes of power-off):
Turn power line ON.

Event signal line (three pulses high-low, with half-period 1 second):
Event serial port start (if serial port starts transmitting 0x00
  after quiet period):
Turn power line OFF.

ATmega88 fuses
Original ATmega88 fuses:
HFUSE: DF = 1101 1111 = !RSTDISBL !DWEN SPIEN !WDTON		!EESAVE !BODLEVEL2 !BODLEVEL1 !BODLEVEL0
LFUSE: 62 = 0110 0010 = CKDIV8 !CKOUT !SUT1 SUT0				CKSEL3 CKSEL2 !CKSEL1 CKSEL0

Fuses for programming 1.8432MHz external crystal, no clock divider.
HFUSE: DF = 1101 1111 = !RSTDISBL !DWEN SPIEN !WDTON		!EESAVE !BODLEVEL2 !BODLEVEL1 !BODLEVEL0
LFUSE: EB = 1110 1011 = !CKDIV8 !CKOUT !SUT1 SUT0				!CKSEL3 CKSEL2 !CKSEL1 !CKSEL0


Serial ports:
UART0 - GPS
UART1 - modem (soft uart)
UART2 - smartflasher (soft uart)

PD3(INT1) - RX1
PD5				- TX1

PD2(INT0)	- RX2 (SYNC OUT+)
PD4				- TX2 (SYNC IN+)
 */

#include <string.h>	/* strcmp */
#include <stdlib.h>	/* atoi */
#include <avr/io.h>	/* inp, outp */
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/sleep.h>
#include <avr/wdt.h>

#include <stdlib.h>

#include "project.h"

/** Tick counter. */
static u16	g_timer											= 0;
/** Number of ticks without a blink. */
static u08	g_ticks_without_blink				= 0;
/** Number of ticks while power off. */
static u16	g_ticks_power_off						= 0;

/** Is the lamp blinking? */
static bool	g_is_blinking								= false;
/** State of blinking at poweroff. */
static bool g_was_blinking_at_poweroff	= false;

/** Timer value on last signal change. */
static u16	g_signal_last_timer					= 0;
/** Number of valid signalling sequences. */
static u08	g_signal_sequences					= 0;

/*****************************************************************************/
static inline void
power_on()
{
#if !defined(TEST_SYNC)
	MODEMPOWER_PORT |= _BV(MODEMPOWER_SIGNAL_BIT);
#endif
}

/*****************************************************************************/
static inline void
power_off()
{
	g_was_blinking_at_poweroff = g_is_blinking;
	g_ticks_power_off = 0;
#if !defined(TEST_SYNC)
	MODEMPOWER_PORT &= ~_BV(MODEMPOWER_SIGNAL_BIT);
#endif
//	debug_blink();
}

// TODO: lift it to the correct place.
/*****************************************************************************/
static bool	shutdown_signal_last = false;
static void shutdown_signal_change()
{
	const u16	delta = g_timer - g_signal_last_timer;
	g_signal_last_timer = g_timer;
	// debug_blink();
	// set_debug_pin(is_signal_high());
	if (delta>=SEQUENCE_TICK_MIN && delta<=SEQUENCE_TICK_MAX) {
		if (!is_signal_high()) {
			++g_signal_sequences;
			if (g_signal_sequences >= SEQUENCE_LENGTH) {
				power_off();
				g_signal_sequences = 0;
			}
		}
	} else {
		// debug_blink();
		g_signal_sequences = 0;
	}
}

/*****************************************************************************/
static void enable_forward()
{
	EICRA	= _BV(ISC10) | _BV(ISC00);	// trigger interrupt on every change
	EIMSK	= _BV(INT0) | _BV(INT1);		// enable INT0 and INT1
}

/*****************************************************************************/
static void disable_forward()
{
	EIMSK	&= ~(_BV(INT0) | _BV(INT1));		// disable INT0 and INT1
}

/*****************************************************************************/
static void update_forward()
{
	if (is_signal_high()) {
		disable_forward();
	} else {
		enable_forward();
	}
}

/** Number of ticks since GPS is powered on. */
uint16_t	gpspower_on_ticks = 0;

/*****************************************************************************/
/** Timer/Counter0 Overflow
 *
 * Frequency macro: TICKS_PER_SECOND.
 */
SIGNAL(SIG_OVERFLOW0)
{
	const bool	signal_high = is_signal_high();
	if (signal_high!=shutdown_signal_last) { 
		update_forward();
		shutdown_signal_change();
	}
	shutdown_signal_last = signal_high;

	++g_timer;
	// debug_blink();
	/** If ticks stopped, turn power on except only when ticks were already off at poweroff. */
	++g_ticks_without_blink;
	if (g_ticks_without_blink > BLINKER_MAX_PERIOD) {
		g_is_blinking = false;
		if (!is_power_on() && g_was_blinking_at_poweroff) {
			power_on();
		}
		g_ticks_without_blink = 0;
	}

	/** Check if power has been off for too long. */
	if (is_power_on()) {
		g_ticks_power_off = 0;
	} else {
		const u16	max_poweroff_period = g_is_blinking ?  POWEROFF_PERIOD : 2*POWEROFF_PERIOD;
		++g_ticks_power_off;
		if (g_ticks_power_off > max_poweroff_period) {
			power_on();
			g_ticks_power_off = 0;
		}
	}

	/** Count/reset GPS poweron ticks. */
	if (is_gpspower_on()) {
		++gpspower_on_ticks;
	} else {
		gpspower_on_ticks = 0;
	}
}

/*****************************************************************************/
static void modem_send_line(			const char*	s)
{
	for (; *s!=0; ++s) {
		modem_send(*s);
	}
	modem_send('\r');
	modem_send('\n');
}

/*****************************************************************************/
static void modem_send_str(			const char*	s)
{
	for (; *s!=0; ++s) {
		modem_send(*s);
	}
}

/*****************************************************************************/
/** Smartflasher receive char. */
void	smartflasher_receive(	const u08	c)
{
	if (c == 0x00) {
		// Turn power on only if we were known not to be blinking.
		g_ticks_without_blink = 0;
		if (!g_is_blinking) {
			power_on();
		}
		g_is_blinking = true; // true;
	}
}

/*****************************************************************************/
/** Calculate t1-t2
 */
static inline s32 t_subtract(	const s32	t1,
															const	s32 t2)
{
	const s32	r = t1 - t2;
	if (r < -(PRECISION_TICKS_PER_DAY/2)) {
		// overroll?
		return r + PRECISION_TICKS_PER_DAY;
	} else if (r > (PRECISION_TICKS_PER_DAY/2)) {
		// underroll??
		return r - PRECISION_TICKS_PER_DAY;
	} else {
		// normal :)
		return r;
	}
}

/*****************************************************************************/
static void
print_time(	PGM_P			prefix,
						const s32	t)
{
	u08	seconds;
	u08	minutes;
	u08	hours;
	char				sbuffer[12];
	s32					tx = t / PRECISION_TICKS_PER_SECOND;
	const u16		tticks = t - tx*PRECISION_TICKS_PER_SECOND;

	hours = tx / 3600;
	tx = tx - hours*3600L;

	minutes = tx / 60;
	tx = tx - minutes*60;

	seconds = tx;


	modem_send_P(prefix);
	modem_send(' ');
	
	utoa(hours, sbuffer, 10);
	modem_send_str(sbuffer);
	modem_send(':');

	utoa(minutes, sbuffer, 10);
	modem_send_str(sbuffer);
	modem_send(':');

	utoa(seconds, sbuffer, 10);
	modem_send_str(sbuffer);
	modem_send(' ');

	utoa(tticks, sbuffer, 10);
	modem_send_str(sbuffer);


	modem_send_P(PSTR("\r\n"));
}

/*****************************************************************************/
/* Main program. Does just setup. */
int
main(void)
{
	/* Setup */
	{
		/* Disable timer interrupts. */
		TIMSK0 = 0x00;
		TIMSK1 = 0x00;
		TIMSK2 = 0x00;

		/* PORTB: MODEMPOWER_SIGNAL_BIT.
		 * Zero all.
		 */
		DDRB	= 0xFF;
		PORTB	= 0x00;

		/* PORTC: GPSPOWER_SIGNAL_BIT.
		 * Zero all (i.e. GPSPOWER is inversed).
		 */
		DDRC	= 0xFF;
		PORTC	= _BV(GPSPOWER_SIGNAL_BIT);

		/* PORTD
		 *	input: PD0(UART0_RX), UART2_PIN_RX, UART1_PIN_RX, MODEM_SIGNAL_BIT
		 *	interrupt on: UART2_PIN_RX and UART1_PIN_RX.
		 */
		DDRD	= 0xFF & ~(_BV(0) | _BV(UART2_PIN_RX) | _BV(UART1_PIN_RX) | _BV(MODEM_SIGNAL_BIT));
		PORTD	= _BV(MODEM_SIGNAL_BIT);// 0x00;

		EICRA	= _BV(ISC10) | _BV(ISC00);	// trigger interrupt on every change
		EIMSK	= _BV(INT0) | _BV(INT1);		// enable INT0 and INT1

		/* UART0; receive only. */
		UBRR0H = (u08) ((UART0_BAUD_SELECT >> 8) && 0xFF);
		UBRR0L = (u08) (UART0_BAUD_SELECT & 0xFF);
		// UCSR0A |= _BV(U2X0); // 0x00;
		UCSR0B = _BV(RXEN0) | _BV(RXCIE0); //  | _BV(TXEN0); // | _BV(TXCIE0) | _BV(UDRIE0);
		// (even parity,)none parity 1 stop bit, 8 data bits
		UCSR0C = /* _BV(UPM01) | */ _BV(UCSZ01) | _BV(UCSZ00);

		{
			// Timer0: 1.8432 MHz / (1024*256)  = 7.03125 Hz
			TCCR0B = (TCCR0B & ~(_BV(CS00) | _BV(CS01) | _BV(CS02))) | (_BV(CS02) | _BV(CS00));
			TIMSK0 = _BV(TOIE0);
		}

		/** Timer1: 3600Hz, Soft-UART interrupt setup. */
		{
			OCR1A  = SOFTUART_TIMERTOP;     /* set top */
#if (SOFTUART_PRESCALE==8)
			/*  Mode 4 - CTC with Prescaler 8 */
			TCCR1B = (1<<WGM12)|(1<<CS11);
#elif (SOFTUART_PRESCALE==1)
			/*  Mode 4 - CTC with Prescaler 1 */
			TCCR1B = (1<<WGM12)|(1<<CS10);
#else
#error "timer_init for prescaler not defined"
#endif
			TIMSK1 |= (1<<OCIE1A); /* enable output-compare int */
			TCNT1 = 0; /* reset counter */
		}

		/* Turn ADC and comparator off. */
		ADCSRA &= ~_BV(ADEN);
		ACSR &= ~(_BV(ACD) | _BV(ACIE));

		/* Disable watchdog. */
		wdt_disable();

		/* Start the work. */
		power_on();
		gpspower_on();
		// set_debug_pin(true);

		update_forward();

		sei();

		modem_send_P(PSTR("cbox:boot\r\n"));

		set_sleep_mode(SLEEP_MODE_IDLE);
	}

	{
		const u08	gps_max_count = 64; // approx. 12 seconds; sigma6 should be at 10ms.
		const s32	gps_max_deltaticks = 80L * PRECISION_TICKS_PER_SECOND;
		const s32	gps_min_deltaticks = 5L * PRECISION_TICKS_PER_SECOND;
		s32	gps_first_tick = -1;		// first sync time.
		s32	gps_delta_sum = 0;			// sum of delta-t's.
		u08	gps_count = 0;
		u08	gps_debug_count = 0;
		u08 syncrocount = 255;

		for (;;) {
			// GPS GPS GPS GPS GPS
			const s16 c = gps_receive();	// received character.
			s32				gps_time;						// gps time.
			if (c!=-1 && handle_gps_input(c, &gps_time)) {
				if (is_ticksoftheday_valid()) {
					const s32 ticks_now = getticksoftheday();
					const s32	dt = t_subtract(gps_time, ticks_now);
					const s32	dt_first = t_subtract(ticks_now, gps_first_tick);
					if (gps_first_tick >= 0 && dt_first<gps_max_deltaticks) {
						if (dt_first>=gps_min_deltaticks) {
							++gps_count;
							gps_delta_sum += dt;
							if (gps_count >= gps_max_count) {
								// 1. Correct ticksoftheday.
								const s32	ticks_to_add = gps_delta_sum >> 6;
								{
									addticksoftheday(ticks_to_add);
									gps_first_tick = -1;
									gpspower_off();
								}

								// 2. Report times.
								{
									char	sbuffer[12];
									// delta ticks.
									modem_send_P(PSTR("cbox:delta="));
									ltoa(ticks_to_add, sbuffer, 10);
									modem_send_line(sbuffer);

									// new time.
									print_time(PSTR("cbox:time="), ticks_now);
								}
							}
						}
					} else {
						modem_send_P(PSTR("cbox:gps start\r\n"));
						gps_count = 0;
						gps_delta_sum = 0;
						gps_first_tick = ticks_now;
					}

#if (0)
					if (++gps_debug_count>=5) {
						char	sbuffer[12];

						modem_send_P(PSTR("cbox:delta_i="));
						ltoa(dt, sbuffer, 10);
						modem_send_line(sbuffer);

						gps_debug_count = 0;
					}
#endif
				} else {
					setticksoftheday(gps_time);
					modem_send_P(PSTR("cbox:gps first round.\r\n"));
				}
			}

			// Idle tasks.
			if (getsyncrocount() != syncrocount) {
				char	sbuffer[12];

				syncrocount = getsyncrocount();
				utoa(syncrocount, sbuffer, 10);
				modem_send_P(PSTR("cbox:sync="));
				modem_send_line(sbuffer);
			}

			// Check if the GPS has been running too long.
			if (is_gpspower_on() && gpspower_on_ticks>GPSPOWER_MAX_PERIOD) {
				modem_send_P(PSTR("cbox:gps time limit exceeded.\r\n"));
				gpspower_off();
				gps_first_tick = -1;
			}

			// Wait for interrupt.
			sleep_mode();
		}
	}

	return 0;
}

