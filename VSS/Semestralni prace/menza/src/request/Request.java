package request;

import cz.zcu.fav.kiv.jsim.JSimInvalidParametersException;
import cz.zcu.fav.kiv.jsim.JSimSimulationAlreadyTerminatedException;
import cz.zcu.fav.kiv.jsim.JSimTooManyProcessesException;

/**
 * Created by liska on 20.01.2018.
 */
public class Request extends IRequest
{
    public Request(double time, boolean runImmediately) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException {
        super(time,runImmediately);
    }
}
