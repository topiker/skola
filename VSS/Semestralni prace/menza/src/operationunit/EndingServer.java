package operationunit;

import connector.Connector;
import cz.zcu.fav.kiv.jsim.*;
import numberGenerator.ILambdaGenerator;
import request.IRequest;

import java.util.List;

/**
 * Created by liska on 21.01.2018.
 * Tato trida reprezentuje server, ktery je koncovym zarizenim. Do nej prijdou pozadavky, nez nadobro zmizi ze systmu.
 * Zde je tedy vhodna zhotovit nejak statistiky.
 * Zaroven hlida, zda nebyl prekrocen maximalni pocet prijatych pozadavku -> pak system ukonci
 */
public class EndingServer extends ProcessServer
{

    /**
     * Maximalni pocet prijatych pozadavku pred ukoncenim systemu
     */
    private int maxProcessedRequests;

    public EndingServer(String s, JSimSimulation jSimSimulation, ILambdaGenerator lGenerator, JSimHead queue, Connector connector, int maxProcessedRequests) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException {
        super(s, jSimSimulation, lGenerator, queue, connector);
        this.maxProcessedRequests = maxProcessedRequests;
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
                List<IRequest> currentRequestsToProcess = this.getRequestsToProcess();
                //Pokud neni pozadavek ke zpracovani, usneme, jinak ho obslouzime
                if (currentRequestsToProcess == null || currentRequestsToProcess.size() == 0)
                {
                    passivate();
                }
                else
                {
                    for(int i = 0; i < currentRequestsToProcess.size();i++)
                    {
                        IRequest currentRequestToProcess = currentRequestsToProcess.get(i);
                        //Pokud koncovy server prekonal pocet prijatych pozadavku, simulaci ukoncim
                        if (this.maxProcessedRequests < processedRequestsCounter + 1) {
                            System.out.println("Server '" + this.getName() + "' zpracoval " + this.processedRequestsCounter + " pozadavku z " + this.maxProcessedRequests + ". System bude ukoncen.");
                            this.myParent.shutdown();
                        }
                        processedRequestsCounter += 1;

                        //Zaevidujeme statistiky pred koncem pozadavku v systemu
                        ((MySimulation)myParent).incrementTotalExitedProcesses();
                        ((MySimulation)myParent).addTotalTimeOfProcessesInSystem(myParent.getCurrentTime() - currentRequestToProcess.getCreationTime());
                        //Vyjmeme pozadavek ze systemu
                        currentRequestToProcess.out();
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
}
