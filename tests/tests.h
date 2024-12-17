#ifndef BMP_TO_ASCII_TESTS_H
#define BMP_TO_ASCII_TESTS_H

#include <iostream>
#include "../bmp/bmp.h"

void T_startTests();
int T_imageToFile(std::string filename, struct pixel4 **image, int width, int height);

#endif //BMP_TO_ASCII_TESTS_H
