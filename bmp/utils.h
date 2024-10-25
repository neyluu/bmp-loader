#ifndef BMP_TO_ASCII_UTILS_H
#define BMP_TO_ASCII_UTILS_H

int calculatePadding(int cols);

void splitByte(unsigned char byte, int *num1, int *num2);
void splitByte(unsigned char byte, int *num1, int *num2, int *num3, int *num4);
void splitByte(unsigned char byte, int (&nums)[8]);

#endif //BMP_TO_ASCII_UTILS_H
