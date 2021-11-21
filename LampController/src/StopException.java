// vim: shiftwidth=4
// vim: ts=4
package com.errapartengineering.LampController;

/** Signals to stop the MIDP applet. */
public final class StopException extends Throwable {
	public StopException(String	reason)
	{
		super(reason);
	}

	public StopException()
	{
		super();
	}
} // class StopException

