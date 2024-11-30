#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "bmp.h"
#include "load.h"

#include <cstring>

BMP::BMP()
{
    memset(signature, 0, 3);
    fileSize = 0;
    memset(reserved, 0, 4);
    dataOffset = 0;
    size = 0;
    width = 0;
    height = 0;
    planes = 0;
    bitCount = 0;
    compression = 0;
    imageSize = 0;
    XpixelsPerM = 0;
    YpixelsPerM = 0;
    colorsUsed = 0;
    colorsImportant = 0;
    colorTable = nullptr;
    image = nullptr;
}

BMP::~BMP()
{
    if(image != nullptr)
    {
        for(int i = 0; i < height; i++)
        {
            delete[] image[i];
        }
        delete[] image;
    }

    delete[] colorTable;
}

int BMP::load(const std::string& filename)
{
    FILE *f = fopen(filename.c_str(), "rb");
    if(f == nullptr) return 1;

    //Reading File Header
    fread(&signature, 1, 2, f);
    fread(&fileSize, 4, 1, f);
    fread(reserved, 4, 1, f);
    fread(&dataOffset, 4, 1, f);

    //Reading DIB header
    fread(&size, 4, 1, f);
    fread(&width, 4, 1, f);
    fread(&height, 4, 1, f);
    fread(&planes, 2, 1, f);
    fread(&bitCount, 2, 1, f);
    fread(&compression, 4, 1, f);
    fread(&imageSize, 4, 1, f);
    fread(&XpixelsPerM, 4, 1, f);
    fread(&YpixelsPerM, 4, 1, f);
    fread(&colorsUsed, 4, 1, f);
    fread(&colorsImportant, 4, 1, f);

    if(compression != 0)
    {
        fclose(f);
        return 1;
    }

    image = new struct pixel3*[height];
    for(int i = 0; i < height; i++)
    {
        image[i] = new struct pixel3[width];
    }

    switch (bitCount) {
        case 1:
            loadImage1 (f, *this);
            break;
        case 2:
            loadImage2 (f, *this);
            break;
        case 4:
            loadImage4 (f, *this);
            break;
        case 8:
            loadImage8 (f, *this);
            break;
        case 16:
            loadImage16 (f, *this);
            break;
        case 24:
            loadImage24(f, *this);
            break;
        default:
            break;
    }

    fclose(f);
    return 0;
}

int BMP::save(const std::string& filename)
{

    return 0;
}

void BMP::printHeader()
{
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Signature: " << signature << std::endl;
    std::cout << "File size: " << fileSize << std::endl;
    std::cout << "Data offset: " << dataOffset << std::endl;

    std::cout << "Size: " << size << std::endl;
    std::cout << "Width: " << width << std::endl;
    std::cout << "Height: " << height << std::endl;
    std::cout << "Planes: " << planes << std::endl;
    std::cout << "Bit count: " << bitCount << std::endl;
    std::cout << "Compression: " << compression << std::endl;
    std::cout << "Image size: " << imageSize << std::endl;
    std::cout << "X pixels per meter: " << XpixelsPerM << std::endl;
    std::cout << "Y pixels per meter: " << YpixelsPerM << std::endl;
    std::cout << "Colors used: " << colorsUsed << std::endl;
    std::cout << "Colors important: " << colorsImportant << std::endl;
    std::cout << "--------------------------------------" << std::endl;
}

void BMP::printImage()
{
    //TODO reading into array based on width -> bottom-up or top-down
    //TODO https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader

    for(int i = 0; i < abs(height); i++)
    {
        for(int j = 0; j < width; j++)
        {
            std::cout.width( 3 );
            std::cout << int(image[i][j].r) << ' ';

            std::cout.width( 3 );
            std::cout << int(image[i][j].g) << ' ';

            std::cout.width( 3 );
            std::cout << int(image[i][j].b) << ' ';

            std::cout << "| ";
        }
        std::cout << std::endl;
    }
}
