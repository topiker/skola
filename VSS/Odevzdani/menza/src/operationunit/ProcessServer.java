package operationunit;

import connector.Connector;
import cz.zcu.fav.kiv.jsim.*;
import numberGenerator.ILambdaGenerator;
import request.IRequest;
import request.Request;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by liska on 20.01.2018.
 * Tato trida reprezentuje server, ktery s pozadavkem chvili pracuje a pak ho preda dal.
 */
public class ProcessServer extends IServer
{

    public ProcessServer(String s, JSimSimulation jSimSimulation, ILambdaGenerator lGenerator, JSimHead queue, Connector connector) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException {
        super(s, jSimSimulation, lGenerator, queue,connector);
    }

    @Override
    public List<IRequest> getRequestsToProcess()
    {
        JSimLink first = this.queue.first();
        if(first != null)
        {
            List<IRequest> results = new ArrayList<>();
            results.add((IRequest)first.getData());
            return results;
        }
        else
        {
            return null;
        }
    }
}
