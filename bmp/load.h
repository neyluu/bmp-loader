#ifndef BMP_TO_ASCII_LOAD_H
#define BMP_TO_ASCII_LOAD_H

#include <iostream>
#include <cmath>
#include <cstdint>

#include "bmp.h"

void loadImage1 (FILE *f, class BMP& data);
void loadImage2 (FILE *f, class BMP& data);
void loadImage4 (FILE *f, class BMP& data);
void loadImage8 (FILE *f, class BMP& data);
void loadImage16(FILE *f, class BMP& data);
void loadImage24(FILE *f, class BMP& data);
void loadImage32(FILE *f, class BMP& data);

void readColorTable(FILE *f, class BMP& data);
void getDirection(int height, int& i, int& change);

#endif //BMP_TO_ASCII_LOAD_H
