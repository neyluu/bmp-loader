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

    bool operator==(const pixel4 &pixel) const
    {
        if(r == pixel.r && g == pixel.g && b == pixel.b && a == pixel.a) return true;
        return false;
    }
    bool operator!=(const pixel4 &pixel) const
    {
        return !(*this == pixel);
    }
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
     * because this data is common for all of these headers.
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
    size_t actualFileSize;

// === load.cpp ===

// Function loading headerFile
// OUTPUT:
// errorCode
//  0 - OK
//  1 - loading failed
    int loadHeaderFile();

// Function loading headerDIB
// OUTPUT:
// errorCode
//  0 - OK
//  1 - loading failed
    int loadHeaderDIB();

    void loadImage1 ();
    void loadImage2 ();
    void loadImage4 ();
    void loadImage8 ();
    void loadImage16();
    void loadImage24();
    void loadImage32();

    void readColorTable();
    void getDirection(int height, int& i, int& change);

// === save.cpp ===

    int saveHeaderFile();
    int saveHeaderDIB();
    int saveColorTable();
    int updateFileSize();
    void prepareHeaders();
    uint8_t joinByte(int row, int col, int pixels);

// Function returning color index from colorTable
// INPUT
// color - pixel that contains searched color
// OUTPUT:
// index - index of searched color
// -1 - if color wasn't found
    int getColorIndex(struct pixel4 color);

    int saveImage1 ();
    int saveImage2 ();
    int saveImage4 ();
    int saveImage8 ();
    int saveImage16();
    int saveImage24();
    int saveImage32();

// === bmp.cpp ===

    int colorTableSize();
public:

    struct headerFile headerFile;
    struct headerDIB headerDIB;

    struct pixel4* colorTable;
    struct pixel4** image;

    BMP();
    ~BMP();

// === load.cpp ===
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

// === save.cpp ===
// Function save BMP into filename given in parameter
// INPUT
// filename - name or path to file to save, if filename don`t ends with .bmp, it`s added automatically
// OUTPUT:
// errorCode
//  0 - OK
    int save(std::string filename);


    void printHeader();
    void printColorTable();
    void printImage();
};

#endif //BMP_TO_ASCII_BMP_H
