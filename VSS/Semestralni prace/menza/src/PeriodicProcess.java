/*
 *	Copyright (c) 2000-2006 Jaroslav Kačer <jaroslav@kacer.biz>
 *	Licensed under the Academic Free License version 2.1
 *	J-Sim source code can be downloaded from http://www.j-sim.zcu.cz/
 *
 */

import cz.zcu.fav.kiv.jsim.JSimException;
import cz.zcu.fav.kiv.jsim.JSimInvalidParametersException;
import cz.zcu.fav.kiv.jsim.JSimProcess;
import cz.zcu.fav.kiv.jsim.JSimSimulation;
import cz.zcu.fav.kiv.jsim.JSimSimulationAlreadyTerminatedException;
import cz.zcu.fav.kiv.jsim.JSimTooManyProcessesException;

/**
 * A process that periodicly prints out a character.
 */
public class PeriodicProcess extends JSimProcess
{
	private char charToPrint;
	private double timeToWait;

	// It is better to throw the exceptions from the constructor than to catch them.
	public PeriodicProcess(JSimSimulation simulation, char c, String name, double timeW) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException
	{
		super(name, simulation);
		charToPrint = c;
		timeToWait = timeW;
	} // constructor

	// We don't care about stopping -- we run infinitly.
	protected void life()
	{
		try
		{
			double time;

			// Yes, it's really a neverending cycle.
			while (true)
			{
				time = myParent.getCurrentTime();
				message(getName() + " at " + time + " : " + charToPrint);

				// Planning of the next printing
				hold(timeToWait);
			} // while
		} // try
		catch (JSimException e)
		{
			e.printStackTrace();
			e.printComment();
		} // catch
	} // life

} // class PeriodicProcess
