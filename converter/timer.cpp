#include "timer.h"

double diffclock(clock_t clock1,clock_t clock2)
{
        double diffticks=clock1-clock2;
        double diffms=(diffticks*10)/CLOCKS_PER_SEC;
        return diffms;
}

