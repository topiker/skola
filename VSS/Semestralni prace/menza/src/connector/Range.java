package connector;

/**
 * Created by liska on 20.01.2018.
 * Trida slouzi jako prepravka pro rozsah
 */
public class Range
{
    /***
     * Od
     */
    public final double from;
    /***
     * Do
     */
    public final double to;

    /**
     *
     * @param from - od
     * @param to - do
     */
    public Range(double from, double to)
    {
        this.from = from;
        this.to = to;
    }
}
