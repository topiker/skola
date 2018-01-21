import cz.zcu.fav.kiv.jsim.*;
import network.Network;
import network.NetworkSchema;
import numberGenerator.GeneratorType;

/**
 * Created by liska on 20.01.2018.
 */
public class Main
{
    public static int requestedRequests = 10000;
    public static void main(String[] args)
    {
        try
        {
            if(args.length == 2)
            {
                GeneratorType gType = null;
                if(args[1].toLowerCase().trim().compareTo("exp")==0)
                {
                    gType = GeneratorType.EXP;
                }
                else if(args[1].toLowerCase().trim().compareTo("gauss")==0)
                {
                    gType = GeneratorType.GAUSS;
                }
                else
                {
                    System.out.println("Nebyl rozpoznan typ generatoru, program bude ukoncen.");
                    System.exit(-1);
                }

                requestedRequests = Integer.parseInt(args[0].trim());

                if(gType==GeneratorType.GAUSS)
                {
                    //Spustime simulaci 3 krat s ruznymi koeficienty variace
                    Network network1 = new Network(NetworkSchema.MENZA, GeneratorType.EXP,0.05,requestedRequests);
                    Network network2 = new Network(NetworkSchema.MENZA, GeneratorType.EXP,0.2,requestedRequests);
                    Network network3 = new Network(NetworkSchema.MENZA, GeneratorType.EXP,0.7 ,requestedRequests);


                }
                else
                {
                    Network network = new Network(NetworkSchema.MENZA, GeneratorType.EXP,0,requestedRequests);
                }

            }
            else
            {
                System.out.println("Ocekavan vstup: [pocet pozadavku] [typ generatoru - EXP nebo GAUSS]");
                System.out.println("Bude spustena simulace s generatorem EXP a pak GAUSS (0.05, 0.2, 0.7)");
                Network network1 = new Network(NetworkSchema.MENZA, GeneratorType.EXP,0,requestedRequests);

                Network network2 = new Network(NetworkSchema.MENZA, GeneratorType.GAUSS,0.05,requestedRequests);
                System.out.println("Koeficinet variace:" + 0.05);
                Network network3 = new Network(NetworkSchema.MENZA, GeneratorType.GAUSS,0.2,requestedRequests);
                System.out.println("Koeficinet variace:" + 0.2);
                Network network4 = new Network(NetworkSchema.MENZA, GeneratorType.GAUSS,0.7,requestedRequests);
                System.out.println("Koeficinet variace:" + 0.7);

            }
        }
        catch(Exception e)
        {
            System.out.println("Nastala chyba pri zpracovani vstupu, program bude ukoncen");
            System.exit(-1);
        }

        //

    } // main



} // class HelloWorld1

