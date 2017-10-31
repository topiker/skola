package com.company;
import java.util.List;
import java.util.TreeMap;

public class Main {

    public static final int histogramColumns = 49;
    public static final int maximalniPocetHvezdicek = 50;


    public static void main(String[] args) {

        InputParser parser = new InputParser();
        GaussianParameters parameters = parser.ParseInput(args);
        if(parameters == null)
        {
            runWithExampleParameters();
        }
        else
        {
            run(parameters);
        }



    }

    public static void runWithExampleParameters()
    {
        List<GaussianParameters> parametersList = GaussianParameters.GetFactoryParameters();
        for (GaussianParameters parameters: parametersList)
        {
            run(parameters);
            System.out.println();
        }
    }

    public static void run(GaussianParameters parameters)
    {
        GaussianDistribution distribution = new GaussianDistribution(parameters,parameters.getPocetCiselKVygenerovani(),histogramColumns);
        TreeMap<Double,Integer> histogram = distribution.getHistogram();
        GaussianParameters realParameters = distribution.getRealGaussianParameters();

        System.out.println("E_teorie="+parameters.getMean());
        System.out.println("D_teorie="+parameters.getOdchylka());
        System.out.println("E_vypocet="+realParameters.getMean());
        System.out.println("D_vypocet="+realParameters.getOdchylka());
        System.out.println();

        GaussHistogramPrinter printer = new GaussHistogramPrinter(histogram,maximalniPocetHvezdicek);
        printer.printHistToConsole();
    }

}
