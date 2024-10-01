#include <cstdio>
#include <iostream>

#include "bmp.h"

int main ()
{
    BMP fileData;

    FILE * file;
    file = fopen("88.bmp", "rb");
    if (file == nullptr)
    {
        std::cout << "Cannot open file";
        return 1;
    }

    readBMPfile(&fileData, file);
    printBMPfileData(fileData);

    freeImagePixel3(fileData.image, fileData.height);
    freeColorTablePixel4(fileData.colorTable, file)
    fclose(file);
}


