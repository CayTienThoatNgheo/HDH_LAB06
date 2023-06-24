#include <stdio.h>

void fifo(int *pos, int pageFrame)
{
    *pos ++;
    *pos = (*pos) % pageFrame;
}