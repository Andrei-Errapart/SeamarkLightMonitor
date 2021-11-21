// vim: shiftwidth=4 
// vim: ts=4 

package com.errapartengineering.LampController;

import com.nokia.m2m.imp.iocontrol.IOControl;
import java.io.IOException;

// IO panel.
public final class IOPanel {
    private final static int COMMAND_WRITE = 0x63;
    private final static int COMMAND_READ = 0x6C;

    /// Shutdown output pin
    private final static int SHUTDOWN_PORT = 2;
    private final static int PIN_CS = 7;
    private final static int PIN_CLK = 8;
    private final static int PIN_MOSI = 9;
    // private final static int PIN_MISO = 11;
    private final static int APIN_MISO = 1;

    /// Toggle pin.
    private final static void TogglePin(int pin)
    {
        try {
        
            IOControl ioc = IOControl.getInstance();
            boolean bit = ioc.getDigitalOutputPin(pin);
            ioc.setDigitalOutputPin(pin, !bit);
        }
        catch (IOException ex)
        {
            // pass.
        }
    }

    /// Set pin to given value.
    private final static void SetPin(int pin, boolean state)
    {
        try
        {
            IOControl.getInstance().setDigitalOutputPin(pin, state);
        }
        catch (IOException ex)
        {
            // pass.
        }
    }

    /// Get digital input pin value.
    private final static boolean GetPin(int pin)
    {
        boolean r = false;
        try
        {
            r = IOControl.getInstance().getDigitalOutputPin(pin);
        }
        catch (IOException ex)
        {
            // pass.
        }
        return r;
    }

    /// Get analog input pin value.
    private final static int GetAnalogPin(int pin)
    {
        int r = 0;
        try
        {
            r = IOControl.getInstance().getAnalogInputPin(pin);
        }
        catch (IOException ex)
        {
            // pass.
        }
        return r;
    }

    /** Checksum algorithm (CRC-8, see http://www.maxim-ic.com/appnotes.cfm/appnote_number/27 for further reference)
     */
    private final static int _crc_ibutton_update(int crc, int data) 
    {
		data = data & 0xFF;
        crc = crc ^ data; 
        for (int i = 0; i < 8; i++) { 
            crc = (crc >> 1) ^ ((crc & 0x01) * 0x8C); 
        } 
        return crc & 0xFF;
    }

    private final static void ClockByteOut(int b) throws StopException
    {
        int mask = 1;
        for (int i = 0; i < 8; ++i)
        {
            SetPin(PIN_MOSI, (b & mask) != 0);
            Globals.sleep(1000);
            SetPin(PIN_CLK, true);
            Globals.sleep(1000);
            SetPin(PIN_CLK, false);

            mask = mask << 1;
        }
    }

    private final static int ClockByteIn() throws StopException
    {
        int mask = 1;
        int r = 0;
        for (int i = 0; i < 8; ++i)
        {
            Globals.sleep(1000);
            SetPin(PIN_CLK, true);
            Globals.sleep(1000);
            if (GetAnalogPin(APIN_MISO)>100)
            {
                r = r | mask;
            }
            SetPin(PIN_CLK, false);

            mask = mask << 1;
        }
        return r;
    }

    public final static void ClearShutdown()
    {
        SetPin(SHUTDOWN_PORT, false);
    }

    public final static void SetShutdown()
    {
        SetPin(SHUTDOWN_PORT, true);
    }

    public final static void SignalShutdown(SmartFlasher smartflasher) throws StopException, IOException
    {
        IOControl ioc = IOControl.getInstance();

        for (int i = 0; i < 5; ++i)
        {
            ioc.setDigitalOutputPin(SHUTDOWN_PORT, true);
            Globals.sleep(1000);
            if (smartflasher != null)
            {
                smartflasher.flushInput();
            }
            ioc.setDigitalOutputPin(SHUTDOWN_PORT, false);
            Globals.sleep(1000);
            if (smartflasher != null)
            {
                smartflasher.flushInput();
            }
        }
    }

    public final static void ServerConnected(boolean connected)
    {
        SetPin(3, connected);
    }

    public final static void ToggleServerTraffic()
    {
        TogglePin(4);
    }

    public final static void ToggleSerialInTraffic()
    {
        TogglePin(5);
    }

    public final static void ToggleSerialOutTraffic()
    {
        TogglePin(6);
    }

    /** Clock intensity out.
     */
    public final static void ClockIntensityOut(
        int CurrentIntensity,
        int DefaultIntensity
        ) throws StopException
    {
        int checksum =
            _crc_ibutton_update(
            _crc_ibutton_update(
            _crc_ibutton_update(0,
                COMMAND_WRITE),
                CurrentIntensity),
                DefaultIntensity);

        SetPin(PIN_CS, false);
        SetPin(PIN_CLK, false);
        SetPin(PIN_MOSI, false);
        Globals.sleep(1000);
        SetPin(PIN_CS, true);

        ClockByteOut(COMMAND_WRITE);
        ClockByteOut(CurrentIntensity);
        ClockByteOut(DefaultIntensity);
        ClockByteOut(checksum);

        SetPin(PIN_CLK, false);
        SetPin(PIN_MOSI, false);
        SetPin(PIN_CS, false);
    }

    /** Read two intensities from the Power Light.
     * @param[Intensities] On success index 0 will contain current intensity, and index 1 will contain default intensity.
     * @return true iff checksums match.
     */
    public final static boolean ClockIntensitiesIn(
        int[] Intensities
        ) throws StopException
    {
        SetPin(PIN_CS, false);
        SetPin(PIN_CLK, false);
        SetPin(PIN_MOSI, false);

        Globals.sleep(1000);
        SetPin(PIN_CS, true);

        ClockByteOut(COMMAND_READ);
        int current_intensity = ClockByteIn();
        int default_intensity = ClockByteIn();
        int checksum_received = ClockByteIn();
        int checksum_real =
            _crc_ibutton_update(
            _crc_ibutton_update(
            _crc_ibutton_update(0,
                COMMAND_READ),
                current_intensity),
                default_intensity);

        SetPin(PIN_CLK, false);
        SetPin(PIN_MOSI, false);
        SetPin(PIN_CS, false);

        if (checksum_real == checksum_received)
        {
            Intensities[0] = current_intensity;
            Intensities[1] = default_intensity;
            return true;
        }
        return false;
    }
} // class Build
