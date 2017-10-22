package com.company;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartFrame;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.demo.XYSeriesDemo3;
import org.jfree.chart.plot.CategoryPlot;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.renderer.category.BarRenderer;
import org.jfree.chart.renderer.category.StandardBarPainter;
import org.jfree.data.category.DefaultCategoryDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.ui.RefineryUtilities;
import sun.reflect.generics.tree.Tree;

import java.awt.*;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;
import java.util.TreeMap;

public class Main {

    public static void main(String[] args) {

        InputParser parser = new InputParser();
        GaussianParameters parameters = parser.ParseInput(args);
        int histogramColumns = 49;
        int maximalniPocetHvezdicek = 100;
        int numberCount = 1000000;

        GaussianDistribution distribution = new GaussianDistribution(parameters,numberCount,histogramColumns);
        TreeMap<Double,Integer> histogram = distribution.getHistogram();
        GaussianParameters realParameters = distribution.getRealGaussianParameters();

        System.out.println("E_teorie="+parameters.getMean());
        System.out.println("D_teorie="+parameters.getOdchylka());
        System.out.println("E_vypocet="+realParameters.getMean());
        System.out.println("D_vypocet="+realParameters.getOdchylka());

        printHistogram(histogram);

        GaussHistogramPrinter printer = new GaussHistogramPrinter(histogram,maximalniPocetHvezdicek);
        printer.printHistToConsole();

        XYSeriesDemo3 histVisualitazion = new XYSeriesDemo3("Generovani nahodnych cisel - Gaussovo rozdeleni",histogram);
        histVisualitazion.pack();
        RefineryUtilities.centerFrameOnScreen(histVisualitazion);
        histVisualitazion.setVisible(true);

    }

    /**
     * Vypise vstup formou histogramu   cislo:******
     * @param histogram
     */
    private static void printHistogram(TreeMap<Double,Integer> histogram)
    {

        /*String windowtitle, String charttitle, String xlabel, String ylabel, int type*/
        /*Histogram h = new Histogram("Generovani cisel", "Gaussovo rozdelení","Hodnoty","Pocet hodnot",2);

        for(Map.Entry<Double,Integer> entry : histogram.entrySet()) {
            Double key = entry.getKey();
            Integer value = entry.getValue();
            h.add(value,value.toString(),key.toString());
        }
        h.setSize(800,600);
        h.setVisible(true);*/

       /* for(Map.Entry<Double,Integer> entry : histogram.entrySet()) {
            Double key = entry.getKey();
            Integer value = entry.getValue();

            System.out.println(key + ":" + convertNumberToStars(value));
        }*/
    }

    /**
     * Prevede cislo na retezec hvezdicek
     * @param number
     * @return
     */
    private static String convertNumberToStars(int number)
    {
        StringBuilder builder = new StringBuilder();
        for (int i = 0; i<number;i++)
        {
            builder.append("*");
        }
        return builder.toString();
    }

    private static void Histogram(Map<Double,Integer> data)
    {
        DefaultCategoryDataset dataset = new DefaultCategoryDataset();
        Iterator it = data.entrySet().iterator();
        while (it.hasNext()) {
            Map.Entry pair = (Map.Entry)it.next();
            dataset.setValue((int)pair.getValue(),""+(int)pair.getValue(),""+(double)pair.getKey());
        }
        JFreeChart chart = ChartFactory.createBarChart("Generator cisel s Gaussovym rozdelenim","Hodnota","Pocet hodnot",dataset, PlotOrientation.VERTICAL,false,true,false);
        CategoryPlot cplot = chart.getCategoryPlot();
        BarRenderer barRenderer = (BarRenderer)cplot.getRenderer();
        barRenderer.setSeriesPaint(0, Color.gray);
        ChartFrame frame = new ChartFrame("Generovani nahodnych cisel - Gaussovo rozdeleni",chart);
        frame.setSize(800,600);
        frame.setVisible(true);

    }

}
