// vim: shiftwidth=4
// vim: ts=4
package com.errapartengineering.LampController;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;

import java.util.Vector;
import java.util.Date;

import com.nokia.m2m.imp.iocontrol.IOControl;
import com.nokia.m2m.orb.idl.terminal.ET;
import com.nokia.m2m.orb.idl.terminal.ETHelper;
import com.nokia.m2m.imp.watchdog.WatchdogTimer;

import javax.microedition.io.Connector;
import javax.microedition.io.StreamConnection;
import javax.microedition.midlet.MIDlet;
import javax.microedition.midlet.MIDletStateChangeException;

/**
 * LampController stuff.
 */
public class LampController extends MIDlet implements Runnable {

	private Thread						thread_			= null;

	/// Server connection ping period, milliseconds.
	private final static long			SERVER_PING_PERIOD	= 120 * 1000;
	/// Server connection timeout, milliseconds.
	private final static long			SERVER_PING_TIMEOUT	= 3 * SERVER_PING_PERIOD;

	// IMlet uses serial port 3
	private final static int			SERIAL_PORT_SPEED = 1200;

	// Size of communication buffers.
	private final static int			BUFFER_SIZE = 4096;

	// Communication Box debugging data.
	private final static String			commbox_prefix = "cbox:";

	// Device IMEI.
	private String						IMEI = "123456789";

	private InputStream					serial_in = null;
	private OutputStream				serial_out = null;
	private StreamConnection			serial_connection = null;

	private SmartFlasher				smartflasher = null;
    private PowerlightSPI               powerlight_spi = null;

	// Watching DOG.
	private WatchdogTimer				watchdog = null;

	/**********************************************************************/
	/**
	 * Start the LampController reading thread.
	 * 
	 */
	public void startApp() throws MIDletStateChangeException {
        Globals.StopFlagClear();
        IOPanel.ServerConnected(false);

		// Open serial port.
		try {
			serial_connection = (StreamConnection) Connector.open(
					"comm:3;baudrate=" + SERIAL_PORT_SPEED, Connector.READ_WRITE);
			serial_in = serial_connection.openInputStream();
			serial_out = serial_connection.openOutputStream();
			log("Serial port open");
		} catch (Exception e) {
			// Oops, will not continue.
			log(e.toString());
			throw new MIDletStateChangeException("Error:" + e.toString());
		}


		if (BuildInfo.powerLight) {
			powerlight_spi = new PowerlightSPI();
		}

		// Setup watchdog.
		if (watchdog == null) {
			watchdog = new WatchdogTimer();
		}
		watchdog.setTimeout(15*60);	// 15 minutes.

		// Setup smartflasher.
		if (smartflasher == null) {
			smartflasher = new SmartFlasher(serial_in, serial_out);
		}

		// Query IMEI.
		{
			org.omg.CORBA.ORB		orb = org.omg.CORBA.ORB.init(null, null);
			// Resolve the Device object.
			org.omg.CORBA.Object	ref = orb.string_to_object("corbaloc::127.0.0.1:19740/ORB/OA/IDL:ET:1.0");
			ET et = ETHelper.narrow(ref);
			String imei = et.IMEI();

			if (imei!=null && imei.length()>0) {
				IMEI = imei;
			}

			// Close the ORB.
			orb.destroy();
		}

		// Start the thread.
		thread_ = new Thread(this);
		thread_.start();
	}

	/**********************************************************************/
	/**
	 * Write log message to serial port
	 * 
	 * @param msg
	 */
	private void log(String msg) {
		// Comment off these rows to remove logging to serial port

		if (BuildInfo.logToSerial) {
			msg = "[" + msg + "]\r";
			// can't log to serial port
			if (serial_out != null) {
				try {
                    IOPanel.ToggleSerialOutTraffic();
					serial_out.write(msg.getBytes());
				} catch (Exception ignored) {
				}
			}
		}
	}

	/**********************************************************************/
	private void log(Exception e)
	{
		if (BuildInfo.logToSerial) {
			if (serial_out != null) {
				try {
					e.printStackTrace();
				} catch (Exception ignored) {
					log("OUCH");
				}
			}
		}
	}

	/**********************************************************************/
	private void stop()
	{
		// Signal stop to this thread.
        Globals.StopFlagSet();

		// Wait for this thread to die.
		if (thread_!=null) {
			Utils.join(thread_);
			thread_ = null;
		}
		smartflasher = null;

        if (powerlight_spi != null)
        {
            Utils.join(powerlight_spi);
            powerlight_spi = null;
        }

		// Close serial port, if left open.
        if (serial_connection != null)
        {
            try
            {
                serial_connection.close();
            }
            catch (Exception e)
            {
            }
        }
        serial_connection = null;
        serial_in = null;
        serial_out = null;

		// Disable watchdog, if any.
		if (watchdog != null)
		{
			watchdog.setTimeout(0);
            watchdog = null;
        }
	}

	/**********************************************************************/
	public void pauseApp()
	{
		stop();
	}

	/**********************************************************************/
	public void destroyApp(boolean arg0) throws MIDletStateChangeException
	{
		stop();
	}

	/**********************************************************************/
	/** Emit message to the server. */
	private final void send(	OutputStream	server_out,
								String			message) throws IOException
	{
		if (server_out != null) {
			CMR.emit(server_out, CMR.LAMPNET, message);
		}
	}

	/**********************************************************************/
	/** Emit error message towards the server. */
	private final void sendError(	OutputStream	server_out,
									String			error_text) throws IOException
	{
		CMR.emit(server_out, CMR.LAMPNET, "error = " + error_text);
	}

	/**********************************************************************/
	/** Emit message towards the server. */
	private final void sendLog(	OutputStream	server_out,
								StringBuffer	sb) throws IOException
	{
		CMR.emit(server_out, CMR.LAMPNET, "log = " + sb.toString());
	}

	/**********************************************************************/
	/** Emit current configuration. */
	private final void sendConfig(	OutputStream	server_out,
									boolean			reload_smartflasher) throws StopException, IOException
	{
		if (smartflasher==null) {
				sendError(server_out, "nosmartflasher");
		} else {
			if (reload_smartflasher || !smartflasher.isLinkOK()) {
				StringBuffer	sb = new StringBuffer();
				if (!smartflasher.reload(sb)) {
					sendError(server_out, "nolink");
					sendLog(server_out, sb);
                    IOPanel.ClearShutdown();
					return;
				}
			}

			if (smartflasher.isLinkOK()) {
				String[] config = smartflasher.getConfig();
				send(server_out, "config = " + Utils.join(config, " "));
			} else {
				sendError(server_out, "nolink");
			}
		}

        IOPanel.ClearShutdown();
	}

	/**********************************************************************/
	/** Emit sensors and counters info. */
	private final void sendSensors(	OutputStream	server_out) throws StopException, IOException
	{
		if (smartflasher == null) {
				sendError(server_out, "nosmartflasher");
		} else {
			StringBuffer	sb = new StringBuffer();
			if (smartflasher.reload(sb)) {
				String[] sensorscounters = smartflasher.getSensorsCounters();
				send(server_out, "sensorscounters = " + Utils.join(sensorscounters, " "));
			} else {
				sendError(server_out, "nolink");
				sendLog(server_out, sb);
			}
		}

        IOPanel.ClearShutdown();
	}

    /**********************************************************************/
    /** Emit sensors and counters info. */
    private final void sendMemory(OutputStream server_out) throws StopException, IOException
    {
        if (smartflasher == null)
        {
            sendError(server_out, "nosmartflasher");
        }
        else
        {
            StringBuffer sb = new StringBuffer();
            if (smartflasher.reload(sb))
            {
                CMR.emit(server_out, CMR.LAMPNET, "MEMORY = " + smartflasher.memoryString());
            }
            else
            {
                CMR.emit(server_out, CMR.LAMPNET, "Reload failure, error=" + sb.toString());
            }
        }

        IOPanel.ClearShutdown();
    }

    /**********************************************************************/
    private final void sendIntensities(
        OutputStream server_out,
        int intensities[],
        String name
        ) throws IOException
    {
        String msg = name + " =" +
            " 1=" + intensities[0] +
            " 2=" + intensities[1] +
            " 3=" + intensities[2] +
            " 4=" + intensities[3];
        send(server_out, msg);
        log(msg);
    }

    /**********************************************************************/
	private final void shutDown(	OutputStream	server_out) throws StopException, IOException
	{
		if (BuildInfo.shutDown) {
			log("shutdown");
			send(server_out, "shutdown in 35 seconds");
			Globals.sleep(30 * 1000);
			send(server_out, "shutdown in 5 seconds.");
            IOPanel.SignalShutdown(smartflasher);
		} else {
			log("shutdown invalid");
		}
	}

	/**********************************************************************/
	/** Handle the set configuration command.
	argv = arguments.
	*/
	private void handle_set_config(
        InputStream		server_in,
        OutputStream	server_out,
        String[]		argv
        ) throws IOException, StopException
	{
		if (smartflasher == null) {
			sendError(server_out, "nosmartflasher");
		} else {
			boolean			ok = true;
			StringBuffer	sb = new StringBuffer();
			try {
				ok = smartflasher.setConfig(argv, sb, powerlight_spi!=null);
			} catch (Exception e) {
                IOPanel.ClearShutdown();
				CMR.emit(server_out, CMR.LAMPNET, "ERROR: "+e.toString());
				return;
			}
            IOPanel.ClearShutdown();

			if (Globals.StopFlagGet()) {
				return;
			}

			if (ok) {
				CMR.emit(server_out, CMR.LAMPNET, "OK");
				CMR.emit(server_out, CMR.LAMPNET, "LOG="+sb.toString());
			} else {
				CMR.emit(server_out, CMR.LAMPNET, "Configuration failure, error log="+sb.toString());
			}

			sendConfig(server_out, false);
		}
	}

	/**********************************************************************/
	/** Handle the set memory command.
	argv = arguments.
	*/
    private void setMemory(
        InputStream     server_in,
        OutputStream    server_out,
        String[]        argv
        ) throws IOException, StopException
    {
        if (smartflasher == null)
        {
            sendError(server_out, "nosmartflasher");
        }
        else
        {
            StringBuffer sb = new StringBuffer();
            boolean ok = smartflasher.setMemory(argv, sb);

            if (ok)
            {
                CMR.emit(server_out, CMR.LAMPNET, "OK");
                sendMemory(server_out);
            }
            else
            {
                CMR.emit(server_out, CMR.LAMPNET, "Memory setting failure, error log=" + sb.toString());
            }
        }
    }

	/**********************************************************************/
	private static String okchars = "ABCDEFGHIJKLMNOPQRSTUWXYZabcdefghijklmnopqrstuwxyz0123456789[]{}\\';:./,<>?!@#$%^^&*()_+- =";
	/**********************************************************************/
	private static void appendEscaped(	StringBuffer	sb,
										byte			b)
	{
		char	c = Utils.char_of_byte(b);
		if (okchars.indexOf(c)>=0) {
			sb.append(c);
		} else {
			int ci = b>=0 ? (int)b : (256 + b);
			sb.append("\\x");
			sb.append(Utils.hex_of_uint8(ci));
		}
	}

    /**********************************************************************/
    private final void handle_lampnet_packet(
        InputStream     server_in,
        OutputStream    server_out,
        String          data_block
        ) throws IOException, StopException
    {
	    // Split it up.
	    String[] argv = Utils.split(data_block, ' ');
	    log("argc: " + argv.length);
	    for (int i=0; i<argv.length; ++i) {
		    log("argv[" + i + "]:" + argv[i]);
	    }

	    if (argv.length<1) {
		    CMR.emit(server_out, CMR.LAMPNET, "Command too short.");
            return;
	    }

	    String	cmd = argv[0];
	    String	op = argv.length>=2 ? argv[1] : "";
	    if (cmd.equals("config")) {
		    if (argv.length>=2) {
			    if (op.equals("=")) {
				    // String[]	real_argv = new String[argv.length-2];
				    // for (int i=2; i<argv.length; ++i) {
					//    real_argv[i-2] = argv[i];
				    // }
				    handle_set_config(server_in, server_out, Utils.chopPrefix(argv, 2));
			    } else if (op.equals("?")) {
				    sendConfig(server_out, true);
			    } else {
				    send(server_out, "Operation unrecognized.");
			    }
		    } else {
			    sendError(server_out, "Too few parameters.");
		    }
	    } else if (cmd.equals("sensorcounter")) {
		    if (argv.length>=2) {
			    if (op.equals("=")) {
				    sendError(server_out, "Can't set sensors-counters.");
			    } else if (op.equals("?")) {
				    sendSensors(server_out);
			    } else {
				    sendError(server_out, "Operation unrecognized.");
			    }
		    } else {
			    sendError(server_out, "Too few parameters.");
		    }
	    } else if (cmd.equals("shutdown") && BuildInfo.shutDown) {
		    shutDown(server_out);
	    } else if (cmd.equals("close")) {
		    log("close");
		    send(server_out, "close");
		    throw new IOException("close");
		    // close connection.
	    } else if (cmd.equals("time") && argv.length>=2 && op.equals("?")) {
		    if (smartflasher == null) {
			    sendError(server_out, "nosmartflasher");
		    } else {
			    StringBuffer	sb = new StringBuffer();
			    MyTime			mt = smartflasher.readTime(sb);
			    if (mt==null) {
				    // oops.
				    sendError(server_out, "timeout");
				    sendLog(server_out, sb);
			    } else {
				    send(server_out, "time = \"" + mt.ToString() + "\"");
				    sendLog(server_out, sb);
			    }
		    }
	    } else if (cmd.equals("shouldrun") && argv.length>=2 && op.equals("?")) {
		    if (smartflasher == null) {
			    sendError(server_out, "nosmartflasher");
		    } else {
			    StringBuffer	sb = new StringBuffer();
			    MyTime			mt = smartflasher.readTime(sb);
			    if (mt==null) {
				    // oops.
				    sendError(server_out, "timeout");
				    sendLog(server_out, sb);
			    } else {
				    send(server_out, "time = \"" + mt.ToString() + "\"");
				    sendLog(server_out, sb);
				    boolean should_run = Utils.shouldRun(mt);
				    send(server_out, "shouldrun = " + (should_run ? "true" : "false"));
			    }
		    }
        } else if (cmd.equals("memory") && argv.length>=2) {
            if (argv.length >= 2)
            {
                if (op.equals("="))
                {
                    String[] real_argv = new String[argv.length-2];
                    for (int i = 2; i < argv.length; ++i)
                    {
                        real_argv[i - 2] = argv[i];
                    }
                    setMemory(server_in, server_out, real_argv);
                }
                else if (op.equals("?"))
                {
                    sendMemory(server_out);
                }
                else
                {
                    sendError(server_out, "Operation unrecognized.");
                }
            }
            else
            {
                sendError(server_out, "Too few parameters.");
            }
        }
        else if (cmd.equals("powerlight") && powerlight_spi!=null)
        {
            if (argv.length >= 2)
            {
                if (op.equals("?"))
                {
                    int intensities[] = new int[4];
                    powerlight_spi.getIntensities(intensities);
                    sendIntensities(server_out, intensities, "getpowerlight");
                }
                else if (op.equals("="))
                {
                    powerlight_spi.setIntensities(Utils.chopPrefix(argv, 2));
                }
            }
            else
            {
                sendError(server_out, "Too few parameters.");
            }
        }
        else
        {
            CMR.emit(server_out, CMR.LAMPNET, "Command unrecognized.");
        }
    }

	/**********************************************************************/
	/** Process the connection to the server. Don't worry about exceptions. */
	private final void handle_connection(	InputStream		server_in,
											OutputStream	server_out,
											StringBuffer	startup_readtime_log) throws IOException, StopException
	{
		// Initialize...
		{
			send(server_out, "lampcontroller = 1=LampController 2=1.0 3=\"" + BuildInfo.buildDate + "\" 4=\"" + IMEI + "\" 5=\""+BuildInfo.compilationArguments + "\"");
            sendMemory(server_out);

			if (Globals.StopFlagGet()) {
				return;
			}

			sendConfig(server_out, false);
			sendSensors(server_out);
			sendLog(server_out, startup_readtime_log);
		}

		long			received_ping_time	= System.currentTimeMillis();
		byte[]			inbuffer = new byte[128];
		CMRAssembler	assembler = new CMRAssembler();
		int				available;
		StringBuffer	sb_serial = new StringBuffer();
        int             intensities[] = new int[4];

		// Loop until timeout
		while (!Globals.StopFlagGet() && received_ping_time + SERVER_PING_TIMEOUT > System.currentTimeMillis()) {
			// 1. Do the TCP stuff.
			available = server_in.available();
			if (available>0) {
				// Read data and feed it.
				if (available>inbuffer.length) {
					available = inbuffer.length;
				}
				int nread = server_in.read(inbuffer, 0, available);
				log("Received: " + nread + " bytes.");
				if (nread>0) {
                    IOPanel.ToggleServerTraffic();

                    // Timeout records.
					received_ping_time = System.currentTimeMillis();

					// Feed the cat.
					assembler.feed(inbuffer, nread);

					// Handle packets.
					for (CMR cmr = assembler.pop(); cmr!=null; cmr = assembler.pop()) {
						log(cmr.toString());
						switch (cmr.type) {
						case CMR.PING:
							log("PING received, watchdog reset.");
							CMR.emit(server_out, CMR.PING, "");
							watchdog.resetTimer();
							break;
						case CMR.LAMPNET:
                            handle_lampnet_packet(server_in, server_out, new String(cmr.data, 0, cmr.data.length));
							break;
						default:
							break;
						}
					}
				}
			}

			/* FIXME: how to detect blinker powerons/powerdowns?
			 */
			/*
			if (smartflasher != null) {
				smartflasher.flushInput();
			}
			*/

            // 2. Do the serial stuff.
			available = serial_in.available();
			if (available>0) {
				if (available>inbuffer.length) {
					available = inbuffer.length;
				}
				int nread = serial_in.read(inbuffer, 0, available);
				for (int i=0; i<nread; ++i) {
					byte	b = inbuffer[i];
					if (b==0x0D || b==0x0A) {
						// CR or LF
						if (sb_serial.length()>0) {
							String	s = sb_serial.toString();
							if (s.indexOf(commbox_prefix)==0) {
								send(server_out, "commbox = " + s.substring(commbox_prefix.length()));
							} else {
								send(server_out, "commbox = " + s);
							}
							sb_serial.setLength(0);
						}
					} else {
						int	bb = (b>=0 ? (int)b : (256 + b));
						if (bb!=0x00 && bb!=0xEC) {
							appendEscaped(sb_serial, b);
						}
					}
				}
			}

            // 3. Check power light, if any.
            if (powerlight_spi != null)
            {
                if (powerlight_spi.popIntensities(intensities)) {
                    sendIntensities(server_out, intensities, "poppowerlight");
                }
            }
		}

		long current_time = System.currentTimeMillis();
		long dt = current_time - received_ping_time;
		send(server_out, "DISCONNECTED dt="+dt+
				" last time:" + Utils.formatDate(received_ping_time) +
				" current time:" + Utils.formatDate(current_time));
	}

	/**********************************************************************/
	public void run()
	{
		StringBuffer	startup_readtime_log = new StringBuffer();
		/** Check if we are run too frequently. */
		if (BuildInfo.shutDown) {
			MyTime	mt = null;
			if (smartflasher != null) {
				try {
					mt = smartflasher.readTime(startup_readtime_log);
					if (mt!=null && !Utils.shouldRun(mt)) {
						Globals.sleep(35 * 1000);
						shutDown(null);
					}
				} catch (IOException e) {
					// pass.
				} catch (StopException ex) {
					log("stopped prematurely");
					return;
				}
			}
			log("will run.");
		}

        IOPanel.ClearShutdown();

		// Keep alive connection to the server, watch pings and timeouts.
		InputStream					server_in = null;
		OutputStream				server_out = null;
		StreamConnection			server_connection = null;
		while (!Globals.StopFlagGet()) {
			try {
				// Connect to the server.
                String server_name = BuildInfo.server;
				log("Connecting to:" + server_name);
                server_connection = (StreamConnection)Connector.open("socket://" + server_name);
				server_in = server_connection.openInputStream();
				server_out = server_connection.openOutputStream();

                IOPanel.ServerConnected(true);
				handle_connection(server_in, server_out, startup_readtime_log);
			} catch (IOException e) {
				log("Connect failed:"+e.toString());
				// Uh, connection dropped, oh no.
			} catch (Exception e) {
				// Exception, bad!
				log(e.toString());
				try {
					send(server_out, "EXCEPTION "+e.toString());
				} catch (IOException e2) {
					// pass.
				}
			} catch (StopException e) {
				try {
					send(server_out, "STOPPED BY REMOTE CONTROL.");
				} catch (IOException e2) {
					// pass.
				}
                Globals.StopFlagSet();
			}

			// Close all the connections.
            if (server_connection != null)
            {
				try {
						server_connection.close();
				} catch (Exception ignored) {
				}
			}
            server_connection = null;
            server_in = null;
            server_out = null;
            IOPanel.ServerConnected(false);

			try
			{
				// wait 10 secs between attempting to connect to the second server.
				for (int i = 0; i < 10; ++i)
				{
					Globals.sleep(1000);
					SmartFlasher sf = smartflasher;
					if (sf == null) {
						break;
					} else {
						sf.flushInput();
					}
				}
			}
			catch (StopException ex)
			{
				// that's all folks.
				return;
			}

			try {
				shutDown(null);
			} catch (IOException ex) {
				// pass.
			} catch (StopException ex) {
				// that's all folks.
				return;
			}
		} // main loop.
	}
}

