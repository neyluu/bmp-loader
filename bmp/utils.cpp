#include "utils.h"

#include <iostream>

// Function calculating padding needed to align data after each loaded row
// INPUT:
// bytes - number of bytes read during loading row
// OUTPUT:
// bytesToSkip - number of bytes to skip to align data
int calculatePadding(int bytes)
{
    int padding = 1;

    if(bytes % 4 == 0) return 0;

    while((bytes + padding) % 4 != 0)
    {
        padding++;
    }

    return padding;
}

void splitByte(uint8_t byte, int *num1, int *num2)
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
void splitByte(uint8_t byte, int *num1, int *num2, int *num3, int *num4)
{
    *num1 = 0;
    *num2 = 0;
    *num3 = 0;
    *num4 = 0;

    if((byte >> 7) & 1) *num1 |= (1 << 1);
    if((byte >> 6) & 1) *num1 |= (1 << 0);

    if((byte >> 5) & 1) *num2 |= (1 << 1);
    if((byte >> 4) & 1) *num2 |= (1 << 0);

    if((byte >> 3) & 1) *num3 |= (1 << 1);
    if((byte >> 2) & 1) *num3 |= (1 << 0);

    if((byte >> 1) & 1) *num4 |= (1 << 1);
    if((byte >> 0) & 1) *num4 |= (1 << 0);
}
void splitByte(uint8_t byte, int (&nums)[8])
{
    for(int i = 0; i < 8; i++)
    {
        if(((byte >> (7 - i)) & 1) == 1) nums[i] = 1;
        else nums[i] = 0;
    }
}