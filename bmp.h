#ifndef BMP_TO_ASCII_BMP_H
#define BMP_TO_ASCII_BMP_H

#include <iostream>

struct BMPfile {
    //Header
    std::string signature;
    int fileSize;
    char reserved[4];
    int dataOffset;

    //Info Header
    int size;
    int width;
    int height;
    short int planes;
    short int bitCount;
    int compression;
    int imageSize;
    int XpixelsPerM;
    int YpixelsPerM;
    int colorsUsed;
    int colorsImportant;

    struct pixel4** colorPalette;

    struct pixel3** image;
};

struct pixel3
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

struct pixel4
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
    unsigned char a;
};

void readBMPfile(struct BMPfile* fileData, FILE* f);
void printBMPfileData(struct BMPfile file);
void freeImagePixel3(struct pixel3** image, int height);

int calculatePadding(unsigned int cols);
void loadImage1 (FILE *f, struct BMPfile *data);
void loadImage2 (FILE *f, struct BMPfile *data);
void loadImage4 (FILE *f, struct BMPfile *data);
void loadImage8 (FILE *f, struct BMPfile *data);
void loadImage16(FILE *f, struct BMPfile *data);
void loadImage24(FILE *f, struct BMPfile *data);
void loadImage32(FILE *f, struct BMPfile *data);

#endif //BMP_TO_ASCII_BMP_H
