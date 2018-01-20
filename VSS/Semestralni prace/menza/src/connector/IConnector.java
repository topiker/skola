package connector;

import operationunit.IServer;
import request.IRequest;

/**
 * Created by liska on 20.01.2018.
 */
public interface IConnector 
{
    
    boolean addNewPossibleOperUnit(ConnectorData toAdd);
    boolean redirectRequestToPossibleOperUnit(IServer operUnitFrom, IRequest request);
}
