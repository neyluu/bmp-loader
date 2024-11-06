#ifndef BMP_TO_ASCII_UTILS_H
#define BMP_TO_ASCII_UTILS_H

#include <cstdint>

int calculatePadding(int cols);

void splitByte(uint8_t byte, int *num1, int *num2);
void splitByte(uint8_t byte, int *num1, int *num2, int *num3, int *num4);
void splitByte(uint8_t byte, int (&nums)[8]);

#endif //BMP_TO_ASCII_UTILS_H
