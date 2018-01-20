package network;

import connector.Connector;
import cz.zcu.fav.kiv.jsim.JSimHead;
import generator.IGenerator;

import java.util.ArrayList;
import java.util.List;

/**
 * Created by liska on 20.01.2018.
 */
public class Network
{
    private NETWORKSCHEMA schema;

    private List<IGenerator> generators;

    private List<JSimHead> queues;

    private List<Connector> connectors;

    public Network(NETWORKSCHEMA schema)
    {
        this.schema = schema;
        this.generators = new ArrayList<>();
        this.queues = new ArrayList<>();
        this.connectors = new ArrayList<>();
        this.prepareNetwork();

        System.out.println("--------------");
        System.out.println("Sit pripravena");
        System.out.println("--------------");

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

    }
}
