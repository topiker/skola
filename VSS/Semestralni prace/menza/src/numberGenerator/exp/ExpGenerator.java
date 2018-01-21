package numberGenerator.exp;

import cz.zcu.fav.kiv.jsim.JSimSystem;
import numberGenerator.ILambdaGenerator;

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
        return JSimSystem.negExp(lambda);
    }
}
