#include <stdio.h>
#include "FIFO.h"

void fifo(int *pos, int pageFrame)
{
    *pos ++;
    *pos = (*pos) % pageFrame;
}
