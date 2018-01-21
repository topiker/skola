package operationunit;

import connector.Connector;
import cz.zcu.fav.kiv.jsim.*;
import numberGenerator.ILambdaGenerator;
import request.IRequest;
import request.Request;

/**
 * Created by liska on 20.01.2018.
 */
public class ProcessServer extends IServer
{

    public ProcessServer(String s, JSimSimulation jSimSimulation, ILambdaGenerator lGenerator, JSimHead queue, Connector connector) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException {
        super(s, jSimSimulation, lGenerator, queue,connector);
    }

    @Override
    public IRequest getRequestToProcess()
    {
        JSimLink first = this.queue.first();
        if(first != null)
        {
            return (IRequest)first.getData();
        }
        else
        {
            return null;
        }
    }
}
