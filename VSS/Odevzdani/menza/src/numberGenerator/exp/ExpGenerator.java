package numberGenerator.exp;

import cz.zcu.fav.kiv.jsim.JSimSystem;
import numberGenerator.ILambdaGenerator;

/**
 * Created by liska on 20.01.2018.
 * Exponencialni generator nahodnych cisel
 */
public class ExpGenerator implements ILambdaGenerator
{
    private double lambda;

    /**
     *
     * @param lambda stredni hodnota
     */
    public ExpGenerator(double lambda)
    {
        this.lambda = lambda;
    }

    @Override
    public double generateNumber()
    {
        return lambda;
    }

    @Override
    public double generateHoldTime()
    {
        return JSimSystem.negExp(lambda);
    }
}
