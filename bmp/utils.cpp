#include "utils.h"

#include <iostream>

int calculatePadding(int cols)
{
    int loadedBytes = 3 * abs(cols);
    int padding = 1;

    if(loadedBytes % 4 == 0) return 0;
    else
    {
        while((loadedBytes + padding) % 4 != 0)
        {
            padding++;
        }
    }

    return padding;
}

void splitByte(unsigned char byte, int *num1, int *num2)
{
    *num1 = 0;
    int counter = 0;

    for(int i = 4; i <= 7; i++)
    {
        if((byte >> i) & 1)
        {
            *num1 |= (1 << counter);
        }
        counter++;
    }

    *num2 = byte;
    for(int i = 7; i >= 4; i--)
    {
        *num2 &= ~(1 << i);
    }
}
void splitByte(unsigned char byte, int *num1, int *num2, int *num3, int *num4)
{

}
void splitByte(unsigned char byte, int **nums)
{

}