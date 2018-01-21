package operationunit;

import connector.Connector;
import cz.zcu.fav.kiv.jsim.*;
import numberGenerator.ILambdaGenerator;
import request.IRequest;

import java.util.List;

/**
 * Created by liska on 20.01.2018.
 * Spolecna trida pro vsechny servery ( + generatory)
 */
public abstract class IServer extends JSimProcess {

    /**
     * Generator delky provadeni pozadavku
     */
    protected ILambdaGenerator lGenerator;
    /**
     * Fronta pred serverem
     */
    protected final JSimHead queue;
    /**
     * Trida reprezentujici dalsi mozne prechody z tohoto serveru
     */
    protected final Connector connector;
    /**
     * Kolik pozadavku proslo timto serverem
     */
    protected int processedRequestsCounter = 0;
    /**
     * Jak dlouhou system pracoval
     */
    public double totalRunningTime = 0;

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
                            //Simulace obsluhy
                            double time = lGenerator.generateHoldTime();
                            totalRunningTime +=time;
                            hold(time);
                            processedRequestsCounter+=1;
                            //Posleme pozadavek do dalsi obsluhy
                            connector.redirectRequestToPossibleOperUnit(this,currentRequestToProcess);
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

    public List<IRequest> getRequestsToProcess()
    {
        return null;
    }

    public JSimHead getQueue()
    {
        return this.queue;
    }

    public int getProcessedRequestsCounter()
    {
        return this.getProcessedRequestsCounter();
    }

    /**
     * Vypocita stredni frekvencz toku v uzlech = pocet zpracovanych pozadavku / delka casu simulace
     * Tedy vime, kolik pozadavku se zpracuje za jednotku casu prumerne.
     * @return
     */
    public double getMeanFrequency()
    {
        return this.processedRequestsCounter / (myParent.getCurrentTime());
    }

    /**
     * Zatez serveru
     * Jak dlouho server bezel vuci celkove dobe behu simulace
     * @return
     */
    public double getLoad()
    {
        return (this.totalRunningTime/myParent.getCurrentTime());
    }

    /**
     * Tq
     * @return
     */
    public double getTq()
    {
        return this.queue.getTw();
    }

    /**
     * Lq
     * @return
     */
    public double getLq()
    {
        return this.queue.getLw();
    }

    /**
     * Vypise statistiky serveru
     * @return retezec reprezentujici statistiky
     */
    public String getStatsAsString()
    {
        StringBuilder result = new StringBuilder();
        result.append(System.getProperty("line.separator"));
        result.append(this.getName());
        result.append(System.getProperty("line.separator"));
        result.append("----------------------------------");
        result.append(System.getProperty("line.separator"));
        result.append("Flow = " + this.getMeanFrequency());
        result.append(System.getProperty("line.separator"));
        result.append("Load = " + this.getLoad());
        result.append(System.getProperty("line.separator"));
        result.append("Lq = " + this.getLq());
        result.append(System.getProperty("line.separator"));
        result.append("Tq = " + this.getTq());
        result.append(System.getProperty("line.separator"));
        result.append("Lw = " + this.queue.getLw());
        result.append(System.getProperty("line.separator"));
        result.append("Tw = " + this.queue.getTw());
        result.append(System.getProperty("line.separator"));
        result.append(System.getProperty("line.separator"));

        return  result.toString();


    }

}