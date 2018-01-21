package numberGenerator;

/**
 * Created by liska on 20.01.2018.
 */
public interface ILambdaGenerator
{
    /**
     * Vygeneruje nahodne cislo. (Lambda)
     * @return nahodne cislo
     */
    double generateNumber();

    /**
     * Vygenruje cas k cekani v systemu.
     * @return
     */
    double generateHoldTime();
}
