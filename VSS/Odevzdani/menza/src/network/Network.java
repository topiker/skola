package network;

import connector.Connector;
import connector.ConnectorData;
import connector.Range;
import cz.zcu.fav.kiv.jsim.*;
import numberGenerator.GeneratorType;
import numberGenerator.exp.ExpGenerator;
import numberGenerator.ILambdaGenerator;
import numberGenerator.gauss.GaussianGenerator;
import operationunit.*;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by liska on 20.01.2018.
 * Trida slouzi pro uchovani informaci o simulovanem systemu jako:
 *  pouzite servery
 *  pouzite generatory
 *  atd..
 *
 *  Zaroven obsahuje vygenerovani systemu simulujici menzu
 */
public class Network
{
    /**
     * Ciselnik schemat site (Menza nebo neco jineho)
     */
    private NetworkSchema schema;

    /***
     * Seznam generatoru pozadavku v siti
     */
    private List<IServer> generators;

    /***
     * Seznam serveru v siti
     */
    private List<IServer> servers;

    /***
     * Seznam spojeni mezi servery ci generatory
     */
    private List<Connector> connectors;

    /***
     * Koeficient variace pro Gaussovo rozdeleni
     */
    private double coeficintOfVariance;

    /***
     * Pocet pozadavku k vygenerovani
     */
    private int requestsToGenerate;

    /**
     * Ciselnik typu generovani nahodnych cisel (EXP | GAUSS)
     */
    private GeneratorType gType;

    /***
     *
     * @param schema Schema site, ktere se ma vygenerovat
     * @param type typ generatoru nahodnych cisel
     * @param coeficintOfVariance koeficient variace pro Gaussovo rozdeleni
     * @param requestsToGenerate pocet pozadavku, ktere maji projit system predtim, nez ho ukoncime
     */
    public Network(NetworkSchema schema, GeneratorType type, double coeficintOfVariance, int requestsToGenerate)
    {
        this.schema = schema;
        this.generators = new ArrayList<>();
        this.connectors = new ArrayList<>();
        this.servers = new ArrayList<>();
        this.coeficintOfVariance = coeficintOfVariance;
        this.requestsToGenerate = requestsToGenerate;
        this.gType = type;

        this.prepareNetwork();

    }

    /***
     * Pripravi sit podle ciselniku
     */
    private void prepareNetwork()
    {
        switch (this.schema)
        {
            case MENZA:
                System.out.println("Menza");
                this.prepareMenzaNetwork();
                break;
            default:
                break;
        }
    }


    /***
     * Vrati generator nahodnych cisel podle ciselniku predanym v konstruktoru. Generatoru nastavi stredni hodnotu (mean)
     * @param mean
     * @return
     */
    private ILambdaGenerator getGenerator(double mean)
    {
        ILambdaGenerator result = null;
        switch (this.gType)
        {
            case GAUSS:
                result = new GaussianGenerator(mean, coeficintOfVariance,0);
                break;
            case EXP:
                result = new ExpGenerator(mean);
                break;
            default:
                result = new ExpGenerator(mean);
                break;

        }
        return result;
    }

    /**
     * Vytvori sit reprezentujici svet v menze. Tato sit byla resena v ramci semestralni prace.
     */
    private void prepareMenzaNetwork()
    {

        MySimulation simSimulation = null;
        double probabilityOfReturn = 0.04;

        double cashier1 = 0.55;
        double cashier2 = 0.45;

        double foodProbability1 = 0.15;
        double foodProbability2 = 0.25;
        double foodProbability3 = 0.18;
        double foodProbability4 = 0.19;
        double foodProbability5 = 0.23;

        try
        {
            simSimulation = new MySimulation("Menza");
            ILambdaGenerator gsEnd = getGenerator(1500);
            ILambdaGenerator gg1 = getGenerator(12);
            ILambdaGenerator gs1 = getGenerator(20);
            ILambdaGenerator gs2 = getGenerator(12);
            ILambdaGenerator gs3 = getGenerator(11);
            ILambdaGenerator gs4 = getGenerator(13);
            ILambdaGenerator gs5 = getGenerator(12);
            ILambdaGenerator gs6 = getGenerator(14);
            ILambdaGenerator gs7 = getGenerator(11);
            ILambdaGenerator gs8 = getGenerator(12);
            ILambdaGenerator gs9 = getGenerator(18);


            IServer sEnd = new EndingServer("Koncovy server",simSimulation,gsEnd, new JSimHead("Fronta koncovy",simSimulation),null,requestsToGenerate);
            Connector fromS9 = new Connector(simSimulation);
            fromS9.addNewPossibleOperUnit(new ConnectorData(new Range(0,1 - probabilityOfReturn),sEnd));
            IServer s9 = new ProcessServer("Server 9",simSimulation,gs9, new JSimHead("Fronta 9",simSimulation),fromS9);

            //Pokladny, prechazi k odevzdani tacu, nebo na zacatek
            Connector fromS7 = new Connector(simSimulation);
            Connector fromS8 = new Connector(simSimulation);
            fromS7.addNewPossibleOperUnit(new ConnectorData(new Range(0,1 - probabilityOfReturn),s9));
            fromS8.addNewPossibleOperUnit(new ConnectorData(new Range(0,1 - probabilityOfReturn),s9));
            IServer s7 = new ProcessServer("Server 7",simSimulation,gs7, new JSimHead("Fronta 7",simSimulation),fromS7);
            IServer s8 = new ProcessServer("Server 8",simSimulation,gs8, new JSimHead("Fronta 8",simSimulation),fromS8);

            //Vyber jidel, prechazi na pokladny
            Connector fromS2 = new Connector(simSimulation);
            fromS2.addNewPossibleOperUnit(new ConnectorData(new Range(0,cashier1),s7));
            fromS2.addNewPossibleOperUnit(new ConnectorData(new Range(cashier1,cashier1+cashier2),s8));
            Connector fromS3 = new Connector(simSimulation);
            fromS3.addNewPossibleOperUnit(new ConnectorData(new Range(0,cashier1),s7));
            fromS3.addNewPossibleOperUnit(new ConnectorData(new Range(cashier1,cashier1+cashier2),s8));
            Connector fromS4 = new Connector(simSimulation);
            fromS4.addNewPossibleOperUnit(new ConnectorData(new Range(0,cashier1),s7));
            fromS4.addNewPossibleOperUnit(new ConnectorData(new Range(cashier1,cashier1+cashier2),s8));
            Connector fromS5 = new Connector(simSimulation);
            fromS5.addNewPossibleOperUnit(new ConnectorData(new Range(0,cashier1),s7));
            fromS5.addNewPossibleOperUnit(new ConnectorData(new Range(cashier1,cashier1+cashier2),s8));
            Connector fromS6 = new Connector(simSimulation);
            fromS6.addNewPossibleOperUnit(new ConnectorData(new Range(0,cashier1),s7));
            fromS6.addNewPossibleOperUnit(new ConnectorData(new Range(cashier1,cashier1+cashier2),s8));
            IServer s2 = new ProcessServer("Server 2",simSimulation,gs2, new JSimHead("Fronta 2",simSimulation),fromS2);
            IServer s3 = new ProcessServer("Server 3",simSimulation,gs3, new JSimHead("Fronta 3",simSimulation),fromS3);
            IServer s4 = new ProcessServer("Server 4",simSimulation,gs4, new JSimHead("Fronta 4",simSimulation),fromS4);
            IServer s5 = new ProcessServer("Server 5",simSimulation,gs5, new JSimHead("Fronta 5",simSimulation),fromS5);
            IServer s6 = new ProcessServer("Server 6",simSimulation,gs6, new JSimHead("Fronta 6",simSimulation),fromS6);

            //Stravnik prichazi, nebo se vraci z konce
            Connector fromS1 = new Connector(simSimulation);
            double currentEndingProbability = 0;
            fromS1.addNewPossibleOperUnit(new ConnectorData(new Range(0,foodProbability1),s2));
            currentEndingProbability += foodProbability1;
            fromS1.addNewPossibleOperUnit(new ConnectorData(new Range(currentEndingProbability,currentEndingProbability+foodProbability2),s3));
            currentEndingProbability += foodProbability2;
            fromS1.addNewPossibleOperUnit(new ConnectorData(new Range(currentEndingProbability,currentEndingProbability+foodProbability3),s4));
            currentEndingProbability += foodProbability3;
            fromS1.addNewPossibleOperUnit(new ConnectorData(new Range(currentEndingProbability,currentEndingProbability+foodProbability4),s5));
            currentEndingProbability += foodProbability4;
            fromS1.addNewPossibleOperUnit(new ConnectorData(new Range(currentEndingProbability,currentEndingProbability+foodProbability5),s6));
            IServer s1 = new ProcessServer("Server 1",simSimulation,gs1, new JSimHead("Fronta 1",simSimulation),fromS1);
            fromS7.addNewPossibleOperUnit(new ConnectorData(new Range(1 - probabilityOfReturn,1),s1));
            fromS8.addNewPossibleOperUnit(new ConnectorData(new Range(1 - probabilityOfReturn,1),s1));
            fromS9.addNewPossibleOperUnit(new ConnectorData(new Range(1 - probabilityOfReturn,1),s1));


            //Stravnik se generuje
            Connector fromG1 = new Connector(simSimulation);
            fromG1.addNewPossibleOperUnit(new ConnectorData(new Range(0,1),s1));
            IServer g1 = new GeneratorServer("Generator vlny 1",simSimulation,gg1,null,fromG1);

            servers.add(s1);
            servers.add(s2);
            servers.add(s3);
            servers.add(s4);
            servers.add(s5);
            servers.add(s6);
            servers.add(s7);
            servers.add(s8);
            servers.add(s9);

            generators.add(g1);


            for (IServer generatorToRun: generators)
            {
                generatorToRun.activate(0.0);
            }

            simSimulation.message("Running the simulation, please wait.");
            int stepCounter = 0;
            while ((simSimulation.step() == true))
            {
                stepCounter++;
            }

            double LqTotal = 0;
            double TqTotal = simSimulation.getTotalTimeOfProcessesInSystem() / simSimulation.getTotalExitedProcesses();

            for (IServer currServer: servers) {
                simSimulation.message(currServer.getStatsAsString());
                LqTotal += currServer.getLq();

            }
            simSimulation.message("Cela sit:");
            simSimulation.message("----------------------------------");
            simSimulation.message("Lq = " + LqTotal);
            simSimulation.message("Tq = " + TqTotal);

        }
        catch (JSimException e) {
            e.printStackTrace();
            e.printComment(System.err);
        }
        // We must shutdown the simulation.
        finally
        {
            simSimulation.shutdown();
        }
    }
}
