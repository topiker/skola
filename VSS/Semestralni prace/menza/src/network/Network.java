package network;

import connector.Connector;
import connector.ConnectorData;
import connector.Range;
import cz.zcu.fav.kiv.jsim.*;
import numberGenerator.ExpGenerator;
import numberGenerator.ILambdaGenerator;
import operationunit.GeneratorServer;
import operationunit.IServer;
import operationunit.ProcessServer;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by liska on 20.01.2018.
 */
public class Network
{
    private NetworkSchema schema;

    private List<IServer> generators;

    private List<IServer> servers;

    private List<Connector> connectors;

    public Network(NetworkSchema schema)
    {
        this.schema = schema;
        this.generators = new ArrayList<>();
        this.connectors = new ArrayList<>();
        this.servers = new ArrayList<>();
        this.prepareNetwork();

    }

    private void prepareNetwork()
    {
        switch (this.schema)
        {
            case MENZA:
                System.out.println("Menza");
                this.prepareMenzaNetwork();
                break;
            default:
        }
    }

    private void prepareMenzaNetwork()
    {

        JSimSimulation simSimulation = null;
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
            simSimulation = new JSimSimulation("Queueing Networks Simulation");
            ILambdaGenerator generator = new ExpGenerator(1500);

            IServer sEnd = new ProcessServer("Koncovy server",simSimulation,generator, new JSimHead("Fronta koncovy",simSimulation),null);
            Connector fromS9 = new Connector(simSimulation);
            fromS9.addNewPossibleOperUnit(new ConnectorData(new Range(0,1 - probabilityOfReturn),sEnd));
            IServer s9 = new ProcessServer("Server 9",simSimulation,generator, new JSimHead("Fronta 9",simSimulation),fromS9);

            //Pokladny, prechazi k odevzdani tacu, nebo na zacatek
            Connector fromS7 = new Connector(simSimulation);
            Connector fromS8 = new Connector(simSimulation);
            fromS7.addNewPossibleOperUnit(new ConnectorData(new Range(0,1 - probabilityOfReturn),s9));
            fromS8.addNewPossibleOperUnit(new ConnectorData(new Range(0,1 - probabilityOfReturn),s9));
            IServer s7 = new ProcessServer("Server 7",simSimulation,generator, new JSimHead("Fronta 7",simSimulation),fromS7);
            IServer s8 = new ProcessServer("Server 8",simSimulation,generator, new JSimHead("Fronta 8",simSimulation),fromS8);

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
            IServer s2 = new ProcessServer("Server 2",simSimulation,generator, new JSimHead("Fronta 2",simSimulation),fromS2);
            IServer s3 = new ProcessServer("Server 3",simSimulation,generator, new JSimHead("Fronta 3",simSimulation),fromS3);
            IServer s4 = new ProcessServer("Server 4",simSimulation,generator, new JSimHead("Fronta 4",simSimulation),fromS4);
            IServer s5 = new ProcessServer("Server 5",simSimulation,generator, new JSimHead("Fronta 5",simSimulation),fromS5);
            IServer s6 = new ProcessServer("Server 6",simSimulation,generator, new JSimHead("Fronta 6",simSimulation),fromS6);

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
            IServer s1 = new ProcessServer("Server 1",simSimulation,generator, new JSimHead("Fronta 1",simSimulation),fromS1);
            fromS7.addNewPossibleOperUnit(new ConnectorData(new Range(1 - probabilityOfReturn,1),s1));
            fromS8.addNewPossibleOperUnit(new ConnectorData(new Range(1 - probabilityOfReturn,1),s1));
            fromS9.addNewPossibleOperUnit(new ConnectorData(new Range(1 - probabilityOfReturn,1),s1));


            //Stravnik se generuje
            Connector fromG1 = new Connector(simSimulation);
            fromG1.addNewPossibleOperUnit(new ConnectorData(new Range(0,1),s1));
            IServer g1 = new GeneratorServer("Generator 1",simSimulation,generator,null,fromG1);

            servers.add(s1);
            servers.add(s2);
            servers.add(s3);
            servers.add(s4);
            servers.add(s5);
            servers.add(s6);
            servers.add(s7);
            servers.add(s8);
            servers.add(s9);
            servers.add(sEnd);

            generators.add(g1);

//            Connector fromS2 = new Connector(simSimulation);
//            Connector fromS3 = new Connector(simSimulation);
//            fromS2.addNewPossibleOperUnit(new ConnectorData(new Range(0,1),s4));
//            fromS3.addNewPossibleOperUnit(new ConnectorData(new Range(0,1),s4));
//            IServer s2 = new ProcessServer("Server 2",simSimulation,generator, new JSimHead("Fronta 2",simSimulation),fromS3);
//            IServer s3 = new ProcessServer("Server 3",simSimulation,generator, new JSimHead("Fronta 3",simSimulation),fromS3);
//
//            Connector fromS1 = new Connector(simSimulation);
//            fromS1.addNewPossibleOperUnit(new ConnectorData(new Range(0,0.5),s2));
//            fromS1.addNewPossibleOperUnit(new ConnectorData(new Range(0.5,1),s3));
//            IServer s1 = new ProcessServer("Server 1",simSimulation,generator, new JSimHead("Fronta 1",simSimulation),fromS1);
//
//            Connector fromG1 = new Connector(simSimulation);
//            fromG1.addNewPossibleOperUnit(new ConnectorData(new Range(0,1),s1));
//            IServer g1 = new GeneratorServer("Generator 1",simSimulation,generator, new JSimHead("Fronta 1",simSimulation),fromG1);
//
//            servers.add(s1);
//            servers.add(s2);
//            servers.add(s3);
//            servers.add(s4);
//
//            generators.add(g1);
//
//            connectors.add(fromG1);
//            connectors.add(fromS1);
//            connectors.add(fromS2);
//            connectors.add(fromS3);

            for (IServer generatorToRun: generators
                 )
            {
                generatorToRun.activate(0.0);
            }

//            ILambdaGenerator generator = new ExpGenerator(15);
//            Connector fromG1 = new Connector(simSimulation);
//            IServer server1 = new ProcessServer("Server 1",simSimulation,generator,new JSimHead("Fronta server 1",simSimulation),null);
//            fromG1.addNewPossibleOperUnit(new ConnectorData(new Range(0,1),server1));
//
//            this.servers.add(server1);
//            this.generators.add(new GeneratorServer("Generator prichozich",simSimulation,generator,new JSimHead("Fronta prichozich",simSimulation),fromG1));
//
//            simSimulation.message("Activating the generators...");
//
//            for (IServer generatorToRun: generators
//                 )
//            {
//                generatorToRun.activate(0.0);
//            }

            simSimulation.message("Running the simulation, please wait.");
            while ((simSimulation.getCurrentTime() < 1000000) && (simSimulation.step() == true))
                ;

            //Todo: vypisy
            for (IServer currServer: servers
                 ) {
                simSimulation.message(currServer.getName() +  "Lw = " + currServer.getQueue().getLw() + ", Tw = " + currServer.getQueue().getTw() + ", Tw all = " + currServer.getQueue().getTwForAllLinks());
            }
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
