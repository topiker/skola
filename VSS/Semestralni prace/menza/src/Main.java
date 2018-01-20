import cz.zcu.fav.kiv.jsim.*;

/**
 * Created by liska on 20.01.2018.
 */
public class Main
{
    public static void main(String[] args)
    {
        PeriodicProcess a, b;
        @SuppressWarnings("unused") // Because of process "e" -- not activated just for demonstration purposes.
                OnePassProcess c, d, e;
        JSimSimulation simulation = null;

        try
        {
            System.out.println("Main program: Starting the `Simple Switching' example.");

            // First, we must create a simulation.
            System.out.print("Main program: Creating a new simulation ... ");
            simulation = new JSimSimulation("Simple Switching Example");
            System.out.println("OK");

            // Then, we create some processes in the simulation and activate them.
            simulation.messageNoNL("Main program: Creating periodic processes ... ");
            a = new PeriodicProcess(simulation, 'A', "Process A", 0.91);
            b = new PeriodicProcess(simulation, 'B', "Process B", 1.13);
            simulation.message("OK");

            simulation.messageNoNL("Main program: Creating one-pass processes ... ");
            c = new OnePassProcess(simulation, "Process C", 3.5); // It will die itself
            d = new OnePassProcess(simulation, "Process D", 25.0); // J-Sim will kill it
            e = new OnePassProcess(simulation, "Process E", 1.0); // This process will never be activated
            simulation.message("OK");

            simulation.messageNoNL("Main program: Activating first four processes, the fifth one is forgotten ... ");
            a.activate(0.02);
            b.activate(0.03);
            c.activate(0.04);
            d.activate(0.05);
            // e is not activated
            simulation.message("OK");

			/*
			 *  And now, we can run the simulation, step by step.
			 *  It is very useful to have a time limit (or another limitation, e.g. a counter of steps executed)
			 *  in the condition because simulation processes usually never finish.
			 *  However, if (by some reason) there are no processes left or there are no processes
			 *  scheduled, there is nothing to do for the simulation. In this case, step() returns false
			 *  and it is completely useless to call the step() function again.
			 */
            simulation.message("\nNow we run the simulation step-by-step until the simulation time");
            simulation.message("is over 10 seconds or there are no scheduled processes.");
            simulation.message("(This will never happen here.)");

            while ((simulation.getCurrentTime() < 10.0) && (simulation.step() == true))
                ;
        } // try

        // You can catch all possible exceptions in just one handler, there's no need to enumerate them all, if you don't want to react in a specific way.
        catch (JSimException exc)
        {
            exc.printStackTrace();
            exc.printComment(System.err);
        }

        // It is a good practice to put the shutdown in the finally block. Then you can be sure that the JVM will not "hang".
        finally
        {
            simulation.shutdown();
        }
    } // main

} // class HelloWorld1

/**
 * A simple process example.
 */
class HelloWorld1Process extends JSimProcess
{
    /**
     * Note that you must ALWAYS write your own constructor &ndash; JSimProcess' one has two parameters.
     */
    public HelloWorld1Process(String name, JSimSimulation sim) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException
    {
        super(name, sim);
    } // constructor

    /**
     * This is the life of the process. You can do whatever you want here.
     */
    protected void life()
    {
        try
        {
            message("Hello! I have just been born!");

            while (true)
            {
                message("Hello! It is " + myParent.getCurrentTime() + " o'clock now and I'm going to sleep for a while.");
                // Now we let other processes run. That's a pity we are alone here!
                hold(9.8765);
            } // for i

            // We will never get here.
            // message("You can never see this message.");
        } // try
        catch (JSimException e)
        {
            e.printStackTrace();
            e.printComment();
        } // catch
    } // life

} // class HelloWorld1Process