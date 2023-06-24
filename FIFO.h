#include <stdio.h>

int fifo(int pos, int pageFrame)
{
    pos ++;
    pos = pos % pageFrame;
    return pos;
}