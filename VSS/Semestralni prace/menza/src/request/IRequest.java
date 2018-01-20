package request;

import cz.zcu.fav.kiv.jsim.*;

/**
 * Created by liska on 20.01.2018.
 */
public abstract class IRequest extends JSimLink
{
    private double timeOfCreation;

    public IRequest(double time) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException
    {
        this.timeOfCreation = time;
    } // constructor

    public double getCreationTime()
    {
        return timeOfCreation;
    } // getCreationTime
}
