package numberGenerator;

/**
 * Created by liska on 20.01.2018.
 */
public class ExpGenerator implements ILambdaGenerator
{
    private double lambda;

    public ExpGenerator(double lambda)
    {
        this.lambda = lambda;
    }

    @Override
    public double generateNumber()
    {
        return lambda;
    }
}
