package connector;

import operationunit.IServer;
import request.IRequest;

/**
 * Created by liska on 20.01.2018
 */
public interface IConnector 
{
    /***
     * Prida do seznamu moznych nasledniku novou instanci (instance a jeji pravdepodobnost)
     * @param toAdd
     * @return true - pokud pravdepodobnost pridavaneho prvku nedava dohromady 1 a prvek se podarilo pridat
     */
    boolean addNewPossibleOperUnit(ConnectorData toAdd);

    /***
     * Preda pozadavek ze vstupniho serveru na dalsi.
     * @param operUnitFrom vstupni server
     * @param request pozadavek k predani
     * @return true - pokud se vse podarilo
     */
    boolean redirectRequestToPossibleOperUnit(IServer operUnitFrom, IRequest request);
}
