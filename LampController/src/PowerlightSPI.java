package com.errapartengineering.LampController;

/**
 * Continuously send intensity information to the PowerLight.
 */
public class PowerlightSPI extends Thread
{
    private int NewCurrentIntensity = -1;
    private int NewDefaultIntensity = -1;

    public int CurrentIntensity = -1;
    public int DefaultIntensity = -1;

    private int ReadCount = 0;
    private int LastReadCount = 0;

    /**********************************************************************/
    /** Creates new SPI communication channel to the Power Light.
     */
    public PowerlightSPI()
    {
        start();
    }

    /**********************************************************************/
    /** Set new intensities.
     */
    public final void setIntensities(
            int NewCurrent,
            int NewDefault
        )
    {
        if (NewCurrent > 0 && NewDefault > 0)
        {
            NewCurrentIntensity = NewCurrent;
            NewDefaultIntensity = NewDefault;
        }
    }

    /**********************************************************************/
    public final void setIntensities(
            String[] intensities
        )
    {
        int NewCurrent = -1;
        int NewDefault = -1;
        for (int intIndex = 0; intIndex < intensities.length; ++intIndex)
        {
            String[] xparam = Utils.split(intensities[intIndex], '=');
            if (xparam.length >= 2)
            {
                if (xparam[0].equals("1"))
                {
                    NewCurrent = Integer.parseInt(xparam[1]);
                }
                else if (xparam[0].equals("2"))
                {
                    NewDefault = Integer.parseInt(xparam[1]);
                }
            }
        }
        setIntensities(NewCurrent, NewDefault);
    }

    /**********************************************************************/
    private final void populateIntensities(
        int[] Intensities
        )
    {
        if (Intensities.length > 0)
        {
            Intensities[0] = CurrentIntensity;
        }
        if (Intensities.length > 1)
        {
            Intensities[1] = DefaultIntensity;
        }
        if (Intensities.length > 2)
        {
            Intensities[2] = NewCurrentIntensity;
        }
        if (Intensities.length > 3)
        {
            Intensities[3] = NewDefaultIntensity;
        }
    }
    /**********************************************************************/
    public final boolean popIntensities(
        int[] Intensities
        )
    {
        if (ReadCount != LastReadCount)
        {
            populateIntensities(Intensities);
            LastReadCount = ReadCount;
            return true;
        }
        return false;
    }

    /**********************************************************************/
    public final void getIntensities(
        int[] Intensities
        )
    {
        populateIntensities(Intensities);
    }

    /**********************************************************************/
    /** Run forewer.
     */
    public void run()
    {
        int[] IntensitiesRead = new int[2];
        try
        {
            while (!Globals.StopFlagGet())
            {
                int new_current_intensity = NewCurrentIntensity;
                int new_default_intensity = NewDefaultIntensity;
                if (new_current_intensity > 0 && new_default_intensity > 0)
                {
                    IOPanel.ClockIntensityOut(new_current_intensity, new_default_intensity);
                    Globals.sleep(2000);
                    if (IOPanel.ClockIntensitiesIn(IntensitiesRead))
                    {
                        CurrentIntensity = IntensitiesRead[0];
                        DefaultIntensity = IntensitiesRead[1];
                        ++ReadCount;
                    }
                    NewCurrentIntensity = -1;
                    NewDefaultIntensity = -1;
                }
                else
                {
                    Globals.sleep(1000);
                }
            }
        }
        catch (StopException ex)
        {
            // pass.
        }
    }
}
