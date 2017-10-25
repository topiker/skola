package com.company;

/**
 * Created by liska on 22.10.2017.
 * Trida zajistuje zparsovani vstupu od uzivatele
 */
public class InputParser {

    /**
     * Bezparametricky konsktrukor
     */
    public InputParser()
    {
    }

    /**
     * Ze vstupniho pole rozparsuje retezce na parametry gaussovskeho modelu
     * @param parameters 0 - pocet cisel ke generovani, 1 - stredni hodnota, 2 - odchylka
     *
     * @return
     */
    public GaussianParameters ParseInput(String [] parameters)
    {
        GaussianParameters result = null;
        if(parameters.length == 3)
        {
            int numbersToGenerate = Integer.parseInt(parameters[0]);
            double mean = Double.parseDouble(parameters[1]);
            double deviation = Math.pow(Double.parseDouble(parameters[2]),2);
            result = new GaussianParameters(mean,deviation,numbersToGenerate);
        }
        else
        {
            return null;
            //System.err.println("Program ocekava jako vstup: [pocet cisel k vygenerovani] [stredni hodnota] [smerodatna odchylka]");
        }

        return result;

    }
}
