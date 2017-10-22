package com.company;

/**
 * Prepravka pro predani parametru gausova rozdeleni
 * Created by liska on 05.10.2017.
 */
public class GaussianParameters
{
    /**
     * Stredni hodnota
     */
    private double mean;

    /**
     * Odchylka
     */
    private double deviation;

    /**
     *
     * @param mean stredni hodnota
     * @param deviation odchylka
     */
    public GaussianParameters(double mean, double deviation)
    {
        this.mean = mean;
        this.deviation = deviation;
    }

    public double getMean()
    {
        return this.mean;
    }

    public double getDeviation()
    {
        return this.deviation;
    }

    public void setDeviation(double deviation)
    {
        this.deviation = deviation;
    }

    public void setMean(double mean)
    {
        this.mean = mean;
    }
}