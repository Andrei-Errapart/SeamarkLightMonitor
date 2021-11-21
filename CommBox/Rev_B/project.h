/*
 * vim: ts=2
 * vim: shiftwidth=2
 */
#ifndef project_h_
#define	project_h_

#include <stdint.h>
#include <avr/pgmspace.h>
/** \file Project-specific options. */

#define F_CPU						1843200 /* Processor clock frequency */

#define	MODEMPOWER_DDR					DDRB
#define	MODEMPOWER_PORT					PORTB
#define	MODEMPOWER_PIN					PINB
#define	MODEMPOWER_SIGNAL_BIT		0	// PB0

#define	GPSPOWER_DDR						DDRC
#define	GPSPOWER_PORT						PORTC
#define	GPSPOWER_PIN						PINC
#define	GPSPOWER_SIGNAL_BIT			2	// PC2 (REVERSED)
#define	gpspower_on()						do { GPSPOWER_PORT &= ~(_BV(GPSPOWER_SIGNAL_BIT)); } while (0)
#define	gpspower_off()					do { GPSPOWER_PORT |= _BV(GPSPOWER_SIGNAL_BIT); } while (0)
#define	is_gpspower_on()				( (GPSPOWER_PORT & _BV(GPSPOWER_SIGNAL_BIT))==0 )

/** Number of GPS fixes required for sync. */
#define	GPSFIXES_REQUIRED				25

#define	MODEM_DDR								DDRD
#define	MODEM_PORT							PORTD
#define	MODEM_PIN								PIND
#define	MODEM_SIGNAL_BIT				6	// PD6
#define	is_power_on()						( (MODEMPOWER_PORT & _BV(MODEMPOWER_SIGNAL_BIT)) !=0 )
#define	is_signal_high()				( (MODEM_PIN & _BV(MODEM_SIGNAL_BIT)) != 0 )


// UART0 - GPS, hardware UART.
/** GPS UART baud rate. */
#ifndef UART0_BAUD_RATE
#define UART0_BAUD_RATE		19200
#endif
/** GPS UART baud register */
#define UART0_BAUD_SELECT (F_CPU/(UART0_BAUD_RATE*16l)-1)

/** Soft-UART (UART1, UART2) baud rate. */
#define	SOFTUART_BAUD_RATE	1200
/** SOFT-UART prescaler. Timer interrupt will be set to three times the required baud rate. */
#define SOFTUART_PRESCALE (8)
//#define SOFTUART_PRESCALE (1)
#define SOFTUART_TIMERTOP ( F_CPU/SOFTUART_PRESCALE/SOFTUART_BAUD_RATE/3 -1)
#if (SOFTUART_TIMERTOP > 0xff)
#warning "Check SOFTUART_TIMERTOP"
#endif


// UART1 - modem communication.
#define	UART1_DDR								DDRD
#define	UART1_PORT							PORTD
#define	UART1_PIN								PIND
#define	UART1_PIN_RX						3			// PD3(INT1)	(TX)
#define	UART1_PIN_TX						5			// PD5				(RX)

// UART2 - smartflasher communication.
#define	UART2_DDR								DDRD
#define	UART2_PORT							PORTD
#define	UART2_PIN								PIND
#define	UART2_PIN_RX						2			// PD2(INT0) (SYNC OUT+)
#define	UART2_PIN_TX						4			// PD4				(SYNC IN+
#define	syncro_on()							do { UART2_PORT |= _BV(UART2_PIN_TX); } while (0)
#define	syncro_off()						do { UART2_PORT &= ~(_BV(UART2_PIN_TX)); } while (0)
#define	is_syncro_on()					( (UART2_PORT & _BV(UART2_PIN_TX))!=0 )


/** Timer ticks per second. */
#define	TICKS_PER_SECOND				7
#define	BLINKER_MAX_PERIOD			(10 * TICKS_PER_SECOND)
#define	POWEROFF_PERIOD					(30 * 60 * TICKS_PER_SECOND)
#define	GPSPOWER_MAX_PERIOD			(5 * 60 * TICKS_PER_SECOND)
#define	SEQUENCE_TICK_MIN				2
#define	SEQUENCE_TICK_MAX				(3*TICKS_PER_SECOND)
#define	SEQUENCE_LENGTH					2	/* total: 4 seconds. */

/** Precision timer for syncing. */
#define	PRECISION_TICKS_PER_SECOND				(3600)
#define	PRECISION_TICKS_PER_DAY						(24L*3600L*PRECISION_TICKS_PER_SECOND)
/** SmartFlasher SYNC pulse, 140 ms. */
#define	PRECISION_TICKS_PER_SYNC					504
/** SmartFlasher startup-delay, 50 ms. */
#define	PRECISION_TICKS_EXTRA_SYNC				180
/** Sync period, 15 minutes. */
#define	PRECISION_TICKS_PERIOD_SYNC				(900L * PRECISION_TICKS_PER_SECOND)
/** Number of ticks before syncro impulse to turn GPS on. */
#define	PRECISION_TICKS_GPS_LEAD					(240L * PRECISION_TICKS_PER_SECOND)

/* Some helper types. FIXME: remove these. */
typedef uint8_t 	u08;	//	0...255
typedef int8_t		s08;	// -127...127
typedef uint16_t	u16;	// 0...65535
typedef int16_t		s16;	// -32768...32768
typedef uint32_t	u32;
typedef int32_t		s32;

typedef enum {
	true	= 0xFF,
	false	= 0x00
} bool;

// ---- File: main.c
/** UART2 data callback. */
extern void	smartflasher_receive(	const u08	c);

// ---- File: uart.c
extern void modem_send(					const u08		c);
extern void modem_send_P(				PGM_P				s);
extern void modem_send_hex(			const uint8_t	x);

/** Read byte from GPS serial port, -1 if none available. */
extern s16 gps_receive();

/** Ticks of the day. */
extern s32	getticksoftheday();
/** Add offset to the ticksoftheday. */
extern void addticksoftheday(	const s32		extra_ticks);
/** Set new ticksoftheday. */
extern void setticksoftheday(	const s32		ticks);
/** Is ticksoftheday valid? */
extern bool is_ticksoftheday_valid();

/** Number of flasher synchronizations so far. */
extern u08 getsyncrocount();

// --- File: gps.c
/**
\param[in]	c				Character read from UART
\param[out]	t				GPS time, corrected for the given moment. Unit: ticks of the day.
 */
extern bool handle_gps_input(		const u08		c,
																s32*				t);

// Debugging, if any.
#if defined(TEST_PIN)
/*****************************************************************************/
// BLINK PC0 and PC1
# define	BLINK_BITS	0x01
#	define set_debug_pin(x)		PORTB = (PORTB & ~BLINK_BITS) | ((x) ? BLINK_BITS : 0x00)
#	define debug_blink()			do { if ((PORTB & BLINK_BITS)==0) set_debug_pin(true); else set_debug_pin(false); } while (0)
#else
#	define set_debug_pin(x)	do { } while (0)
#	define debug_blink()			do { } while (0)
#endif

#endif /* project_h_ */

