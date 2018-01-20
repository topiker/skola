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
 * A process with no periodicity, just prints a message, sleeps for a while, prints another message and dies. 
 */
public class OnePassProcess extends JSimProcess
{
	private double timeToSleep;

	// It is better to throw the exceptions from the constructor than to catch them.
	public OnePassProcess(JSimSimulation simulation, String name, double timeS) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException
	{
		super(name, simulation);
		timeToSleep = timeS;
	} // constructor

	// During the life, we just print a message, then we sleep for a while and print another message just before dying.
	protected void life()
	{
		try
		{
			message(getName() + ": Going to sleep for " + timeToSleep + " seconds.");
			hold(timeToSleep);
			message(getName() + ": Awaken, going to die just now.");
		} // try
		catch (JSimException e)
		{
			e.printStackTrace();
			e.printComment();
		} // catch
	} // life

} // class OnePassProcess
