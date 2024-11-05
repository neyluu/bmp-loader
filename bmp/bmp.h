#ifndef BMP_TO_ASCII_BMP_H
#define BMP_TO_ASCII_BMP_H

#include <iostream>

class BMP
{
private:
public:
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

    struct pixel4* colorTable;
    struct pixel3** image;

    ~BMP();

    //--- Methods ---
    int load(const std::string& filename);
    int save(const std::string& filename);


    void printHeader();
    void printImage();
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

#endif //BMP_TO_ASCII_BMP_H
