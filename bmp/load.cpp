#include "load.h"
#include "utils.h"

void loadImage1 (FILE *f, BMP& data)
{
    readColorTable(f, data);
    fseek(f, data.dataOffset, SEEK_SET);

    uint8_t byte;
    int pixels[8];

    int loadedRows = 0;
    int row, col, change;
    getDirection(data.height, row, change);

    int bytesPerRow;
    int bitRest = data.width % 8;
    bool aligned; // true - every row need full X bytes, false - from last byte only few bits are needed

    if(bitRest == 0)
    {
        aligned = true;
        bytesPerRow = data.width / 8;
    }
    else
    {
        aligned = false;
        bytesPerRow = floor(data.width / 8) + 1;
    }

    while(loadedRows++ < data.height)
    {
        int pixelsToSet;
        col = 0;

        for(int j = 0; j < bytesPerRow; j++)
        {
            std::cout << "pos: " << ftell(f) << std::endl;
            fread(&byte, 1, 1, f);
            splitByte(byte, pixels);

            if(j < bytesPerRow - 1 || aligned) pixelsToSet = 8;
            else pixelsToSet = bitRest;

            for(int k = 0; k < pixelsToSet; k++)
            {
                data.image[row][col].r = data.colorTable[pixels[k]].r;
                data.image[row][col].g = data.colorTable[pixels[k]].g;
                data.image[row][col].b = data.colorTable[pixels[k]].b;
                col++;
            }
        }

        fseek(f, calculatePadding(bytesPerRow), SEEK_CUR);
        row += change;
    }
}
void loadImage2 (FILE *f, BMP& data)
{
    readColorTable(f, data);
    fseek(f, data.dataOffset, SEEK_SET);

    uint8_t byte;
    int pixel1, pixel2, pixel3, pixel4;
    int loadedBytes;

    int loadedRows = 0;
    int i, change;
    getDirection(data.height, i, change);

    while(loadedRows++ < data.height)
    {
        loadedBytes = 0;
        for(int j = 0; j < data.width; j += 4)
        {
            fread(&byte, 1, 1, f);
            loadedBytes++;

            splitByte(byte, &pixel1, &pixel2, &pixel3, &pixel4);

            data.image[i][j + 0].r = data.colorTable[pixel1].r;
            data.image[i][j + 0].g = data.colorTable[pixel1].g;
            data.image[i][j + 0].b = data.colorTable[pixel1].b;

            data.image[i][j + 1].r = data.colorTable[pixel2].r;
            data.image[i][j + 1].g = data.colorTable[pixel2].g;
            data.image[i][j + 1].b = data.colorTable[pixel2].b;

            data.image[i][j + 2].r = data.colorTable[pixel3].r;
            data.image[i][j + 2].g = data.colorTable[pixel3].g;
            data.image[i][j + 2].b = data.colorTable[pixel3].b;
            
            data.image[i][j + 3].r = data.colorTable[pixel4].r;
            data.image[i][j + 3].g = data.colorTable[pixel4].g;
            data.image[i][j + 3].b = data.colorTable[pixel4].b;
        }

        fseek(f, calculatePadding(loadedBytes), SEEK_CUR);
        i += change;
    }
}
void loadImage4 (FILE *f, BMP& data)
{
    readColorTable(f, data);
    fseek(f, data.dataOffset, SEEK_SET);

    uint8_t byte = 0;
    int pixel1, pixel2;
    int loadedBytes;

    int loadedRows = 0;
    int i, change;
    getDirection(data.height, i, change);

    while(loadedRows++ < data.height)
    {
        loadedBytes = 0;
        for(int j = 0; j < data.width; j += 2)
        {
            fread(&byte, 1, 1, f);
            loadedBytes++;

            splitByte(byte, &pixel1, &pixel2);

            data.image[i][j + 0].r = data.colorTable[pixel1].r;
            data.image[i][j + 0].g = data.colorTable[pixel1].g;
            data.image[i][j + 0].b = data.colorTable[pixel1].b;

            data.image[i][j + 1].r = data.colorTable[pixel2].r;
            data.image[i][j + 1].g = data.colorTable[pixel2].g;
            data.image[i][j + 1].b = data.colorTable[pixel2].b;
            byte = 0;
        }

        fseek(f, calculatePadding(loadedBytes), SEEK_CUR);
        i += change;
    }
}
void loadImage8 (FILE *f, BMP& data)
{
    readColorTable(f, data);
    fseek(f, data.dataOffset, SEEK_SET);

    uint8_t index;

    int loadedRows = 0;
    int i, change;
    getDirection(data.height, i, change);

    while(loadedRows++ < data.height)
    {
        for(int j = 0; j < data.width; j++)
        {
            fread(&index, 1, 1, f);
            data.image[i][j].r = data.colorTable[index].r;
            data.image[i][j].g = data.colorTable[index].g;
            data.image[i][j].b = data.colorTable[index].b;
            index = 0;
        }

        i += change;
    }
}
void loadImage16(FILE *f, BMP& data)
{

}
void loadImage24(FILE *f, BMP& data)
{
    uint8_t r, g, b;

    int loadedRows = 0;
    int i, change;
    getDirection(data.height, i, change);

    while(loadedRows++ < data.height)
    {
        for(int j = 0; j < data.width; j++)
        {
            fread(&b, 1, 1, f);
            fread(&g, 1, 1, f);
            fread(&r, 1, 1, f);
            data.image[i][j].r = r;
            data.image[i][j].g = g;
            data.image[i][j].b = b;
        }

        fseek(f, calculatePadding(data.width * 3), SEEK_CUR);
        i += change;
    }
}
void loadImage32(FILE *f, BMP& data)
{

}

void readColorTable(FILE *f, BMP& data)
{
    unsigned char r, g, b, a;
    int colors;
    if(data.colorsUsed == 0) colors = int(pow(2, data.bitCount));
    else colors = data.colorsUsed;

    data.colorTable = new struct pixel4[colors];

    for(int i = 0; i < colors; i++)
    {
        fread(&b, 1, 1, f);
        fread(&g, 1, 1, f);
        fread(&r, 1, 1, f);
        fread(&a, 1, 1, f);
        data.colorTable[i].r = r;
        data.colorTable[i].g = g;
        data.colorTable[i].b = b;
        data.colorTable[i].a = a;
        std::cout << i << ". " << int(r) << ' ' << int(g) << ' ' << int(b) << ' ' << int(a) << std::endl;
    }
}

void getDirection(int height, int& i, int& change)
{
    if(height < 0)
    {
        i = 0;
        change = 1;
    }
    else
    {
        i = height - 1;
        change = -1;
    }
}