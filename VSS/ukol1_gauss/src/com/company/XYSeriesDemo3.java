/* ===========================================================
 * JFreeChart : a free chart library for the Java(tm) platform
 * ===========================================================
 *
 * (C) Copyright 2000-2004, by Object Refinery Limited and Contributors.
 *
 * Project Info:  http://www.jfree.org/jfreechart/index.html
 *
 * This library is free software; you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free Software Foundation;
 * either version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License along with this
 * library; if not, write to the Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * [Java is a trademark or registered trademark of Sun Microsystems, Inc. 
 * in the United States and other countries.]
 *
 * ------------------
 * XYSeriesDemo3.java
 * ------------------
 * (C) Copyright 2004, by Object Refinery Limited and Contributors.
 *
 * Original Author:  David Gilbert (for Object Refinery Limited);
 * Contributor(s):   -;
 *
 * $Id: XYSeriesDemo3.java,v 1.6 2004/05/05 16:28:55 mungady Exp $
 *
 * Changes
 * -------
 * 03-Feb-2004 : Version 1 (DG);
 *
 */

package org.jfree.chart.demo;

import java.awt.Color;
import java.awt.Font;
import java.util.HashMap;
import java.util.Map;
import java.util.TreeMap;

import org.jfree.chart.ChartFactory;
import org.jfree.chart.ChartPanel;
import org.jfree.chart.JFreeChart;
import org.jfree.chart.plot.IntervalMarker;
import org.jfree.chart.plot.PlotOrientation;
import org.jfree.chart.plot.XYPlot;
import org.jfree.data.xy.IntervalXYDataset;
import org.jfree.data.xy.XYSeries;
import org.jfree.data.xy.XYSeriesCollection;
import org.jfree.ui.ApplicationFrame;
import org.jfree.ui.Layer;
import org.jfree.ui.RectangleAnchor;
import org.jfree.ui.RefineryUtilities;
import org.jfree.ui.TextAnchor;

import javax.swing.*;

/**
 * This demo shows a simple bar chart created using the {@link XYSeriesCollection} dataset.
 *
 */
public class XYSeriesDemo3 extends JFrame {

    /**
     * Creates a new demo instance.
     *
     * @param title the frame title.
     */
    public XYSeriesDemo3(final String title, TreeMap<Double, Integer> data) {
        super(title);
        IntervalXYDataset dataset = createDataset(data);
        JFreeChart chart = createChart(dataset);
        final ChartPanel chartPanel = new ChartPanel(chart);
        chartPanel.setPreferredSize(new java.awt.Dimension(500, 270));
        setContentPane(chartPanel);
    }

    public XYSeriesDemo3(final String title) {
        super(title);
        IntervalXYDataset dataset = createDataset();
        JFreeChart chart = createChart(dataset);
        final ChartPanel chartPanel = new ChartPanel(chart);
        chartPanel.setPreferredSize(new java.awt.Dimension(500, 270));
        setContentPane(chartPanel);
    }

    private IntervalXYDataset createDataset() {
        final XYSeries series = new XYSeries("Random Data");
        series.add(1.0, 400.2);
        series.add(5.0, 294.1);
        series.add(4.0, 100.0);
        series.add(12.5, 734.4);
        series.add(17.3, 453.2);
        series.add(21.2, 500.2);
        series.add(21.9, null);
        series.add(25.6, 734.4);
        series.add(30.0, 453.2);
        for(int i = 0; i < 1000;i++)
        {
            series.add(30.0 + i, 5000-i*5);
        }
        final XYSeriesCollection dataset = new XYSeriesCollection(series);
        return dataset;
    }

    /**
     * Creates a sample dataset.
     *
     * @return A sample dataset.
     */
    private IntervalXYDataset createDataset(TreeMap<Double, Integer> data) {
        XYSeries series = new XYSeries("Random Data");
        for (Map.Entry<Double, Integer> entry : data.entrySet()) {
            series.add(entry.getKey(), entry.getValue());
        }
        final XYSeriesCollection dataset = new XYSeriesCollection(series);
        return dataset;
    }

    /**
     * Creates a sample chart.
     *
     * @param dataset the dataset.
     * @return A sample chart.
     */
    private JFreeChart createChart(IntervalXYDataset dataset) {
        final JFreeChart chart = ChartFactory.createXYBarChart(
                "",
                "X",
                false,
                "Y",
                dataset,
                PlotOrientation.VERTICAL,
                true,
                true,
                true
        );
        return chart;
    }

    public static void main(String[] args) {
        {
            final XYSeriesDemo3 demo = new XYSeriesDemo3("XY Series Demo 3");
            demo.pack();
            RefineryUtilities.centerFrameOnScreen(demo);
            demo.setVisible(true);
        }
    }


}


           