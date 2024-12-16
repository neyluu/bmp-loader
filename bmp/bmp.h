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

struct headerFile
{
    char signature[3];
    int32_t fileSize;
    uint8_t reserved[4];
    int32_t dataOffset;
};
struct headerDIB
{
    /*
     * Library uses BITMAPINFOHEADER (40 bytes).
     * BMPs with BITMAPV2INFOHEADER, BITMAPV3INFOHEADER, BITMAPV4HEADER or BITMAPV5HEADER
     * are supported but only first 40 bytes of header is loaded and used,
     * because this data is common for all of this headers.
     */
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
};

class BMP
{
private:
    FILE *file;
    int loadHeaderFile();
    int loadHeaderDIB();
public:
// Function loading headerFile
// OUTPUT:
// errorCode
//  0 - OK
//  1 - loading failed
    struct headerFile headerFile;
    struct headerDIB headerDIB;

    struct pixel4* colorTable;
    struct pixel3** image;

    BMP();
    ~BMP();

// Function loads BMP file
// INPUT
// filename - name or path to file to load
// OUTPUT:
// errorCode
//  0 - OK
//  1 - opening file failed
//  2 - loading headerFile failed
//  3 - loading headerDIB failed
    int load(const std::string& filename);
    int save(const std::string& filename);


    void printHeader();
    void printImage();
};

#endif //BMP_TO_ASCII_BMP_H
