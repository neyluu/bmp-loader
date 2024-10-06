#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <cmath>

#include "bmp.h"

void readBMPfile(struct BMP* fileData, FILE* f)
{
    //Reading File Header
    fread(&fileData->signature, 1, 2, f);
    fread(&fileData->fileSize, 4, 1, f);
    fread(fileData->reserved, 4, 1, f);
    fread(&fileData->dataOffset, 4, 1, f);

    //Reading DIB header
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
            loadImage1(f, fileData);
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

void loadImage1(FILE *f, struct BMP *data)
{

}
void loadImage2(FILE *f, struct BMP *data)
{

}
void loadImage4(FILE *f, struct BMP *data)
{
    unsigned char r, g, b;

    readColorTable(f, data);

    unsigned char byte = 0;
    int pixel1, pixel2;

    fseek(f, data->dataOffset, SEEK_SET);

    int loadedBytes;

    if(data->height < 0)
    {
        for(int i = 0; i < data->height; i++)
        {
            loadedBytes = 0;
            for(int j = 0; j < data->width; j += 2)
            {
                fread(&byte, 1, 1, f);
                loadedBytes++;

                byteTo2Nums(byte, &pixel1, &pixel2);

                data->image[i][j].r = data->colorTable[pixel1].r;
                data->image[i][j].g = data->colorTable[pixel1].g;
                data->image[i][j].b = data->colorTable[pixel1].b;

                data->image[i][j + 1].r = data->colorTable[pixel2].r;
                data->image[i][j + 1].g = data->colorTable[pixel2].g;
                data->image[i][j + 1].b = data->colorTable[pixel2].b;
                byte = 0;
            }

            fseek(f, calculatePadding(loadedBytes), SEEK_CUR);
        }
    }
    else
    {

        for(int i = (data->height - 1); i >= 0; i--)
        {
            loadedBytes = 0;
            for(int j = 0; j < data->width; j += 2)
            {
                fread(&byte, 1, 1, f);
                loadedBytes++;

                byteTo2Nums(byte, &pixel1, &pixel2);

                data->image[i][j].r = data->colorTable[pixel1].r;
                data->image[i][j].g = data->colorTable[pixel1].g;
                data->image[i][j].b = data->colorTable[pixel1].b;

                data->image[i][j + 1].r = data->colorTable[pixel2].r;
                data->image[i][j + 1].g = data->colorTable[pixel2].g;
                data->image[i][j + 1].b = data->colorTable[pixel2].b;
                byte = 0;
            }

            fseek(f, calculatePadding(loadedBytes), SEEK_CUR);
        }
    }
}
void loadImage8(FILE *f, struct BMP *data)
{
    unsigned char r, g, b, a;

    readColorTable(f, data);

    //Reading pixel storage
    unsigned char idx;
    int index;
    fseek(f, data->dataOffset, SEEK_SET);

    if(data->height < 0)
    {
        for(int i = 0; i < data->height; i++)
        {
            for(int j = 0; j < data->width; j++)
            {
                fread(&idx, 1, 1, f);
                index = int(idx);
                data->image[i][j].r = data->colorTable[index].r;
                data->image[i][j].g = data->colorTable[index].g;
                data->image[i][j].b = data->colorTable[index].b;
            }
        }
    }
    else
    {
        for(int i = (data->height - 1); i >= 0; i--)
        {
            for(int j = 0; j < data->width; j++)
            {
                fread(&idx, 1, 1, f);
                index = int(idx);
                data->image[i][j].r = data->colorTable[index].r;
                data->image[i][j].g = data->colorTable[index].g;
                data->image[i][j].b = data->colorTable[index].b;
            }
        }
    }
}
void loadImage16(FILE *f, struct BMP *data)
{

}
void loadImage24(FILE *f, struct BMP *data)
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
void loadImage32(FILE *f, struct BMP *data)
{

}

void readColorTable(FILE *f, struct BMP *data)
{
    unsigned char r, g, b, a;
    int colors;
    if(data->colorsUsed == 0) colors = int(pow(2, data->bitCount));
    else colors = data->colorsUsed;

    data->colorTable = new struct pixel4[colors];

    for(int i = 0; i < colors; i++)
    {
        fread(&b, 1, 1, f);
        fread(&g, 1, 1, f);
        fread(&r, 1, 1, f);
        fread(&a, 1, 1, f);
        data->colorTable[i].r = r;
        data->colorTable[i].g = g;
        data->colorTable[i].b = b;
        data->colorTable[i].a = a;
        std::cout << i << ". " << int(r) << ' ' << int(g) << ' ' << int(b) << ' ' << int(a) << std::endl;
    }
}

void byteTo2Nums(unsigned char byte, int *num1, int *num2)
{
    *num1 = 0;
    int counter = 0;

    for(int i = 4; i <= 7; i++)
    {
        if((byte >> i) & 1)
        {
            *num1 |= (1 << counter);
        }
        counter++;
    }

    *num2 = byte;
    for(int i = 7; i >= 4; i--)
    {
        *num2 &= ~(1 << i);
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

void printBMPfileData(struct BMP file)
{
    std::cout << "--------------------------------------" << std::endl;
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
    std::cout << "--------------------------------------" << std::endl;

    //TODO reading into array based on width -> bottom-up or top-down
    //TODO https://learn.microsoft.com/en-us/windows/win32/api/wingdi/ns-wingdi-bitmapinfoheader
    for(int i = 0; i < abs(file.height); i++)
    {
        for(int j = 0; j < file.width; j++)
        {
//            std::cout << "RGB: ";

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

int calculatePadding(int cols)
{
    int loadedBytes = 3 * abs(cols);
    int padding = 1;

    if(loadedBytes % 4 == 0) return 0;
    else
    {
        while((loadedBytes + padding) % 4 != 0)
        {
            padding++;
        }
    }

    return padding;
}