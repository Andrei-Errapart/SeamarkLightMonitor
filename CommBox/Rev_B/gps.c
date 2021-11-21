/*
 * vim: ts=2
 * vim: shiftwidth=2
 */

#include "project.h"

#define	GPS_MAX_FIELDS	20

/*****************************************************************************/
typedef struct {
	u08	hour;			///< 0 .. 23
	u08	minute;		///< 0 .. 59
	u08	second;		///< 0 .. 59
	u16	tick;			///< Internal, set to zero only. 0 .. 7199
} TIME;

/*****************************************************************************/
static u08			gps_field_index = 0;	///< 0 - not receiving, 1=type (GPGGA), 2=time (HHMMSS.s/ss/sss), 3=lat, 4='N'/'S', 5=lon, 6='E'/'W', 7=quality (0=no fix, 1=fix, 2=diff. fix), 8=number of satellites per view, 9=PDOP, 10=altitude, 11=alt. unit, ..., *CHECKSUM
static u08			gps_buffer[20];
static u08			gps_buffer_index = 0;
static bool			gps_is_gpgga = false;
static bool			gps_is_checksum = false;
static bool			gps_has_fix = false;
static bool			gps_has_time = false;
static TIME			gps_time;
static u08			gps_checksum = 0;
static s32			gps_start_ticks = 0;

/*****************************************************************************/
static u16
parse_n_decimals(	const u08*	s,
									const u08		n)
{
	u08	i;
	u16	r = 0;
	for (i=0; i<n; ++i) {
		r = r*10 + (s[i]-'0');
	}
	return r;
}

/*****************************************************************************/
static u08
hexchar_of_int(		const u08	ii)
{
	return ii<10 ? ii + '0' : ii + 'A' - 10;
}

/*****************************************************************************/
static s32 ticks_of_time(		TIME*				t)
{
	// hour
	s32	r = t->hour;

	// minute
	r *= 60;
	r += t->minute;

	// second
	r *= 60;
	r += t->second;

	// ticks
	r *= PRECISION_TICKS_PER_SECOND;
	r += t->tick;
	return r;
}

/*****************************************************************************/
static bool gps_parse_time(	TIME*				t,
														const u08*	s,
														const u08		size)
{
	u16		ticks = 0;

	if (size<6) {
		return false;
	}

	if (size>=8 && s[6]=='.') {
		const u08	dotpos = 6;
		const u08	n = size - 6 - 1;
		switch (n) {
		case 1:
			ticks = parse_n_decimals(s + dotpos + 1, n);
			ticks = ticks * (PRECISION_TICKS_PER_SECOND / 10);
			break;
		case 2:
			ticks = parse_n_decimals(s + dotpos + 1, n);
			ticks = ticks * (PRECISION_TICKS_PER_SECOND / 100);
			break;
		case 3:
			ticks = parse_n_decimals(s + dotpos + 1, n);
			ticks = (ticks / 5) * (PRECISION_TICKS_PER_SECOND / 200);
			break;
		default:
			return false;
		}
	}

	t->hour   = parse_n_decimals(s  , 2);
	t->minute = parse_n_decimals(s+2, 2);
	t->second = parse_n_decimals(s+4, 2);
	t->tick		= ticks;

	return true;
}

/*****************************************************************************/
bool
handle_gps_input(	const u08		c,
									s32*				t)
{
	bool	r = false;

	switch (c) {
	case '$':
		// Start again.
		gps_start_ticks = getticksoftheday();
		gps_field_index = 1;
		gps_buffer_index = 0;
		gps_is_gpgga = false;
		gps_is_checksum = false;
		gps_has_fix = false;
		gps_has_time = false;
		gps_checksum = 0;
		break;
	case '*':
		// Switch to checksum.
		gps_is_checksum = true;
		gps_buffer_index = 0;
		break;
	case ',':
		// Field over.
		if (gps_field_index>=1 && gps_field_index + 1 < GPS_MAX_FIELDS) {
			if (gps_field_index==1) {
				// must be GPGGA
				gps_is_gpgga = gps_buffer_index==5 && gps_buffer[0]=='G' && gps_buffer[1]=='P' && gps_buffer[2]=='G' && gps_buffer[3]=='G' && gps_buffer[4]=='A';
			} else if (gps_is_gpgga) {
				switch (gps_field_index) {
					case 2:
						// time
						gps_has_time = gps_parse_time(&gps_time, gps_buffer, gps_buffer_index);
						break;
					case 7:
						// fix
						gps_has_fix = gps_buffer_index>0 && gps_buffer[0]!='0';
#if (0)
						if (gps_has_fix) {
							modem_send_P(PSTR("GOT FIX\r\n"));
						} else {
							modem_send('$');
							modem_send_hex(gps_buffer_index);
							modem_send('_');
							modem_send_hex(gps_buffer[0]);
							modem_send_P(PSTR("NO FIX\r\n"));
						}
						break;
#endif
#if defined(GPS_IGNORE_FIX)
						gps_has_fix = true;
#endif
				}
			}
		}
		gps_buffer_index = 0;
		++gps_field_index;
		break;
	case 0x0D:
		// Check checksum.
		if (gps_buffer_index>=2 && hexchar_of_int(gps_checksum >> 4)==gps_buffer[0] && hexchar_of_int(gps_checksum & 0x0F)==gps_buffer[1]) {
			if (gps_has_time && gps_has_fix) {
				// YES!
				*t = ticks_of_time(&gps_time) + (getticksoftheday() - gps_start_ticks);
				r = true;
#if (0)
				modem_send_P(PSTR("TIME OK\r\n"));
#endif
			} else {
#if (0)
				if (gps_has_time) {
					modem_send_P(PSTR("FIX MISSING\r\n"));
				} else {
					modem_send_P(PSTR("CK OK\r\n"));
				}
#endif
			}
		} else {
#if (0)
			modem_send(gps_has_time ? '!' : '#');
			modem_send_hex(gps_buffer_index);
			modem_send(':');
			modem_send_hex(gps_checksum);
			modem_send('=');
			modem_send(gps_buffer[0]);
			modem_send(gps_buffer[1]);
			modem_send_P(PSTR("\r\n"));
#endif
		}
		// Clear.
		gps_field_index = 0;
		gps_has_time = false;
		gps_has_fix = false;
		break;
	case 0x0A:
		// pass.
		break;
	default:
		// Receiving?
		if (gps_field_index>0) {
			// !Overflow?
			if (gps_buffer_index+1 < sizeof(gps_buffer)/sizeof(gps_buffer[0])) {
				gps_buffer[gps_buffer_index] = c;
				++gps_buffer_index;
				if (!gps_is_checksum) {
					gps_checksum ^= c;
				}
			} else {
				gps_field_index = 0; // restart on overflow.
			}
		}
	}

	return r;
}

