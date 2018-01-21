package operationunit;

import connector.Connector;
import cz.zcu.fav.kiv.jsim.*;
import numberGenerator.ILambdaGenerator;
import request.IRequest;
import request.Request;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by liska on 21.01.2018.
 * Tato trida generuje pozadavky a preposila je dal. Navic po nejakem intervalu vytvori vlnu pozadavku (napr. 10 najednou).
 * Tim lze otestovat narazova zatez systemu
 */
public class WaveGenerator extends IServer
{

    /**
     * Kolik pozadavku se ma naraz vygenerovat
     */
    private int waveSize;
    /**
     * Intverval pozadavku mezi vlnami
     */
    private final int waveInterval = 100;

    public WaveGenerator(String s, JSimSimulation jSimSimulation, ILambdaGenerator lGenerator, JSimHead queue, Connector connector, int waveSize) throws JSimSimulationAlreadyTerminatedException, JSimInvalidParametersException, JSimTooManyProcessesException {
        super(s, jSimSimulation, lGenerator, queue, connector);
        this.waveSize = waveSize;
    }

    @Override
    public List<IRequest> getRequestsToProcess()
    {
        try
        {
            List<IRequest> result = new ArrayList<>();
            if((processedRequestsCounter+1)%waveInterval==0)
            {
                for(int i = 0; i < waveSize; i++)
                {
                    IRequest request = new Request(myParent.getCurrentTime(),true);
                    result.add(request);
                }
            }
            else
            {
                IRequest request = new Request(myParent.getCurrentTime(),false);
                result.add(request);
            }
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
