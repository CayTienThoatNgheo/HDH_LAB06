#include <stdio.h>

void change_pos(int *pos, int pageFrame)
{
    *pos ++;
    *pos = (*pos) % pageFrame;
}
