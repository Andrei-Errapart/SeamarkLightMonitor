// vim: shiftwidth=4
// vim: ts=4
package com.errapartengineering.LampController;

import java.lang.Thread;
import java.lang.StringBuffer;

import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import java.util.Vector;
import java.util.Date;
import java.util.Calendar;

/**
Reads lines from the given input stream. The 0-s in the data read are
replaced with null strings.
*/
public final class SmartFlasher {
	private final static class MemoryArea {
		public int	address;
		public int	length;
		public boolean writable;
		public MemoryArea (int address, int length, boolean writable) 
		{
			this.address	= address;
			this.length		= length;
			this.writable	= writable;
		}
	} // class MemoryArea

	public	byte[] 			memory		= new byte[0xB800];
	private	MemoryArea[]	memoryAreas	= new MemoryArea[7];
	private	boolean			link_ok_	= false;


	private	LineReader		linereader_	= null;
	private	OutputStream	line_out_	= null;

	private final static int	WRITELOCK_ADDRESS	= 0x001B;
	private final static byte	WRITELOCK_BYTE_1	= (byte)0x5A;
	private final static byte	WRITELOCK_BYTE_2	= (byte)0xA5;

	/** Read command timeout, milliseconds. */
	private final static long	READ_TIMEOUT				= 2000;
	/** Read timeout per character. */
	private final static long	READ_TIMEOUT_PER_CHAR		=    0;
	/** Write command timeout, milliseconds. */
	private final static long	WRITE_TIMEOUT				= 5000;

	/** Parameters block. */
	private final static int	ADDRESS_BLOCK_PARAMETERS	= 0xB600;
	/** Blinking block. */
	private final static int	ADDRESS_BLOCK_BLINKING		= 0xB61B;
	/** Another parameters block. */
	private final static int	ADDRESS_BLOCK_PARAMETERS_2	= 0xB7D0;

	/** Constructor. */
	public SmartFlasher (	InputStream		line_in,
							OutputStream	line_out)
	{
		for (int i=0; i<memory.length; ++i) {
			memory[i] = 0;
		}
		memoryAreas[0] = new MemoryArea(0x0000, 32,	false);
		memoryAreas[1] = new MemoryArea(0x002F, 1,	true);
		memoryAreas[2] = new MemoryArea(0x0036, 10, false);
		memoryAreas[3] = new MemoryArea(0x0040, 16,	false);
		memoryAreas[4] = new MemoryArea(0xB600, 27,	true);
		memoryAreas[5] = new MemoryArea(0xB61B, 48,	true);
		memoryAreas[6] = new MemoryArea(0xB7D0, 33,	true);

		line_out_	= line_out;
		linereader_ = new LineReader(line_in);
	}

	/** Flush input on the linereader. */
	public void flushInput() throws StopException
	{
		linereader_.checkInput();
		if (linereader_.length()>0) {
			linereader_.popAll();
		}
	}

	/** Is smartflash link OK? */
	public boolean isLinkOK()
	{
		return link_ok_;
	}

	/** Reload from the smartflasher. */
	public boolean reload(	StringBuffer	error_buffer) throws StopException
	{
		boolean ok = true;

        IOPanel.SetShutdown();
		try {
			for (int i=0; i<memoryAreas.length; ++i) {
				ok = ok && read(memoryAreas[i].address, memoryAreas[i].length, 5, error_buffer);
			}
		} finally {
            IOPanel.ClearShutdown();
		}

		link_ok_ = ok;
		return ok;
	}

	private void xlog(String msg, StringBuffer error_buffer)
	{
		try {
			if (error_buffer != null) {
				error_buffer.append(msg);
				error_buffer.append('\n');
			}
			/*
			byte[] buffer = (msg + '\r').getBytes();
			line_out_.write(buffer, 0, buffer.length);
			*/
		} catch (Exception e) {
			// pass.
		}
	}

	/** Construct write command. */
	private final static String write_command(	byte[]	buffer,
												int		buffer_address,
												int		address,
												int		length)
	{
		StringBuffer	sb = new StringBuffer();
		// WHHLLNNdd...dd
		sb.append('W');
		sb.append(Utils.hex_of_uint8(address >>> 8));
		sb.append(Utils.hex_of_uint8(address & 0xFF));
		sb.append(Utils.hex_of_uint8(length));
		for (int i=0; i<length; ++i) {
			sb.append(Utils.hex_of_uint8(buffer[buffer_address + i]));
		}
		sb.append('\r');

		return sb.toString();
	}

	/** Construct read command. */
	private final static String read_command(	int		address,
												int		length)
	{
		StringBuffer	sb = new StringBuffer();
		// RHHLLNN00
		sb.append('R');
		sb.append(Utils.hex_of_uint8(address >>> 8));
		sb.append(Utils.hex_of_uint8(address & 0xFF));
		sb.append(Utils.hex_of_uint8(length));
		sb.append("00\r");

		return sb.toString();
	}

	/** Executes raw memory write command, expects positive answer (OK). */
	private boolean write(	byte[]			buffer,
							int				buffer_address,
							int				address,
							int				length,
							StringBuffer	error_buffer) throws StopException
	{
		// Build the command.
		String	scommand = write_command(buffer, buffer_address, address, length);
		byte[]	command = scommand.getBytes();

		// TODO: should retry?
		{
			long	start_time	= System.currentTimeMillis();
			long	end_time	= start_time + WRITE_TIMEOUT;
			try {
				// Write the line.
                IOPanel.ToggleSerialOutTraffic();
				line_out_.write(command, 0, command.length);
				start_time	= System.currentTimeMillis();
				end_time	= start_time + WRITE_TIMEOUT;
				error_buffer.append("Sent line: "+scommand + "("+(System.currentTimeMillis() - start_time) + ")\n");

				// No need to wait for the response.
				return true;
			} catch (Exception ex) {
				// pass.
				// pass the time.
				while (end_time > System.currentTimeMillis()) {
                    linereader_.checkInput();
				}
			}
		}

		xlog("Lowlevel write (address " + Utils.hex_of_uint16(address) + " length " + length + ") timeouted.", error_buffer);
		return false;
	}

	/** Write some bytes from memory area into SmartFlasher. */
	private boolean write(	int				address,
							byte[]			data,
							StringBuffer	error_buffer) throws StopException
	{
		// 1. Set up writelock.
		memory[WRITELOCK_ADDRESS+0] =	WRITELOCK_BYTE_1;
		memory[WRITELOCK_ADDRESS+1] =	WRITELOCK_BYTE_2;

		// 3. Do the reading.
		for (int retryCount=0; retryCount<1; ++retryCount) {
			boolean	ok = true;
			// Release writelock.
			ok = ok && read(0xB600, 1, 5, error_buffer);

			ok = ok && write(memory, WRITELOCK_ADDRESS, WRITELOCK_ADDRESS, 2, error_buffer);
			// Check writelock.
			ok = ok && read(WRITELOCK_ADDRESS, 2, 2, error_buffer);
			if (ok && memory[WRITELOCK_ADDRESS+0] != WRITELOCK_BYTE_1 || memory[WRITELOCK_ADDRESS+1] != WRITELOCK_BYTE_2) {
				xlog("Oops, writelock failure.", error_buffer);
				ok = false;
			}
			ok = ok && read(0xB600, 1, 5, error_buffer);

			// Do the raw write.
			ok = ok && write(data, 0, address, data.length, error_buffer);

			/*
			if (false) {
				// WRITELOCK.
				{
					String	scommand = write_command(memory, WRITELOCK_ADDRESS, WRITELOCK_ADDRESS, 2);
					byte[]	command = scommand.getBytes();
					try {
						line_out_.write(command, 0, command.length);
						line_out_.write(command, 0, command.length);
					} catch (IOException e) {
					}
				}
				// DATA.
				{
					String	scommand = write_command(data, 0, address, data.length);
					byte[]	command = scommand.getBytes();
					try {
						line_out_.write(command, 0, command.length);
					} catch (IOException e) {
					}
				}
			}
			*/

			// Read from the SmartFlasher.
			ok = ok && read(address, data.length, 5, error_buffer);

			if (ok) {
				return true;
			}
		}
		xlog("Write (address " + Utils.hex_of_uint16(address) + " length " + data.length + ") timeouted.", error_buffer);

		return false;
	}

	/** Read some bytes from SmartFlasher into given memory area.
	@param memory_buffer	Buffer to read into
	@param memory_offset	Offset to start reading into
	@param address			SmartFlasher address
	@param length			Number of bytes to read.
	@param nretries			Number of retries.
	@param error_buffer		Buffe to store error messsages (if any).
	*/
	private boolean read(	byte[]			memory_buffer,
							int				memory_offset,
							int				address,
							int 			length,
							int				nretries,
							StringBuffer	error_buffer) throws StopException
	{
		// Build the command.
		String	scommand = read_command(address, length);
		byte[]	command = scommand.getBytes();

		// Retry up to 5 times, timeout for each query is approx. 1 second?
		int	expected_length = 2 * (length + 3);
		byte rbuffer[] = new byte[length];
		for (int retryCount=0; retryCount<nretries; ++retryCount) {
			long start_time = System.currentTimeMillis();
			long end_time = start_time + READ_TIMEOUT + READ_TIMEOUT_PER_CHAR*length;
			try {
				// Write the line.
                IOPanel.ToggleSerialOutTraffic();
				line_out_.write(command, 0, command.length);
				start_time = System.currentTimeMillis();
				end_time = start_time + READ_TIMEOUT + READ_TIMEOUT_PER_CHAR*length;
				error_buffer.append("Start time: " + start_time + " end time: " + end_time + "\n");
				error_buffer.append("Sent line: " + scommand + "\n");

				// Check responses until timeout.
				do {
                    linereader_.checkInput();
					if (linereader_.length()>0) {
						String[]	lines = linereader_.popAll();
						for (int lineIndex=0; lineIndex<lines.length; ++lineIndex) {
							String line = lines[lineIndex];
							if (line == null) {
								error_buffer.append("Got ping. (" + (System.currentTimeMillis() - start_time) + ")\n");
							   	continue;
							}
							error_buffer.append("Got line: " + line + "(" + (System.currentTimeMillis() - start_time) + ")\n");
							if (line.length()!=expected_length) {
								xlog("wrong length:" + line.length(), error_buffer);
								continue;
							}

							// 1. Address check.
							int address2 = Integer.parseInt(line.substring(0,4), 16);
							if (address2 != address) {
								xlog("address mismatch", error_buffer);
								continue;
							}

							// Parse data.
							int	checksum = 0x00; // (address >>> 8) + (address & 0xFF);
							for (int i=0; i<length; ++i) {
								int idx = 2 * (i + 2);
								rbuffer[i] = (byte)Integer.parseInt(line.substring(idx, idx+2), 16);
								checksum = checksum + rbuffer[i];
							}

							// Checksum?
							checksum = checksum & 0xFF;
							int	checksum2 = Integer.parseInt(line.substring(expected_length-2, expected_length), 16);
							if (checksum2 != checksum) {
								xlog("checksum error, got "+checksum+" expected "+checksum2, error_buffer);
								continue;
							}

							// YES!

							for (int i=0; i<length; ++i) {
								memory_buffer[memory_offset + i] = rbuffer[i];
							}
							return true;
						}
					}
				} while (end_time > System.currentTimeMillis());
				long current_time = System.currentTimeMillis();
				long dt = current_time - start_time;
				xlog("Read (address " + Utils.hex_of_uint16(address) + " length " + length + ") retry " + (retryCount+1) + " timeouted" + " DT = " + dt, error_buffer);
			} catch (Exception ex) {
				xlog("Exception: " + ex.toString(), error_buffer);
				// pass the time.
				while (end_time > System.currentTimeMillis()) {
                    linereader_.checkInput();
				}
			}

		}
		return false;
	}

	/** Read some bytes from SmartFlasher into memory area. */
	private boolean read(	int				address,
							int 			length,
							int				nretries,
							StringBuffer	error_buffer) throws StopException
	{
		return read(memory, address, address, length, nretries, error_buffer);
	}

	/** String representation of the memory area. */
	public final String memoryString()
	{
		StringBuffer	sb = new StringBuffer();

		for (int areaIndex=0; areaIndex<memoryAreas.length; ++areaIndex) {
			int address = memoryAreas[areaIndex].address;
			int length = memoryAreas[areaIndex].length;
			sb.append(Utils.hex_of_uint16(address));
			sb.append(':');
			for (int i=0; i<length; ++i) {
				sb.append(Utils.hex_of_uint8(memory[address + i]));
			}
			sb.append(';');
		}

		return sb.toString();
	}

	/** Make copy of the memory are for custom purposes. */
	private final byte[] makeCopy(	int			address,
									int			length)
	{
		byte[]	r = new byte[length];
		for (int i=0; i<length; ++i) {
			r[i] = memory[address + i];
		}
		return r;
	}

	/**********************************************************************/
	/** Update blink period in the memory. */
	private final static void update_blink(	byte[]		memory,
											int			offset,
											String		speriod)
	{
		int fp_period = oMathFP.toFP(speriod);
		int period = oMathFP.toInt(oMathFP.mul(fp_period, oMathFP.toFP(100)));
		memory[offset+0] = (byte)((period >>> 8) & 0xFF);
		memory[offset+1] = (byte)( period        & 0xFF);
	}

    /**********************************************************************/
    /// Calculate byte representation (percentage of 12V).
    private final static int percentage_of_voltage12(String s)
    {
        int fp_voltage = oMathFP.toFP(s);
        int fp_percentage = oMathFP.div(oMathFP.mul(fp_voltage, oMathFP.toFP(25)), oMathFP.toFP(3));
        int percentage = oMathFP.toInt(oMathFP.round(fp_percentage));
        return percentage;
    }

	/**********************************************************************/
	/** Set the configuration array.
     * If spi_present is set, the voltage is fixed to 12V.
     */
    public final boolean setConfig(
            String[]        config,
            StringBuffer	error_buffer,
            boolean         spi_present
        ) throws StopException
	{
		boolean ok = true;

        IOPanel.SetShutdown();
		try {
			ok = ok && read(ADDRESS_BLOCK_PARAMETERS, 27, 5, error_buffer);
			ok = ok && read(ADDRESS_BLOCK_BLINKING, 32, 5, error_buffer);
			ok = ok && read(ADDRESS_BLOCK_PARAMETERS_2, 33, 5, error_buffer);
			if (!ok)
			{
				return false;
			}

			byte[]	parameters = makeCopy(ADDRESS_BLOCK_PARAMETERS, 27);
			byte[]	blinking = makeCopy(ADDRESS_BLOCK_BLINKING, 32);
			byte[]  parameters2 = makeCopy(ADDRESS_BLOCK_PARAMETERS_2, 33);

            for (int configIndex = 0; configIndex < config.length; ++configIndex)
            {
                String[] xparam = Utils.split(config[configIndex], '=');
				if (xparam.length>=2) {
					if (xparam[0].equals("1")) {
                        if (spi_present)
                        {
                            parameters[3] = 100; // 12V.
                        }
                        else
                        {
                            // Voltage.
                            int percentage = percentage_of_voltage12(xparam[1]);
                            xlog("voltage=" + xparam[1] + " percentage=" + percentage, error_buffer);
                            parameters[3] = (byte)percentage;
                        }
					} else if (xparam[0].equals("2")) {
						// Blinker periods.
						String[]	speriods = Utils.split(xparam[1], ':');
						int			n = speriods.length / 2;
						if (n>0) {
							// bperiods - byte buffer.
							update_blink(parameters, 8, speriods[2*n-1]);
							blinking[0] = 0x01;
							for (int i=0; i<2*n-1; ++i) {
								update_blink(blinking, 2 * i + 1, speriods[i]);
							}
							int	last_off = 4*n-1;
							blinking[last_off + 0] = 0x00;
							blinking[last_off + 1] = 0x00;
							int	cnt = 0;
							for (int i = last_off + 2; i < blinking.length; ++i)
							{
								blinking[i] = (byte)0xFF;
								if (cnt<16) {
									switch (cnt % 4) {
									case 0:	// fallthrough
									case 1:
										blinking[i] = (byte)0xFF;
										break;
									case 2: // fallthrough
									case 3:
										blinking[i] = (byte)0x00;
										break;
									}
								}
								++cnt;
							}
							// Update stuff in memory.
						}
					} else if (xparam[0].equals("11")) {
						// Photocell threshold at 0xB600 + 0x0E.
						int fp_voltage		= oMathFP.toFP(xparam[1]);
						int fp_percentage	= oMathFP.div(oMathFP.mul(fp_voltage, oMathFP.toFP(255)), oMathFP.toFP(5));
						int	byte_value		= oMathFP.toInt(oMathFP.round(fp_percentage));
						xlog("threshold=" + xparam[1] + " byte_value=" + byte_value, error_buffer);
						parameters[14] = (byte)byte_value;
					}
					else if (xparam[0].equals("21"))
					{
						// Charger Cut-In voltage at 0xB7D0 + 0x13.
						int percentage = percentage_of_voltage12(xparam[1]);
						xlog("cut-in=" + xparam[1] + " percentage=" + percentage, error_buffer);
						parameters2[0x13] = (byte)percentage;
					}
					else if (xparam[0].equals("22"))
					{
						// Charger Cut-Out voltage at 0xB7D0 + 0x14.
						int percentage = percentage_of_voltage12(xparam[1]);
						xlog("cut-out=" + xparam[1] + " percentage=" + percentage, error_buffer);
						parameters2[0x14] = (byte)percentage;
					}
				}
			}

			ok = ok && write(ADDRESS_BLOCK_PARAMETERS, parameters, error_buffer);
			ok = ok && write(ADDRESS_BLOCK_BLINKING, blinking, error_buffer);
			ok = ok && write(ADDRESS_BLOCK_PARAMETERS_2, parameters2, error_buffer);
		} finally {
            IOPanel.ClearShutdown();
		}
        return true;
	}

    /**********************************************************************/
	/** Set the memory. */
    public final boolean setMemory( String[]        memory_commands,
                                    StringBuffer    error_buffer) throws StopException
    {
        IOPanel.SetShutdown();
		try {
            // do what?
            for (int memoryIndex = 0; memoryIndex < memory_commands.length; ++memoryIndex)
            {
                String[] xparam = Utils.split(memory_commands[memoryIndex], ':');
                if (xparam.length >= 2)
                {
                    int addr = Integer.parseInt(xparam[0], 16);
                    int length = xparam[1].length() / 2;
                    if (2*length == xparam[1].length() && (
                        (addr==0xB600 && length<=27) ||
                        (addr==0xB61B && length<=48) ||
                        (addr==0xB63B && length<=32) ||
                        (addr==0xB7D0 && length<=33)) ) {

                        // buffer - fill it.
                        byte[] buffer = new byte[length];
                        for (int i = 0; i < length; ++i)
                        {
                            int xaddr = addr + i;
                            if (xaddr == 0xB611 || xaddr == 0xB612)
                            {
                                // serial number.
                                buffer[i] = memory[xaddr];
                            }
                            else
                            {
                                String s_data = xparam[1].substring(2 * i, 2 * i + 2);
                                buffer[i] = (byte)(Integer.parseInt(s_data, 16));
                            }
                        }

                        // Upload.
                        if (write(addr, buffer, error_buffer))
                        {
                            // Write OK, copy contents back to 
                            read(addr, length, 5, error_buffer);
                        }
                    }
                }
            }
        }
        finally
        {
            IOPanel.ClearShutdown();
        }
        return true;
    }

	/**********************************************************************/
	/** Convert memory[idx .... idx+1] (time units of 0.01) into seconds, two places after comman. */
	private final String string_of_time(	int	index,
											int	nbytes)
	{
		int	mytime = Utils.uint_of_bytes(memory, index, nbytes);
		return Utils.string_of_fp100(mytime);
	}

	/**********************************************************************/
	/** String representation of a byte in memory, multiplied and divided.
	 */
	private final String string_of_memory(	int		index,
											int		nbytes,
											int		multiplier,
											int		divider,
											int		ndecimals)
	{
		int	fp_memory	= oMathFP.toFP(Utils.uint_of_bytes(memory, index, nbytes));
		int	fp_result	= oMathFP.div(oMathFP.mul(fp_memory, oMathFP.toFP(multiplier)), oMathFP.toFP(divider));
		return oMathFP.toString(fp_result, ndecimals, ndecimals);
	}

	/**********************************************************************/
	/** Convert percentage of 12V into voltage string, one decimal place. */
	private final String string_of_voltage(	int	index)
	{
		return string_of_memory(index, 1, 3, 25, 1);
	}

	/**********************************************************************/
	/** Get the configuration array. */
	public final String[] getConfig() throws StopException
	{
		String	Lamp_Voltage = string_of_voltage(ADDRESS_BLOCK_PARAMETERS + 3);

		String	Blinking_Pattern = "";
		{
			StringBuffer	sb = new StringBuffer();
			for (int i=0; i<8; ++i) {
				int		idx = ADDRESS_BLOCK_BLINKING + 1 + i*4;
				boolean	is_last	= memory[idx+2]==0 && memory[idx+3]==0;

				sb.append(string_of_time(idx, 2));
				sb.append(':');

				if (is_last) {
					sb.append(string_of_time(ADDRESS_BLOCK_PARAMETERS + 8, 2));
				} else {
					sb.append(string_of_time(idx+2, 2));
					sb.append(':');
				}

				if (is_last) {
					break;
				}
			}

			Blinking_Pattern = sb.toString();
		}
		String	Blinking_Enabled = memory[ADDRESS_BLOCK_BLINKING+0]==0 ? "N" : "Y";

		String	Battery_Voltage_Min				= string_of_voltage(ADDRESS_BLOCK_PARAMETERS + 12);
		String	Battery_Hysteresis				= string_of_voltage(ADDRESS_BLOCK_PARAMETERS + 13);
		String	Photocell_Threshold				= string_of_memory(ADDRESS_BLOCK_PARAMETERS+14, 1,  5, 256, 1);
		String	Serial_No						= String.valueOf(
													Utils.uint_of_bytes(
														memory[ADDRESS_BLOCK_PARAMETERS+17],
														memory[ADDRESS_BLOCK_PARAMETERS+18]));
		String	Charger_CutIn_Voltage			= string_of_voltage(ADDRESS_BLOCK_PARAMETERS_2 + 19);
		String	Charger_CutOut_Voltage			= string_of_voltage(ADDRESS_BLOCK_PARAMETERS_2 + 20);

		// FIXME: implement this.
		return new String[] {
			"1="+Lamp_Voltage,
			"2="+Blinking_Pattern,
			"3="+Blinking_Enabled,
			"9="+Battery_Voltage_Min,
			"10="+Battery_Hysteresis,
			"11="+Photocell_Threshold,
			"14="+Serial_No,
			"21="+Charger_CutIn_Voltage,
			"22="+Charger_CutOut_Voltage,
		};
	}

	/**********************************************************************/
	/** Get the sensors-counters array. */
	public final String[] getSensorsCounters() throws StopException
	{
		// FIXME: implement this.
		
		String	Battery_Voltage_Loaded	= string_of_voltage(6);
		String	Battery_Voltage_Open	= string_of_voltage(7);
		String	Filament1_OnTime		= string_of_time(19, 4);
		String	Filament2_OnTime		= string_of_time(23, 4);
		String	Charge_Current			= string_of_memory(0x0040 + 8,  2, 1, 1000, 3);
		String	Charged_Ampere_Hours	= Utils.string_of_fp100(Utils.uint_of_bytes(memory, 0x0040 + 10, 4) / 36);
											// string_of_memory(0x0040 + 10, 4, 1, 3600, 2);
// int	mytime = Utils.uint_of_bytes(memory, index, nbytes);
// return Utils.string_of_fp100(mytime);
		String	Flasher_Charging_Status	= Charge_Current.equals("0.000") ? "N" : "Y";
		String	Flasher_Temperature;
		{
			// temp = 0.335*X - 17.712.
			int	reading1000 = Utils.uint_of_bytes(memory, 5, 1) * 1000;
			int	result1000	= reading1000 * 335 / 1000 - 17712;
			int	fp_result	= oMathFP.div(oMathFP.toFP(result1000), oMathFP.toFP(1000));
			Flasher_Temperature = oMathFP.toString(fp_result, 1, 1);
		}
		String	Flasher_Error_Code		= String.valueOf(Utils.uint_of_byte(memory[0x0040 + 2]));
		String	Photocell_Level			= string_of_memory(0x002F, 1, 5, 256, 2);

		return new String[] {
			"2="+Battery_Voltage_Loaded,
			"3="+Battery_Voltage_Open,
			"6="+Filament1_OnTime,
			"7="+Filament2_OnTime,
			"9="+Charge_Current,
			"10="+Charged_Ampere_Hours,
			"11="+Flasher_Charging_Status,
			"12="+Flasher_Temperature,
			"13="+Flasher_Error_Code,
			"15="+Photocell_Level
		};
	}

	/** Read time from SmartFlasher. */
	public MyTime readTime(	StringBuffer error_buffer) throws StopException
	{
		MyTime	r = null;

        IOPanel.SetShutdown();
		try {
			/*
			Calendar	cal = Calendar.getInstance();
			cal.set(Calendar.YEAR, 2006);
			cal.set(Calendar.MONTH, Utils.monthOf1to12(12));
			cal.set(Calendar.DAY_OF_MONTH, 29);
			cal.set(Calendar.HOUR, 16);
			cal.set(Calendar.MINUTE, 18);
			cal.set(Calendar.SECOND, 33);
			return cal.getTime();
			*/

			// Build the command.
			String	scommand = "T\r";
			byte[]	command = scommand.getBytes();
			int		nretries = 10;
			int		min_read_length = 14;


			// Retry up to 5 times, timeout for each query is approx. 1 second?

			for (int retryCount=0; retryCount<nretries; ++retryCount) {
				long start_time = System.currentTimeMillis();
				long end_time = start_time + READ_TIMEOUT;
				try {
					// Write the line.
                    IOPanel.ToggleSerialOutTraffic();
					line_out_.write(command, 0, command.length);
					start_time = System.currentTimeMillis();
					end_time = start_time + READ_TIMEOUT;
					error_buffer.append("Start time: " + start_time + " end time: " + end_time + "\n");
					error_buffer.append("Sent line: " + scommand + "\n");

					// Check responses until timeout.
					do {
						if (linereader_.length()>0) {
							String[]	lines = linereader_.popAll();
							for (int lineIndex=0; lineIndex<lines.length; ++lineIndex) {
								String line = lines[lineIndex];
								if (line == null) {
									error_buffer.append("Got ping. (" + (System.currentTimeMillis() - start_time) + ")\n");
									continue;
								}
								error_buffer.append("Got line: " + line + "(" + (System.currentTimeMillis() - start_time) + ")\n");
								if (line.length()<min_read_length) {
									xlog("wrong length:" + line.length(), error_buffer);
									continue;
								}

								try {
									r = Utils.makeTimeOfSmartFlasher(line);
								} catch (RuntimeException e) {
									// pass.
								}

								if (r != null) {
									flushInput();
									return r;
								}
							}
						}
					} while (end_time > System.currentTimeMillis());
                } catch (StopException ex) {
                    throw ex;
				} catch (Exception ex) {
					xlog("Exception: " + ex.toString(), error_buffer);
					// pass the time.
					Globals.sleep(100);
				}
			}
			flushInput();
		} finally {
            IOPanel.ClearShutdown();
		}
		return r;
	}
} // class Lampmemory

