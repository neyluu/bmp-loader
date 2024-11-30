#ifndef BMP_TO_ASCII_BMP_H
#define BMP_TO_ASCII_BMP_H

#include <iostream>
#include <cstdint>

struct pixel3
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct pixel4
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
};

class BMP
{
private:
public:
    char signature[3];
    int32_t fileSize;
    uint8_t reserved[4];
    int32_t dataOffset;

    //Info Header
    int32_t size;
    int32_t width;
    int32_t height;
    int16_t planes;
    int16_t bitCount;
    int32_t compression;
    int32_t imageSize;
    int32_t XpixelsPerM;
    int32_t YpixelsPerM;
    int32_t colorsUsed;
    int32_t colorsImportant;

    struct pixel4* colorTable;
    struct pixel3** image;

    BMP();
    ~BMP();

    int load(const std::string& filename);
    int save(const std::string& filename);


    void printHeader();
    void printImage();
};

#endif //BMP_TO_ASCII_BMP_H
