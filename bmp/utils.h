#ifndef BMP_TO_ASCII_UTILS_H
#define BMP_TO_ASCII_UTILS_H

#include <cstdint>

// Function calculating padding needed to align data after each loaded row
// INPUT:
// bytes - number of bytes read during loading row
// OUTPUT:
// bytesToSkip - number of bytes to skip to align data
int calculatePadding(int bytes);

void splitByte(uint8_t byte, int *num1, int *num2);
void splitByte(uint8_t byte, int *num1, int *num2, int *num3, int *num4);
void splitByte(uint8_t byte, int (&nums)[8]);

#endif //BMP_TO_ASCII_UTILS_H
