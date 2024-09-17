#include <cstdio>
#include <iostream>
#include <cstdlib>

struct BMPfile {
    //Header
    char signature[3] = {'\0'};
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

    struct pixel3** image;
};

struct pixel3
{
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

void readBMPfile(struct BMPfile* fileData, FILE* f);
void printBMPfileData(struct BMPfile file);
void loadImage(FILE *f, struct pixel3*** image);
void freeImagePixel3(struct pixel3** image, int height);

int calculatePadding(unsigned int cols);

int main ()
{
    BMPfile fileData;

    FILE * file;
    file = fopen ("24.bmp", "rb");
    if (file == nullptr)
    {
        std::cout << "Cannot open file";
        return 1;
    }

    readBMPfile(&fileData, file);
    printBMPfileData(fileData);

    freeImagePixel3(fileData.image, fileData.height);
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

    if(fileData->compression != 0) return;

    fileData->image = new struct pixel3*[fileData->height];
    for(int i = 0; i < fileData->height; i++)
    {
        fileData->image[i] = new struct pixel3;
    }

    std::cout << "pos: " << ftell(f) << std::endl;

    unsigned char r, g, b;
    for(int i = 0; i < fileData->height; i++)
    {
        for(int j = 0; j < fileData->width; j++)
        {
            fread(&b, 1, 1, f);
            fread(&g, 1, 1, f);
            fread(&r, 1, 1, f);
            fileData->image[i][j].r = r;
            fileData->image[i][j].g = g;
            fileData->image[i][j].b = b;

            std::cout << int(r) << ' ' << int(g) << ' ' << int(b) << std::endl;
        }
        std::cout << std::endl;

        int padding = calculatePadding(fileData->width);
        std::cout << padding << std::endl;
        fseek(f, padding, SEEK_CUR);
    }
}

void loadImage(FILE *f, struct pixel3*** image)
{

}

void freeImagePixel3(struct pixel3** image, int height)
{
    for(int i = 0; i < height; i++)
    {
        delete[] image[i];
    }
    delete[] image;
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

    //TODO reading into array based on width -> bottom-up or top-down
    //TODO https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
    for(int i = 0; i < file.height; i++)
    {
        for(int j = 0; j < file.width; j++)
        {
            std::cout << "RGB: ";

            std::cout.width( 3 );
            std::cout << int(file.image[i][j].r) << ' ';

            std::cout.width( 3 );
            std::cout << int(file.image[i][j].g) << ' ';

            std::cout.width( 3 );
            std::cout << int(file.image[i][j].b) << ' ';

            std::cout << "| ";
        }
        std::cout << std::endl;
    }
}

int calculatePadding(unsigned int cols)
{
    int readedBytes = 3 * cols;
    int padding = 1;

    if(readedBytes % 4 == 0) return 0;
    else
    {
        while((readedBytes + padding) % 4 != 0)
        {
            padding++;
        }
    }

    return padding;
}

