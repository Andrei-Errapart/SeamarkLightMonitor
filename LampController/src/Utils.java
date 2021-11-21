// vim: shiftwidth=4
// vim: ts=4
package com.errapartengineering.LampController;

import java.lang.Integer;

import java.util.Vector;
import java.util.Calendar;
import java.util.Date;

import java.io.IOException;
import java.io.DataOutputStream;
import java.io.DataInputStream;
import java.io.ByteArrayOutputStream;
import java.io.ByteArrayInputStream;

import javax.microedition.rms.RecordStore;
import javax.microedition.rms.RecordStoreException;
import javax.microedition.rms.RecordStoreNotOpenException;

import com.nokia.m2m.imp.iocontrol.IOControl;

/**
Some utility functions.
 */
public abstract class Utils {
	/** Join the thread until it dies. */
	public final static void join(Thread thread)
	{
		while (thread.isAlive()) {
			try {
				thread.join();
			} catch (InterruptedException e) {
				// pass.
			}
		}
	}
	/** Split the string with given separator.
	
	There will always be at least one member in the returned array.
	*/
	public final static String[] split(String s, char sep)
	{
		// Temporary.
		Vector r = new Vector();

		// Do the splitting.
		int so_far = 0;
		int spos = -1;
		do {
			spos = s.indexOf(sep, so_far);
			if (spos>=0) {
				r.addElement(s.substring(so_far, spos));
				so_far = spos + 1;
			}
		} while (spos>=0);
		r.addElement(s.substring(so_far, s.length()));

		// Convert
		String[] sr = new String[r.size()];
		for (int i=0; i<sr.length; ++i) {
			sr[i] = (String)r.elementAt(i);
		}
		return sr;
	}

	/** Join the string array with given separator.
	 */
	public final static String join(String[] array, String sep)
	{
		StringBuffer	sb = new StringBuffer();
		if (array.length>0) {
			sb.append(array[0]);
		}
		for (int i=1; i<array.length; ++i) {
			sb.append(sep);
			sb.append(array[i]);
		}
		return sb.toString();
	}

	/** Convert (signed) byte to (unsigned) integer. */
	public final static int uint_of_byte(	byte	b)
	{
		return b>=0
			? b
			: 256 + b;
	}

	/** Convert two (signed) bytes to (unsigned) integer. */
	public final static int	uint_of_bytes(	byte	high_byte,
											byte	low_byte)
	{
		return uint_of_byte(high_byte)*256 + uint_of_byte(low_byte);
	}

	/** Convert N (signed) bytes to (unsigned) integer. */
	public final static int	uint_of_bytes(	byte[]	array,
											int		index,
											int		n)
	{
		int	r = 0;
		for (int i=0; i<n; ++i) {
			r = r*256 + uint_of_byte(array[index+i]);
		}
		return r;
	}

	/** Convert signed byte to unsigned 16-bit char. */
	public final static char char_of_byte(byte b)
	{
		return (char)(b>=0
			? (int)b
			: (256 + b));
	}

	private static char[] hexchar = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

	/** Convert integer in range 0..255 into hexadecimal representation with possible leading zero. */
	public final static String hex_of_uint8(	int	i)
	{
		char[] chars = { hexchar[(i >>> 4) & 0x0F], hexchar[i & 0x0F] };
		return new String(chars);
	}

	/** Convert integer in range 0..65535 into hexadecimal representation with possible leading zeroes. */
	public final static String hex_of_uint16(	int	i)
	{
		char[] chars = {
			hexchar[(i >>> 12) & 0x0F],
			hexchar[(i >>> 8) & 0x0F],
			hexchar[(i >>> 4) & 0x0F],
			hexchar[i & 0x0F] };
		return new String(chars);
	}

	/** String representation of integer-encode floating point, with 2 decimal places. */
	public final static String	string_of_fp100(	int	fp100)
	{
		int	ints		= fp100 / 100;
		int	decimals	= fp100 % 100;
		if (decimals<=9) {
			return String.valueOf(ints) + ".0" + String.valueOf(decimals);
		}
		return String.valueOf(ints) + "." + String.valueOf(decimals);

	}

	/** Permanent Store name used. */
	private static String store_name = "runcounter";
	/** Maximum idle time between runs, seconds. */
	private static long max_idle_time = 20 * 60;
	/** Maximum number of runs. */
	private static int max_idle_runs = 10;

	/** Helper class to translate between times and permanent storage records. */
	private static class RunCounterRecord
	{
		long Time;
		int RunCount;
		byte[] Buffer;

		public RunCounterRecord(byte[] record) throws java.io.IOException
		{
			ByteArrayInputStream bis = new ByteArrayInputStream(record);
			DataInputStream dis = new DataInputStream(bis);
			Time			= dis.readLong();
			RunCount	= dis.readInt();
			Buffer		= record;
			dis.close();
		}

		public RunCounterRecord(long t, int count) throws java.io.IOException
		{
			ByteArrayOutputStream bos = new ByteArrayOutputStream();
			DataOutputStream dos = new DataOutputStream(bos);
			dos.writeLong(t);
			dos.writeInt(count);
			dos.close();
			Buffer	= bos.toByteArray();
			Time		= t;
			RunCount= count;
		}
	} // class RunCounterRecord

	/** Checks if we should run (i.e. not too early or something.) */
	public static boolean shouldRun(	MyTime	currentTime)
	{
		// 1. Open record store, or create a new one.
		RecordStore rs = null;
		try {
			rs = RecordStore.openRecordStore(store_name, true);
		} catch (Exception e) {
			// log("openRecordStore:" + e.toString());
			return true;
		}

		long current_time = currentTime.getTime();

		boolean r = true;

		try {
			// 2. See if there is a record...
			if (rs.getNumRecords()>0) {
				RunCounterRecord new_record = null;
				RunCounterRecord runcounter = new RunCounterRecord(rs.getRecord(1));
				long delta_time = current_time - runcounter.Time;
				if (delta_time < 0) {
					delta_time += MyTime.maxTime;
				}
				if (delta_time>max_idle_time || runcounter.RunCount>=max_idle_runs) {
					// log("Time to run!!!");
					r = true;
					new_record = new RunCounterRecord(current_time, 1);
				} else {
					// log("Not time to run!!!");
					r = false;
					new_record = new RunCounterRecord(runcounter.Time, runcounter.RunCount + 1);
				}
				rs.setRecord(1, new_record.Buffer, 0, new_record.Buffer.length);
			} else {
				// Insert new record :)
				// log("First run!");
				r = true;
				RunCounterRecord new_record = new RunCounterRecord(current_time, 1);
				rs.addRecord(new_record.Buffer, 0, new_record.Buffer.length);
			}
		} catch (Exception e) {
			// something has gone terribly wrong.
			r = true;
			// log("Exception in shouldRun / check counts: " + e);
			try {
				rs.closeRecordStore();
			} catch (Exception e2) {
				// log("Exception in shouldRun / closeRecordStore: " + e2);
			}

			rs = null;

			try {
				RecordStore.deleteRecordStore(store_name);
			} catch (Exception e2) {
				// log("Exception in shouldRun / deleteRecordStore: " + e2);
			}
		}

		if (rs != null) {
			try {
				rs.closeRecordStore();
			} catch (Exception e) {
				// log("Exception in shouldRun / closeRecordStore: " + e);
			}
		}
		return r;
	}

	/** String representation of number with given number of places. Zeros are stuffed as required. */
	public final static String stringOfInt(	int	i, int nr_of_places, char prefix)
	{
		String sign = i>=0 ? "" : "-";
		String si = Integer.toString(i>=0 ? i : -i);
		int nmissing = nr_of_places - sign.length() - si.length();
		if (nmissing<=0) {
			return sign + si;
		}
		StringBuffer	sb = new StringBuffer();
		sb.append(sign);
		for (int j=0; j<nmissing; ++j) {
			sb.append(prefix);
		}
		sb.append(si);
		return sb.toString();
	}

	/** Convert to Calendar representation of time.. */
	private final static int monthOf1to12(	int month_no)
	{
		switch (month_no) {
		case 1:		return Calendar.JANUARY;
		case 2:		return Calendar.FEBRUARY;
		case 3:		return Calendar.MARCH;
		case 4:		return Calendar.APRIL;
		case 5:		return Calendar.MAY;
		case 6:		return Calendar.JUNE;
		case 7:		return Calendar.JULY;
		case 8:		return Calendar.AUGUST;
		case 9:		return Calendar.SEPTEMBER;
		case 10:	return Calendar.OCTOBER;
		case 11:	return Calendar.NOVEMBER;
		case 12:	return Calendar.DECEMBER;
		}
		return 1;
	}

	/** Convert Calendar representation of time to month in range 1..12. */
	private final static int monthTo1to12(	int calendar_month)
	{
		switch (calendar_month) {
		case Calendar.JANUARY:	return 1;
		case Calendar.FEBRUARY:	return 2;
		case Calendar.MARCH:	return 3;
		case Calendar.APRIL:	return 4;
		case Calendar.MAY:		return 5;
		case Calendar.JUNE:		return 6;
		case Calendar.JULY:		return 7;
		case Calendar.AUGUST:	return 8;
		case Calendar.SEPTEMBER:return 9;
		case Calendar.OCTOBER:	return 10;
		case Calendar.NOVEMBER:	return 11;
		case Calendar.DECEMBER:	return 12;
		}
		return 1;
	}

	/** Date in a form: "yyyy-MM-dd HH:mm:ss". */
	public final static String formatDate(Date dt)
	{
		Calendar cal = Calendar.getInstance();
		cal.setTime(dt);
		int year	= cal.get(Calendar.YEAR);
		int month	= monthTo1to12(cal.get(Calendar.MONTH));
		int	day		= cal.get(Calendar.DAY_OF_MONTH);
		int hour	= cal.get(Calendar.HOUR_OF_DAY);
		int minute	= cal.get(Calendar.MINUTE);
		int	second	= cal.get(Calendar.SECOND);
		return
			stringOfInt(year, 4, '0') + "-" +
			stringOfInt(month, 2, '0') + "-" +
			stringOfInt(day, 2, '0') + " " +
			stringOfInt(hour, 2, '0') + ":" +
			stringOfInt(minute, 2, '0') + ":" +
			stringOfInt(second, 2, '0') +
			" (" + dt.getTime()+")";

	}

	/** Date in a form: "yyyy-MM-dd HH:mm:ss". */
	public final static String formatDate(long ms)
	{
		return formatDate(new Date(ms));
	}

	/** Construct new Date object. */
	public final static Date makeDate(
								int year,
								int	month,
								int day_of_month,
								int hour,
								int minute,
								int second)
	{
		Calendar	cal = Calendar.getInstance();
		cal.set(Calendar.YEAR, year);
		cal.set(Calendar.MONTH, Utils.monthOf1to12(month));
		cal.set(Calendar.DAY_OF_MONTH, day_of_month);
		cal.set(Calendar.HOUR_OF_DAY, hour);
		cal.set(Calendar.MINUTE, minute);
		cal.set(Calendar.SECOND, second);
		return cal.getTime();
	}

	/** Construct new Date from the SmartFlasher date string. */
	public final static Date makeDateOfSmartFlasher(String	s) throws RuntimeException
	{
		int year			= Integer.parseInt(s.substring( 0, 4), 10);
		int month			= Integer.parseInt(s.substring(12,14), 10);
		int day_of_month	= Integer.parseInt(s.substring(10,12), 10);
		int hour			= Integer.parseInt(s.substring( 8,10), 10);
		int minute			= Integer.parseInt(s.substring( 6, 8), 10);
		int second			= Integer.parseInt(s.substring( 4, 6), 10);
		if (year>=2006 &&
			month>=1 && month<=12 &&
			day_of_month>=1 && day_of_month<=31 &&
			hour>=0 && hour<=24 &&
			minute>=0 && minute<=60 &&
			second>=0 && second<=60) {
			return makeDate(year, month, day_of_month, hour, minute, second);
		}
		throw new RuntimeException("makeDateOfSmartFlasher: Invalid date: " + s);
	}

	/** Construct new Date from the SmartFlasher date string. */
	public final static MyTime makeTimeOfSmartFlasher(String	s) throws RuntimeException
	{
		int hour			= Integer.parseInt(s.substring( 8,10), 10);
		int minute			= Integer.parseInt(s.substring( 6, 8), 10);
		int second			= Integer.parseInt(s.substring( 4, 6), 10);
		if (hour>=0 && hour<=24 &&
			minute>=0 && minute<=60 &&
			second>=0 && second<=60) {
			return new MyTime(hour, minute, second);
		}
		throw new RuntimeException("makeTimeOfSmartFlasher: Invalid date: " + s);
	}

    public final static String[] chopPrefix(
        String[] list,
        int n
        )
    {
        int new_n = list.length - n;
        if (new_n <= 0)
        {
            return new String[0];
        }

        String[] r = new String[new_n];
        for (int i = n; i < list.length; ++i)
        {
            r[i - n] = list[i];
        }
        return r;
    }
} // class Utils

