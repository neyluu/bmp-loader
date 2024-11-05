#include "load.h"
#include "utils.h"

void loadImage1 (FILE *f, class BMP& data)
{

}
void loadImage2 (FILE *f, class BMP& data)
{

}
void loadImage4 (FILE *f, class BMP& data)
{
    unsigned char r, g, b;
    readColorTable(f, data);

    unsigned char byte = 0;
    int pixel1, pixel2;

    fseek(f, data.dataOffset, SEEK_SET);

    int loadedBytes;

    if(data.height < 0)
    {
        for(int i = 0; i < data.height; i++)
        {
            loadedBytes = 0;
            for(int j = 0; j < data.width; j += 2)
            {
                fread(&byte, 1, 1, f);
                loadedBytes++;

                splitByte(byte, &pixel1, &pixel2);

                data.image[i][j].r = data.colorTable[pixel1].r;
                data.image[i][j].g = data.colorTable[pixel1].g;
                data.image[i][j].b = data.colorTable[pixel1].b;

                data.image[i][j + 1].r = data.colorTable[pixel2].r;
                data.image[i][j + 1].g = data.colorTable[pixel2].g;
                data.image[i][j + 1].b = data.colorTable[pixel2].b;
                byte = 0;
            }

            fseek(f, calculatePadding(loadedBytes), SEEK_CUR);
        }
    }
    else
    {

        for(int i = (data.height - 1); i >= 0; i--)
        {
            loadedBytes = 0;
            for(int j = 0; j < data.width; j += 2)
            {
                fread(&byte, 1, 1, f);
                loadedBytes++;

                splitByte(byte, &pixel1, &pixel2);

                data.image[i][j].r = data.colorTable[pixel1].r;
                data.image[i][j].g = data.colorTable[pixel1].g;
                data.image[i][j].b = data.colorTable[pixel1].b;

                data.image[i][j + 1].r = data.colorTable[pixel2].r;
                data.image[i][j + 1].g = data.colorTable[pixel2].g;
                data.image[i][j + 1].b = data.colorTable[pixel2].b;
                byte = 0;
            }

            fseek(f, calculatePadding(loadedBytes), SEEK_CUR);
        }
    }
}
void loadImage8 (FILE *f, class BMP& data)
{
    unsigned char r, g, b, a;

    readColorTable(f, data);

    //Reading pixel storage
    unsigned char idx;
    int index;
    fseek(f, data.dataOffset, SEEK_SET);

    if(data.height < 0)
    {
        for(int i = 0; i < data.height; i++)
        {
            for(int j = 0; j < data.width; j++)
            {
                fread(&idx, 1, 1, f);
                index = int(idx);
                data.image[i][j].r = data.colorTable[index].r;
                data.image[i][j].g = data.colorTable[index].g;
                data.image[i][j].b = data.colorTable[index].b;
            }
        }
    }
    else
    {
        for(int i = (data.height - 1); i >= 0; i--)
        {
            for(int j = 0; j < data.width; j++)
            {
                fread(&idx, 1, 1, f);
                index = int(idx);
                data.image[i][j].r = data.colorTable[index].r;
                data.image[i][j].g = data.colorTable[index].g;
                data.image[i][j].b = data.colorTable[index].b;
            }
        }
    }
}
void loadImage16(FILE *f, class BMP& data)
{

}
void loadImage24(FILE *f, class BMP& data)
{
    unsigned char r, g, b;

    if(data.height < 0)
    {
        for(int i = 0; i < data.width; i++)
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

            fseek(f, calculatePadding(data.width), SEEK_CUR);
        }
    }
    else
    {
        for(int i = (data.height - 1); i >= 0; i--)
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

            fseek(f, calculatePadding(data.width), SEEK_CUR);
        }
    }
}
void loadImage32(FILE *f, class BMP& data)
{

}

void readColorTable(FILE *f, class BMP& data)
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