package connector;


import cz.zcu.fav.kiv.jsim.JSimException;
import cz.zcu.fav.kiv.jsim.JSimHead;
import cz.zcu.fav.kiv.jsim.JSimSimulation;
import operationunit.IServer;
import request.IRequest;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by liska on 20.01.2018.
 */
public abstract class Connector implements IConnector{

    private List<ConnectorData> possibleNextQueues;

    private double totalProbability;

    private JSimSimulation simulation;

    public Connector(JSimSimulation sim)
    {
        this.possibleNextQueues = new ArrayList<>();
        this.totalProbability = 0;
        this.simulation = sim;
    }

    @Override
    public boolean addNewPossibleOperUnit(ConnectorData toAdd) {
        double probabilitySize = toAdd.getProbabilityRange().to - toAdd.getProbabilityRange().from;
        if((this.totalProbability + probabilitySize)<1)
        {
            this.totalProbability += probabilitySize;
            this.possibleNextQueues.add(toAdd);
            return true;
        }
        else
        {
            System.err.println("Total probability of this connection is more than 1!");
            return false;
        }
    }

    @Override
    public boolean redirectRequestToPossibleOperUnit(IServer operUnitFrom, IRequest request)
    {
        double randomProbability = 0;
        try
        {
            if(this.possibleNextQueues.size()>0)
            {
                randomProbability = Math.random();
                for (ConnectorData cData: this.possibleNextQueues)
                {
                    Range currentProbabilityRange = cData.getProbabilityRange();
                    if(currentProbabilityRange.from <= randomProbability && currentProbabilityRange.to > randomProbability)
                    {
                        //Touto frontou se vydam
                        request.out();
                        IServer server = cData.getServer();
                        JSimHead nextQueue = server.getQueue();
                        request.into(nextQueue);
                        if (server.isIdle())
                        {
                            server.activate(simulation.getCurrentTime());
                        }
                        return true;
                    }
                }
                return false;
            }
            else
            {
                return false;
                //Pozadavek je na konci cesty ...
                //Delej neco :D
            }
        }
        catch (JSimException e)
        {
            e.printStackTrace();
            e.printComment(System.err);
        }
        return false;
    }
}

