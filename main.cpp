#include <cstdio>
#include <iostream>

struct BMPfile {
    //Header
    char signature[2];
    int fileSize;
    char reserved[4];
    int dataOffset;

    //InfoHeader
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
};

void readBMPfile(struct BMPfile* fileData, FILE* f);
void printBMPfileData(struct BMPfile file);

int main ()
{
    BMPfile fileData;

    FILE * file;
    file = fopen ("image.bmp", "rb");
    if (file == nullptr)
    {
        std::cout << "Cannot open file";
        return 1;
    }

    readBMPfile(&fileData, file);
    printBMPfileData(fileData);

    fclose(file);
}

void readBMPfile(struct BMPfile* fileData, FILE* f)
{
    fread(fileData->signature, 1, 2, f);
    fread(&fileData->fileSize, 4, 1, f);
    fread(fileData->reserved, 4, 1, f);
    fread(&fileData->dataOffset, 4, 1, f);

    fread(&fileData->size, 4, 1, f);
    fread(&fileData->width, 4, 1, f);
    fread(&fileData->height, 4, 1, f);
    fread(&fileData->planes, 2, 1, f);
    fread(&fileData->bitCount, 2, 1, f);
    fread(&fileData->compression, 4, 1, f);
    fread(&fileData->imageSize, 4, 1, f);
    fread(&fileData->XpixelsPerM, 4, 1, f);
    fread(&fileData->YpixelsPerM, 4, 1, f);
    fread(&fileData->colorsUsed, 4, 1, f);
    fread(&fileData->colorsImportant, 4, 1, f);
}
void printBMPfileData(struct BMPfile file)
{
    std::cout << "Signature: " << file.signature << std::endl;
    std::cout << "File size: " << file.fileSize << std::endl;
    std::cout << "Data offset: " << file.dataOffset << std::endl;

    std::cout << "Size: " << file.size << std::endl;
    std::cout << "Width: " << file.width << std::endl;
    std::cout << "Height: " << file.height << std::endl;
    std::cout << "Planes: " << file.planes << std::endl;
    std::cout << "Bit count: " << file.bitCount << std::endl;
    std::cout << "Compression: " << file.compression << std::endl;
    std::cout << "Image size: " << file.imageSize << std::endl;
    std::cout << "X pixels per meter: " << file.XpixelsPerM << std::endl;
    std::cout << "Y pixels per meter: " << file.YpixelsPerM << std::endl;
    std::cout << "Colors used: " << file.colorsUsed << std::endl;
    std::cout << "Colors important: " << file.colorsImportant << std::endl;
}