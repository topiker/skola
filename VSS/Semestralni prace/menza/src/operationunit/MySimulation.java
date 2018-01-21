package operationunit;

import cz.zcu.fav.kiv.jsim.JSimInvalidParametersException;
import cz.zcu.fav.kiv.jsim.JSimSimulation;

/**
 * Created by liska on 21.01.2018.
 * Trida obalujici celou simulaci.
 * Uchovavaji se zde statistiky spolecne cely system
 */
public class MySimulation extends JSimSimulation
{

    /**
     * Kolik processu celkove proslo system
     */
    private int totalExitedProcesses;
    /**
     * Suma doby behu vsech procesu v systemu
     */
    private double totalTimeOfProcessesInSystem;

    public MySimulation(String s) throws JSimInvalidParametersException {
        super(s);
    }

    /**
     * Zvysi pocet proslych pozadavku systeme o jedna
     */
    public void incrementTotalExitedProcesses()
    {
        this.totalExitedProcesses +=1;
    }

    /**
     * Zvysi celkovou dobu behu procesu v systemu o xx
     * @param timeToAdd
     */
    public void addTotalTimeOfProcessesInSystem( double timeToAdd)
    {
        this.totalTimeOfProcessesInSystem+=timeToAdd;
    }

    /**
     * Celkova doba behu procesů v systemu
     * @return
     */
    public double getTotalTimeOfProcessesInSystem()
    {
        return this.totalTimeOfProcessesInSystem;
    }

    /**
     * Cekovy pocet pozadavku, ktere prosli systemem
     * @return
     */
    public int getTotalExitedProcesses()
    {
        return this.totalExitedProcesses;
    }


}
