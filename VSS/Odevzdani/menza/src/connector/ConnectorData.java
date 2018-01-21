package connector;

import cz.zcu.fav.kiv.jsim.JSimHead;
import operationunit.IServer;

/**
 * Created by liska on 20.01.2018.
 */
public class ConnectorData
{
    private Range probabilityRange;

    private IServer server;

    public ConnectorData(Range probabilityRange, IServer server)
    {
        this.probabilityRange = probabilityRange;
        this.server = server;
    }

    public IServer getServer()
    {
        return this.server;
    }

    public Range getProbabilityRange()
    {
        return this.probabilityRange;
    }
}

