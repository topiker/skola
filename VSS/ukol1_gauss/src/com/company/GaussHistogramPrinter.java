package com.company;

import java.text.DecimalFormat;
import java.util.Map;
import java.util.TreeMap;

/**
 * Created by liska on 21.10.2017.
 */
public class GaussHistogramPrinter {
    private TreeMap<Double,Integer> data;
    private int maxStars;
    public GaussHistogramPrinter(TreeMap<Double,Integer> data, int maxStars)
    {
        this.data = data;
        this.maxStars = maxStars;
    }

    public void printHistToConsole()
    {
        int maxHistValue = getMaxHistValue();
        int valuesNeededForStars = (int)Math.ceil((double)maxHistValue / (double)maxStars);
        printHistogram(this.data,valuesNeededForStars);
    }

    private int getMaxHistValue()
    {
        Map.Entry<Double,Integer> maxEntry = null;
        for (Map.Entry<Double,Integer> entry : data.entrySet())
        {
            if (maxEntry == null || entry.getValue().compareTo(maxEntry.getValue()) > 0)
            {
                maxEntry = entry;
            }
        }
        if(maxEntry!=null)
        {
            return maxEntry.getValue();
        }
        else
        {
            return 0;
        }
    }

    private String convertNumberToStars(int number, int valuesNeededForStars)
    {
        StringBuilder builder = new StringBuilder();
        int numberOfStars = (int)Math.floor((double)number/(double)valuesNeededForStars);
        for (int i = 0; i<numberOfStars;i++)
        {
            builder.append("*");
        }
        return builder.toString();
    }

    /**
     * Vypise vstup formou histogramu   cislo:******
     * @param histogram
     */
    private void printHistogram(TreeMap<Double,Integer> histogram,int valuesNeededForStars)
    {
        for(Map.Entry<Double,Integer> entry : histogram.entrySet()) {
            Double key = entry.getKey();
            Integer value = entry.getValue();

            System.out.println(new DecimalFormat("#0.00").format(key) + ":" + convertNumberToStars(value,valuesNeededForStars));
        }
    }
}
