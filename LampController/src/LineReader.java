// vim: shiftwidth=4
// vim: ts=4
package com.errapartengineering.LampController;

import java.util.Vector;
import java.lang.Thread;
import java.lang.StringBuffer;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;


/**
Reads lines from the given input stream. The 0-s in the data read are
replaced with null strings.
*/
public final class LineReader {
	private Vector			string_queue_	= new Vector();
	private StringBuffer	sb_				= new StringBuffer();
	private boolean			last_was_cr_	= false;
	private InputStream		line_in_		= null;
	private byte[]			inbuffer_		= new byte[256];
	private final static int max_sb_length_	= 128;	// 2x48+4 = 100

	/** Construct a new linereader and start reading :) */
	public LineReader(InputStream line_in)
	{
		line_in_ = line_in;
	}

	/// <summary>
	/// Number of lines in the queue.
	/// </summary>
	public final int length() throws StopException
	{
		checkInput();
		return string_queue_.size();
	}

	/// <summary>
	/// Pop all lines 
	/// </summary>
	/// <returns>Iterator</returns>
	public final String[] popAll() throws StopException
	{
		checkInput();

		int			n = string_queue_.size();
		String[]	r = new String[n];

		for (int i=0; i<n; ++i) {
			r[i] = (String)string_queue_.elementAt(i);
		}
		string_queue_.removeAllElements();

		return r;
	}

	/// <summary>
	/// Append data to the queue, split them into lines in the process.
	/// </summary>
	/// <param name="data"></param>
	private final void append(byte[] data, int length)
	{
		for (int i=0; i<length; ++i) {
			byte b = data[i];
			if (b == 0) {
				string_queue_.addElement(null);
			} else {
				char c = Utils.char_of_byte(b);
				switch (c) {
				case '\r':
					string_queue_.addElement(sb_.toString());
					sb_.setLength(0);
					last_was_cr_ = true;
					break;
				case '\n':
					if (!last_was_cr_) {
						string_queue_.addElement(sb_.toString());
						sb_.setLength(0);
					}
					last_was_cr_ = false;
					break;
				default:
					sb_.append(c);
					// Fast track for "OK".
					if (sb_.length() == 2) {
						String	s = sb_.toString();
						if (s.equals("OK")) {
							string_queue_.addElement(s);
							sb_.setLength(0);
						}
					}

					last_was_cr_ = false;
					break;
				}
			}
		}

		// Fast track for datetimes: length 14, all digits.
		if (sb_.length() == 14) {
			String s = sb_.toString();
			String s4 = s.substring(0,4);
			if (!s4.equals("00000") && !s4.equals("002F") && !s4.equals("0036") && !s4.equals("0040") && !s4.equals("B600") && !s4.equals("B61B")) {
				boolean all_digits = true;
				for (int i=4; i<s.length(); ++i) {
					char c = s.charAt(i);
					if (c<'0' || c>'9') {
						all_digits = false;
						break;
					}
				}
				if (all_digits) {
					try {
						Utils.makeTimeOfSmartFlasher(s);
						string_queue_.addElement(s);
						sb_.setLength(0);
					} catch (RuntimeException e) {
						// pass.
					}
				}
			}
		}

		// Have we got too much trash?
		if (sb_.length()>max_sb_length_) {
			string_queue_.addElement("linereader overflow");
			sb_.setLength(0);
		}
	}

    /** Reading cycle.
    Keep watching for available data, return prematurely if there is any.
    Obey should_stop_ flag by throwing StopException.
     */
    public final void checkInput() throws StopException
	{
		if (Globals.StopFlagGet()) {
			throw new StopException("LineReader.checkInput");
		}

		// Try to read some bytes.
		try {
			int navailable = line_in_.available();
			if (navailable>inbuffer_.length) {
				navailable = inbuffer_.length;
			}
			if (navailable>0) {
				int	nread = line_in_.read(inbuffer_, 0, navailable);
				if (nread>0) {
                    IOPanel.ToggleSerialInTraffic();
                    append(inbuffer_, nread);
				}
			}
		} catch (IOException e) {
			// pass.
		}

		// Let other threads to run, too.
		// Thread.yield();
	}
} // class CMR

