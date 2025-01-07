#include <cstring>
#include <cmath>

#include "bmp.h"


BMP::BMP()
{
    memset(headerFile.signature, 0, 3);
    headerFile.fileSize = 0;
    memset(headerFile.reserved, 0, 4);
    headerFile.dataOffset = 0;

    headerDIB.size = 0;
    headerDIB.width = 0;
    headerDIB.height = 0;
    headerDIB.planes = 0;
    headerDIB.bitCount = 0;
    headerDIB.compression = 0;
    headerDIB.imageSize = 0;
    headerDIB.XpixelsPerM = 0;
    headerDIB.YpixelsPerM = 0;
    headerDIB.colorsUsed = 0;
    headerDIB.colorsImportant = 0;
    colorTable = nullptr;
    image = nullptr;

    file = nullptr;
    actualFileSize = 0;
}
BMP::~BMP()
{
    if(image != nullptr)
    {
        for(int i = 0; i < headerDIB.height; i++)
        {
            delete[] image[i];
        }
        delete[] image;
    }

    delete[] colorTable;
}

void BMP::printHeader()
{
    std::cout << "--------------------------------------" << std::endl;
    std::cout << "Signature: "      << headerFile.signature << std::endl;
    std::cout << "File size: "      << headerFile.fileSize << std::endl;
    std::cout << "Data offset: "    << headerFile.dataOffset << std::endl;

    std::cout << "Size: "               << headerDIB.size << std::endl;
    std::cout << "Width: "              << headerDIB.width << std::endl;
    std::cout << "Height: "             << headerDIB.height << std::endl;
    std::cout << "Planes: "             << headerDIB.planes << std::endl;
    std::cout << "Bit count: "          << headerDIB.bitCount << std::endl;
    std::cout << "Compression: "        << headerDIB.compression << std::endl;
    std::cout << "Image size: "         << headerDIB.imageSize << std::endl;
    std::cout << "X pixels per meter: " << headerDIB.XpixelsPerM << std::endl;
    std::cout << "Y pixels per meter: " << headerDIB.YpixelsPerM << std::endl;
    std::cout << "Colors used: "        << headerDIB.colorsUsed << std::endl;
    std::cout << "Colors important: "   << headerDIB.colorsImportant << std::endl;
    std::cout << "--------------------------------------" << std::endl;
}

void BMP::printColorTable()
{
    std::cout << "Color table:" << std::endl;
    for(int i = 0; i < headerDIB.colorsUsed; i++)
    {
        std::cout << i << ". "  << int(colorTable[i].r) << ' '
                                << int(colorTable[i].g) << ' '
                                << int(colorTable[i].b) << ' '
                                << int(colorTable[i].a) << std::endl;
    }
    std::cout << std::endl;
}

void BMP::printImage()
{
    //TODO reading into array based on width -> bottom-up or top-down
    //TODO https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader

    for(int i = 0; i < abs(headerDIB.height); i++)
    {
        for(int j = 0; j < headerDIB.width; j++)
        {
            std::cout.width( 3 );
            std::cout << int(image[i][j].r) << ' ';

            std::cout.width( 3 );
            std::cout << int(image[i][j].g) << ' ';

            std::cout.width( 3 );
            std::cout << int(image[i][j].b) << ' ';

            std::cout.width(3);
            std::cout << int(image[i][j].a) << ' ';

            std::cout << "| ";
        }
        std::cout << std::endl;
    }
}

int BMP::colorTableSize()
{
    if(headerDIB.colorsUsed == 0) return int(pow(2, headerDIB.bitCount));
    return headerDIB.colorsUsed;
}
