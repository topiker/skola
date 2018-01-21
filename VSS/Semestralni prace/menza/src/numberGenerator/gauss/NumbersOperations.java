package numberGenerator.gauss;

import java.util.List;
import java.util.Map;
import java.util.OptionalDouble;
import java.util.TreeMap;

/**
 * Trida pro praci s cisly
 * Created by liska on 05.10.2017.
 */
public class NumbersOperations
{

    /**
     * Pro vstupni pole cisel vypocita stredni hodnotu a odchylku
     * @param data
     * @return
     */
    public static GaussianParameters getGausianParameters(List<Double> data)
    {
        GaussianParameters parameters = null;

        double mean = calculateMean(data);
        double deviation = calculateDeviation(data, mean);


        parameters = new GaussianParameters(mean,deviation,data.size());
        return  parameters;
    }

    /**
     * Vypocita stredni hodnotu ze vstupnich dat
     * @param data
     * @return
     */
    private static double calculateMean(List<Double> data)
    {
        double mean = 0 ;
        OptionalDouble average = data.stream()
                .mapToDouble(a -> a)
                .average();
        if(average.isPresent())
        {
            mean = average.getAsDouble();
        }

        return mean;
    }

    /**
     * Vypocita odchylku pro vstupni data za vyuziti stredni hodnoty
     * @param data
     * @param mean
     * @return
     */
    private static double calculateDeviation(List<Double> data, double mean)
    {
        double deviation = 0;
        double n = data.size();
        double upperSum = 0;
        for (double item : data)
        {
            upperSum += Math.pow(item-mean,2);
        }
        return Math.sqrt(upperSum / (n-1));
    }

    /**
     * Metoda tvori ze vstupnich dat histogram, histogram je navic i serazen podle klice
     * @param data
     * @param histogramColumns
     * @return
     */
    public static TreeMap<Double,Integer> getHistogram(List<Double>data, int histogramColumns)
    {


        TreeMap<Double,Integer> result = new TreeMap<Double,Integer>();
        if(data!=null && data.size()>0)
        {
            //Vezmu maximalni a minimalni vygenerovanou hodnotu.
            // Zjistim si, jak budou velike jednotlive intervaly histogramu a vlozim si je do treemapy
            double min = Math.floor(data.stream()
                    .mapToDouble(a -> a)
                    .min().getAsDouble());
            double max = Math.ceil(data.stream()
                    .mapToDouble(a -> a)
                    .max().getAsDouble());
            double rangeForOne = (max - min)/(histogramColumns-1);
            for(int i = 0; i < histogramColumns;i++)
            {
                result.put(min+(rangeForOne*i),0);
            }

            //Pro kazde vygenerovane cislo budu hledat, do ktereho intervalu bude spadat.



            for (double current : data)
            {

                double previous=-Double.MAX_VALUE;
                int previousCount = 0;
                double actual;
                int actualCount = 0;
                for(Map.Entry<Double,Integer> entry : result.entrySet()) {

                    actual = entry.getKey();
                    actualCount = entry.getValue();
                    if(previous != Double.MAX_VALUE)
                    {
                        if (current >= previous && current < actual)
                        {
                            result.put(previous,previousCount+1);
                        }
                    }
                    previous = actual;
                    previousCount = actualCount;
                }
            }
        }

        return result;
    }

}


