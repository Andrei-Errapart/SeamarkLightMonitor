// vim: shiftwidth=4
// vim: ts=4
package com.errapartengineering.LampController;

/**
 * Summary description for Globals.
 */
public abstract class Globals
{
    private static boolean StopFlag = false;

    /** Sleep for a given amount of milliseconds.
    This sleep honors StopFlag.
     */
    public final static void sleep(int ms) throws StopException
    {
        int so_far = 0;
        if (StopFlag)
        {
            throw new StopException("Sleep Stop");
        }
        while (so_far < ms)
        {
            int this_round = so_far + 1000 < ms ? 1000 : ms - so_far;
            so_far += this_round;
            try
            {
                Thread.sleep(this_round);
            }
            catch (InterruptedException ignore)
            {
            }
            if (StopFlag)
            {
                throw new StopException("Sleep Stop");
            }
        }
    }
    public final static void StopFlagClear()
    {
        StopFlag = false;
    }
    public final static void StopFlagSet()
    {
        StopFlag = true;
    }
    public final static boolean StopFlagGet()
    {
        return StopFlag;
    }
}
