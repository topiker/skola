package com.company;

import java.util.TreeMap;

/**
 * Trida slouzi k vygenerovani seznamu nahodnych cisel podle normalniho rozdeleni
 * Created by liska on 05.10.2017.
 */
public class GaussianDistribution implements GaussianDistributor, RandomNumberGenerator {

    private GaussianParameters parameters;

    /**
     * Kolik se vygeneruje cisel
     */
    private int numbersToGenerate;

    /**
     * Vypocitana stredni hodnota a odchylka z vygenerovanych cisel
     */
    private GaussianParameters realParameters;

    /***
     * Histogram vygenerovanych cisel
     */
    private TreeMap<Double, Integer> histogram;

    /**
     * Pocet sloupcu v histogramu
     */
    private int histogramColumns;

    /**
     * Rozsah hodnot pro jeden sloupec intervalu
     */
    private double rangeForOneColumn;

    /**
     * Odhadnuta minimalni hodnota vygenerovaneho cisla
     */
    private double predictedMinValue;

    /**
     * Odhadnuta maximalni hodnota vygenerovaneho cisla
     */
    private double predictedMaxValue;

    /**
     * Counter vygenerovanych cisel v histogramu
     */
    private int numbersCount;

    /**
     * Flag, zde se nove cislo ma generovat, nebo pouzit predpocitane z predchoziho
     */
    private boolean generate;

    /**
     * Predpocitana hodnota z predchoziho random cisla (box miller)
     */
    private double z1;

    private int notIn = 0;
    /**
     *
     * @param parameters objekt s parametry normalniho rozdeleni (odchylka, stredni hodnota)
     * @param numbersToGenerate pocet cisel, ktere se maji vygenerovat
     * @param histogramColumns pocet sloupcu histogramu
     */
    public GaussianDistribution(GaussianParameters parameters, int numbersToGenerate, int histogramColumns)
    {
        this.parameters = parameters;
        this.numbersToGenerate = numbersToGenerate;
        this.realParameters = new GaussianParameters(parameters.getMean(),parameters.getOdchylka(),0);
        this.histogramColumns = histogramColumns;

        predictedMinValue = parameters.getMean() - this.parameters.getOdchylka()*3;
        predictedMaxValue = parameters.getMean() + this.parameters.getOdchylka()*3;
        rangeForOneColumn = (predictedMaxValue - predictedMinValue)/(histogramColumns-1);
        initHistogram();
        generateNumbersToHistogram();
    }

    /**
     * Metoda prepocita stredni hodnotu a odchylku
     * @param addedNumber
     */
    private void recalcDeviationAndMean(double addedNumber)
    {
        if(numbersCount>0)
        {
            //mean[n+1] = ((n * mean[n]) + value)/(n+1)
            double oldMean = this.realParameters.getMean();
            double newMean = ((numbersCount * oldMean)+addedNumber)/(numbersCount+1);

            double oldDeviation = this.realParameters.getRozptyl();
            //(n * (var[n] + mean[n]^2) + value^2)/(n+1) - mean[n+1]^2
            double top = numbersCount*(oldDeviation + Math.pow(oldMean,2)) + Math.pow(addedNumber,2);
            double newVariation = top/(numbersCount+1) - Math.pow(newMean,2);
            this.realParameters.setMean(newMean);
            this.realParameters.setRozptyl(newVariation);
        }

    }

    @Override
    public GaussianParameters getRealGaussianParameters() {
        return this.realParameters;
    }

    @Override
    public TreeMap<Double, Integer> getHistogram() {
        return this.histogram;
    }

    @Override
    public void addNewRandomToHistogram(double valueToAdd) {

        double next = 0;
        double current = 0;
        int currentCount = 0;
        if(valueToAdd < predictedMinValue || valueToAdd > predictedMaxValue)
        {
            return;
        }
        for(int i = 0; i < histogramColumns;i++)
        {
            current = predictedMinValue+(rangeForOneColumn*(i));
            next = predictedMinValue+(rangeForOneColumn*(i+1));

            if (current < valueToAdd  && next > valueToAdd)
            {
                currentCount = histogram.get(current);
                histogram.put(current,currentCount+1);
            }
        }
          numbersCount+=1;
          recalcDeviationAndMean(valueToAdd);
    }

    /**
     * Vytvori strukturu pro ulozeni histogramu
     */
    private void initHistogram()
    {
        this.histogram = new TreeMap<Double,Integer>();
        for(int i = 0; i < histogramColumns;i++)
        {
            histogram.put(predictedMinValue+(rangeForOneColumn*i),0);
        }
    }

    /***
     * Do histogramu vygeneruje cisla (tolik, kolik bylo predano v konstruktoru)
     */
    private void generateNumbersToHistogram()
    {
        for(int i = numbersCount; i< this.numbersToGenerate;i = numbersCount)
        {
            addNewRandomToHistogram(generateNewNumber());
        }
    }


    @Override
    public double generateNewNumber() {
        generate = !generate;
        if(!generate)
        {
            return z1 * this.parameters.getOdchylka() + this.parameters.getMean();
        }
        double u1 = 1.0-Math.random(); //uniform(0,1] random doubles
        double u2 = 1.0-Math.random();
        double z0;
        double twoPiU2 = 2*Math.PI*u2;
        double R = -2.0 * Math.log(u1);
        z0 = Math.sqrt(R)*Math.cos(twoPiU2);
        z1 = Math.sqrt(R)*Math.sin(twoPiU2);
        return z0*this.parameters.getOdchylka() + this.parameters.getMean();
    }
}
