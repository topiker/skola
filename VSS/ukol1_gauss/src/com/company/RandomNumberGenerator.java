package com.company;

import java.util.Map;

/**
 * Created by liska on 15.10.2017.
 */
public interface RandomNumberGenerator
{
    /**
     * Metoda zajistuje vygenerovani nahodnych cisel a vytvoreni histogramu
     * @return
     */
    Map<Double,Integer> getHistogram();

    /**
     * Metoda slouzi k pridani noveho vygenerovaneho cisla do histogramu
     * @param value
     * @return
     */
    void addNewRandomToHistogram(double value);

    /**
     * Vygeneruje nove nahodne cislo
     * @return
     */
    double generateNewNumber();

}
