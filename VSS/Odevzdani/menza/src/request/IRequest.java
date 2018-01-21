package request;

import cz.zcu.fav.kiv.jsim.*;

/**
 * Created by liska on 20.01.2018.
 * Trida reprezentuje pozadavek v systemu
 */
public abstract class IRequest extends JSimLink
{
    /***
     * Simulacni cas, kdy byl pozadavek vytvoren
     */
    private double timeOfCreation;

    /**
     * Zda se ma pozadavek predat ihned, nebo se ma zdrzovat ve fronte.
     */
    private boolean runImmediately;

    public IRequest(double time, boolean runImmediately) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException
    {
        this.timeOfCreation = time;
        this.runImmediately = runImmediately;
    } // constructor

    public double getCreationTime()
    {
        return timeOfCreation;
    } // getCreationTime

    public boolean getRunImmediately()
    {
        return this.runImmediately;
    }

    public void setRunImmediately(boolean flag)
    {
        this.runImmediately = flag;
    }

    @Override
    public Object getData() {
        return this;
    }
}
