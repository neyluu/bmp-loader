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
void freeImagePixel3(struct pixel3** image, int height);

int calculatePadding(unsigned int cols);
void loadImage1 (FILE *f, struct BMPfile *data);
void loadImage2 (FILE *f, struct BMPfile *data);
void loadImage4 (FILE *f, struct BMPfile *data);
void loadImage8 (FILE *f, struct BMPfile *data);
void loadImage16(FILE *f, struct BMPfile *data);
void loadImage24(FILE *f, struct BMPfile *data);

int main ()
{
    BMPfile fileData;

    FILE * file;
    file = fopen("24.bmp", "rb");
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

    switch (fileData->bitCount) {
        case 1:
            loadImage1(f, nullptr);
            break;
        case 2:
            loadImage2(f, fileData);
            break;
        case 4:
            loadImage4(f, fileData);
            break;
        case 8:
            loadImage8(f, fileData);
            break;
        case 16:
            loadImage16(f, fileData);
            break;
        case 24:
            loadImage24(f, fileData);
            break;
        default:
            break;
    }
}

void loadImage1(FILE *f, struct BMPfile *data)
{
    
}
void loadImage2(FILE *f, struct BMPfile *data)
{
    
}
void loadImage4(FILE *f, struct BMPfile *data)
{
    
}
void loadImage8(FILE *f, struct BMPfile *data)
{
    unsigned char r, g, b;

    for(int i = 0; i < data->height; i++)
    {
        for(int j = 0; j < data->width; j++)
        {
            fread(&b, 1, 1, f);
            fread(&g, 1, 1, f);
            fread(&r, 1, 1, f);
            fseek(f, 1, SEEK_CUR);
            data->image[i][j].r = r;
            data->image[i][j].g = g;
            data->image[i][j].b = b;
        }
    }
}
void loadImage16(FILE *f, struct BMPfile *data)
{
    
}
void loadImage24(FILE *f, struct BMPfile *data)
{
    unsigned char r, g, b;

    if(data->height < 0)
    {
        for(int i = 0; i < data->width; i++)
        {
            for(int j = 0; j < data->width; j++)
            {
                fread(&b, 1, 1, f);
                fread(&g, 1, 1, f);
                fread(&r, 1, 1, f);
                data->image[i][j].r = r;
                data->image[i][j].g = g;
                data->image[i][j].b = b;
            }

            fseek(f, calculatePadding(data->width), SEEK_CUR);
        }
    }
    else
    {
        for(int i = (data->height - 1); i >= 0; i--)
        {
            for(int j = 0; j < data->width; j++)
            {
                fread(&b, 1, 1, f);
                fread(&g, 1, 1, f);
                fread(&r, 1, 1, f);
                data->image[i][j].r = r;
                data->image[i][j].g = g;
                data->image[i][j].b = b;
            }

            fseek(f, calculatePadding(data->width), SEEK_CUR);
        }
    }
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
    for(int i = 0; i < abs(file.height); i++)
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

