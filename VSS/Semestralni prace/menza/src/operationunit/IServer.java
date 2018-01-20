package operationunit;

import cz.zcu.fav.kiv.jsim.*;
import numberGenerator.ILambdaGenerator;
import request.IRequest;

/**
 * Created by liska on 20.01.2018.
 */
public abstract class IServer extends JSimProcess implements IServerMethods {

    private ILambdaGenerator lGenerator;

    public IServer(String s, JSimSimulation jSimSimulation, ILambdaGenerator lGenerator) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException {
        super(s, jSimSimulation);
        this.lGenerator = lGenerator;
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
                    //Simulace obsluhy
                    hold(lGenerator.generateNumber());
                    //Posleme pozadavek do dalsi obsluhy

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

}