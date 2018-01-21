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
 * Tato trida slouzi jako generator pozadavku. V casovem intervalu generuje pozadavky a predava je do front.
 */
public class GeneratorServer extends IServer
{
    public GeneratorServer(String s, JSimSimulation jSimSimulation, ILambdaGenerator lGenerator, JSimHead queue, Connector connector) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException
    {
        super(s, jSimSimulation, lGenerator, queue,connector);
    }

    @Override
    protected void life() {
        try
        {
            //Nekonecna smycka zarizeni
            while (true)
            {
                //Ziskame aktualni pozadavek ke zpracovani
                List<IRequest> currentRequestsToProcess = this.getRequestsToProcess();
                //Pokud neni pozadavek ke zpracovani, usneme, jinak ho obslouzime
                if (currentRequestsToProcess == null || currentRequestsToProcess.size()==0)
                {
                    passivate();
                }
                else
                {
                    for (IRequest currentRequestToProcess: currentRequestsToProcess)
                    {
                        //Jedna se o pozadavek, ktery se ma spustit ihned, napriklad generujeme vlnu pozadavku
                        if(currentRequestToProcess.getRunImmediately()==true)
                        {
                            currentRequestToProcess.setRunImmediately(false);
                            processedRequestsCounter+=1;
                            connector.redirectRequestToPossibleOperUnit(this,currentRequestToProcess);
                        }
                        else
                        {
                            processedRequestsCounter+=1;
                            //Posleme pozadavek do dalsi obsluhy
                            connector.redirectRequestToPossibleOperUnit(this,currentRequestToProcess);
                            //Simulace obsluhy
                            double time = lGenerator.generateHoldTime();
                            totalRunningTime +=time;
                            hold(time);
                        }
                    }

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
    public List<IRequest> getRequestsToProcess()
    {
        try
        {
            List<IRequest> result = new ArrayList<>();
            IRequest request = new Request(myParent.getCurrentTime(),false);
            result.add(request);
            return result;
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
