// vim: shiftwidth=4
// vim: ts=4
package com.errapartengineering.LampController;
import java.io.OutputStream;


/** Very simple time class, only hours, minutes and seconds. */
class MyTime {
	/** hour, range 0..23. */
	public int hour;
	/** minute, range 0..59. */
	public int minute;
	/** second, range 0..59. */
	public int second;

	/** Wrap point for getTime(). */
	public final static long maxTime = 24 * 3600;

	/** Construct new Time... */
	public MyTime(int hour, int minute, int second)
	{
		this.hour = hour;
		this.minute = minute;
		this.second = second;
	}

	/** String representation of time. */
	public String ToString()
	{
		return
			Utils.stringOfInt(hour, 2, '0') + ":" +
			Utils.stringOfInt(minute, 2, '0') + ":" +
			Utils.stringOfInt(second, 2, '0');
	}

	/** Time in milliseconds since midnight. */
	public long getTime()
	{
		return hour*3600 + minute*60 + second;
	}
} // class Time.

