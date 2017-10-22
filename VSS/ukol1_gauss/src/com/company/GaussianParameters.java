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
    private double odchylka;

    /**
     * Pocet cisel, ktere se maji vygenerovat
     */
    private int pocetCiselKVygenerovani;

    /**
     *
     * @param mean stredni hodnota
     * @param odchylka odchylka
     * @param pocetCisel pocet cisel, ktere se maji vygenerovat
     */
    public GaussianParameters(double mean, double odchylka, int pocetCisel)
    {
        this.mean = mean;
        this.odchylka = odchylka;
        this.pocetCiselKVygenerovani = pocetCisel;
    }

    /**
     * Vraci stredni hodnotu
     * @return
     */
    public double getMean()
    {
        return this.mean;
    }

    /**
     * Vraci odchylku
     * @return
     */
    public double getOdchylka()
    {
        return this.odchylka;
    }

    /**
     * Nastavuje odchylku
     * @param odchylka
     */
    public void setOdchylka(double odchylka)
    {
        this.odchylka = odchylka;
    }

    /**
     * Nastavuje rozptyl
     * @param rozptyl
     */
    public void setRozptyl(double rozptyl)
    {
        this.odchylka = Math.sqrt(rozptyl);
    }

    /**
     * Nastavuje stredni hodnotu
     * @param mean
     */
    public void setMean(double mean)
    {
        this.mean = mean;
    }

    /**
     * Vraci rozptyl
     * @return
     */
    public double getRozptyl()
    {
        return Math.pow(this.odchylka,2);
    }


    /***
     * Vraci pocet cisel, ktere se maji vygenerovat
     * @return
     */
    public int getPocetCiselKVygenerovani()
    {
        return this.pocetCiselKVygenerovani;
    }
}