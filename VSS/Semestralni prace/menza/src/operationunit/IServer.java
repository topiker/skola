package operationunit;

import connector.Connector;
import cz.zcu.fav.kiv.jsim.*;
import numberGenerator.ILambdaGenerator;
import request.IRequest;

/**
 * Created by liska on 20.01.2018.
 */
public abstract class IServer extends JSimProcess implements IServerMethods {

    private ILambdaGenerator lGenerator;
    protected final JSimHead queue;
    protected final Connector connector;

    public IServer(String s, JSimSimulation jSimSimulation, ILambdaGenerator lGenerator, JSimHead queue, Connector connector) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException {
        super(s, jSimSimulation);
        this.lGenerator = lGenerator;
        this.queue = queue;
        this.connector = connector;
    }

    @Override
    protected void life()
    {
        try
        {
            //Nekonecna smycka zarizeni
            while (true)
            {
                //Ziskame aktualni pozadavek ke zpracovani
                IRequest currentRequestToProcess = this.getRequestToProcess();
                //Pokud neni pozadavek ke zpracovani, usneme, jinak ho obslouzime
                if (currentRequestToProcess == null)
                {
                    passivate();
                }
                else
                {
                    //Posleme pozadavek do dalsi obsluhy
                    connector.redirectRequestToPossibleOperUnit(this,currentRequestToProcess);
                    //Simulace obsluhy
                    hold(lGenerator.generateNumber());
                }
            }
        }
        catch (JSimException e)
        {
            e.printStackTrace();
            e.printComment(System.err);
        }
    }

    @Override
    public IRequest getRequestToProcess() {
        return null;
    }

    public JSimHead getQueue()
    {
        return this.queue;
    }

}