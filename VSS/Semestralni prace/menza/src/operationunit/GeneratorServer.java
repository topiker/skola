package operationunit;

import connector.Connector;
import cz.zcu.fav.kiv.jsim.*;
import numberGenerator.ILambdaGenerator;
import request.IRequest;
import request.Request;

/**
 * Created by liska on 20.01.2018.
 */
public class GeneratorServer extends IServer
{
    public GeneratorServer(String s, JSimSimulation jSimSimulation, ILambdaGenerator lGenerator, JSimHead queue, Connector connector) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException
    {
        super(s, jSimSimulation, lGenerator, queue,connector);
    }

    @Override
    public IRequest getRequestToProcess()
    {
        try
        {
            IRequest request = new Request(myParent.getCurrentTime());
            return request;
        }
        catch (JSimSimulationAlreadyTerminatedException e)
        {
            e.printStackTrace();
        } catch (JSimInvalidParametersException e)
        {
            e.printStackTrace();
        } catch (JSimTooManyProcessesException e)
        {
            e.printStackTrace();
        }

        return null;

    }
}
