#include <cmath>

#include "bmp.h"
#include "utils.h"

int BMP::load(const std::string& filename)
{
    FILE *f = fopen(filename.c_str(), "rb");
    if(f == nullptr) return 1;
    file = f;

    int res;

    res = loadHeaderFile();
    if(res == 1)
    {
        fclose(file);
        return 2;
    }
    res = loadHeaderDIB();
    if(res == 1)
    {
        fclose(file);
        return 3;
    }

    // TODO TMP
    //    if(headerDIB.compression != 0)
    //    {
    //        std::cout << "Used compression: " << headerDIB.compression << std::endl;
    //        fclose(f);
    //        return 69;
    //    }

    image = new pixel4*[headerDIB.height];
    for(int i = 0; i < headerDIB.height; i++)
    {
        image[i] = new pixel4[headerDIB.width];
    }

    switch (headerDIB.bitCount) {
        case 1:
            loadImage1 ();
        break;
        case 2:
            loadImage2 ();
        break;
        case 4:
            loadImage4 ();
        break;
        case 8:
            loadImage8 ();
        break;
        case 16:
            loadImage16 ();
        break;
        case 24:
            loadImage24();
        break;
        case 32:
            loadImage32();
        break;
        default:
            break;
    }

    fclose(file);
    return 0;
}
int BMP::loadHeaderFile()
{
    int res;

    res = fread(headerFile.signature, 1, 2, file);
    if(res != 2) return 1;

    res = fread(&headerFile.fileSize, 4, 1, file);
    if(res != 1) return 1;

    res = fread(headerFile.reserved, 4, 1, file);
    if(res != 1) return 1;

    res = fread(&headerFile.dataOffset, 4, 1, file);
    if(res != 1) return 1;

    return 0;
}
int BMP::loadHeaderDIB()
{
    int res = fread(&headerDIB, sizeof(struct headerDIB), 1, file);
    if(res != 1) return 1;

    const int headerFileSize = 14;
    fseek(file, headerFileSize + headerDIB.size, SEEK_SET);

    return 0;
}

void BMP::loadImage1 ()
{
    readColorTable();
    fseek(file, headerFile.dataOffset, SEEK_SET);

    uint8_t byte;
    int pixels[8];

    int loadedRows = 0;
    int row, col, change;
    getDirection(headerDIB.height, row, change);

    int bytesPerRow;
    int bitRest = headerDIB.width % 8;
    bool aligned; // true - every row need full X bytes, false - from last byte only few bits are needed

    if(bitRest == 0)
    {
        aligned = true;
        bytesPerRow = headerDIB.width / 8;
    }
    else
    {
        aligned = false;
        bytesPerRow = floor(headerDIB.width / 8) + 1;
    }

    while(loadedRows++ < headerDIB.height)
    {
        int pixelsToSet;
        col = 0;

        for(int j = 0; j < bytesPerRow; j++)
        {
            fread(&byte, 1, 1, file);
            splitByte(byte, pixels);

            if(j < bytesPerRow - 1 || aligned) pixelsToSet = 8;
            else pixelsToSet = bitRest;

            for(int k = 0; k < pixelsToSet; k++)
            {
                image[row][col].r = colorTable[pixels[k]].r;
                image[row][col].g = colorTable[pixels[k]].g;
                image[row][col].b = colorTable[pixels[k]].b;
                image[row][col].a = colorTable[pixels[k]].a;

                col++;
            }
        }

        fseek(file, calculatePadding(bytesPerRow), SEEK_CUR);
        row += change;
    }
}
void BMP::loadImage2 ()
{
    readColorTable();
    fseek(file, headerFile.dataOffset, SEEK_SET);

    uint8_t byte;
    int pixel1, pixel2, pixel3, pixel4;
    int loadedBytes;

    int loadedRows = 0;
    int i, change;
    getDirection(headerDIB.height, i, change);

    while(loadedRows++ < headerDIB.height)
    {
        loadedBytes = 0;
        for(int j = 0; j < headerDIB.width; j += 4)
        {
            fread(&byte, 1, 1, file);
            loadedBytes++;

            splitByte(byte, &pixel1, &pixel2, &pixel3, &pixel4);

            image[i][j + 0].r = colorTable[pixel1].r;
            image[i][j + 0].g = colorTable[pixel1].g;
            image[i][j + 0].b = colorTable[pixel1].b;

            image[i][j + 1].r = colorTable[pixel2].r;
            image[i][j + 1].g = colorTable[pixel2].g;
            image[i][j + 1].b = colorTable[pixel2].b;

            image[i][j + 2].r = colorTable[pixel3].r;
            image[i][j + 2].g = colorTable[pixel3].g;
            image[i][j + 2].b = colorTable[pixel3].b;
            
            image[i][j + 3].r = colorTable[pixel4].r;
            image[i][j + 3].g = colorTable[pixel4].g;
            image[i][j + 3].b = colorTable[pixel4].b;
        }

        fseek(file, calculatePadding(loadedBytes), SEEK_CUR);
        i += change;
    }
}
void BMP::loadImage4 ()
{
    readColorTable();
    fseek(file, headerFile.dataOffset, SEEK_SET);

    uint8_t byte = 0;
    int pixel1, pixel2;
    int loadedBytes;

    int loadedRows = 0;
    int i, change;
    getDirection(headerDIB.height, i, change);

    while(loadedRows++ < headerDIB.height)
    {
        loadedBytes = 0;
        for(int j = 0; j < headerDIB.width; j += 2)
        {
            fread(&byte, 1, 1, file);
            loadedBytes++;

            splitByte(byte, &pixel1, &pixel2);

            image[i][j + 0].r = colorTable[pixel1].r;
            image[i][j + 0].g = colorTable[pixel1].g;
            image[i][j + 0].b = colorTable[pixel1].b;
            image[i][j + 0].a = colorTable[pixel1].a;

            image[i][j + 1].r = colorTable[pixel2].r;
            image[i][j + 1].g = colorTable[pixel2].g;
            image[i][j + 1].b = colorTable[pixel2].b;
            image[i][j + 1].a = colorTable[pixel2].a;

            byte = 0;
        }

        fseek(file, calculatePadding(loadedBytes), SEEK_CUR);
        i += change;
    }
}
void BMP::loadImage8 ()
{
    readColorTable();
    fseek(file, headerFile.dataOffset, SEEK_SET);

    uint8_t index;

    int loadedRows = 0;
    int i, change;
    getDirection(headerDIB.height, i, change);

    while(loadedRows++ < headerDIB.height)
    {
        for(int j = 0; j < headerDIB.width; j++)
        {
            fread(&index, 1, 1, file);

            image[i][j].r = colorTable[index].r;
            image[i][j].g = colorTable[index].g;
            image[i][j].b = colorTable[index].b;
            image[i][j].a = colorTable[index].a;
            index = 0;
        }

        fseek(file, calculatePadding(headerDIB.width), SEEK_CUR);
        i += change;
    }
}
void BMP::loadImage16()
{

}
void BMP::loadImage24()
{
    uint8_t r, g, b;

    int loadedRows = 0;
    int i, change;
    getDirection(headerDIB.height, i, change);

    while(loadedRows++ < headerDIB.height)
    {
        for(int j = 0; j < headerDIB.width; j++)
        {
            fread(&b, 1, 1, file);
            fread(&g, 1, 1, file);
            fread(&r, 1, 1, file);
            image[i][j].r = r;
            image[i][j].g = g;
            image[i][j].b = b;
        }

        fseek(file, calculatePadding(headerDIB.width * 3), SEEK_CUR);
        i += change;
    }
}
void BMP::loadImage32()
{
    uint8_t r, g, b, a;

    int loadedRows = 0;
    int i, change;
    getDirection(headerDIB.height, i, change);

    while(loadedRows++ < headerDIB.height)
    {
        for(int j = 0; j < headerDIB.width; j++)
        {
            fread(&b, 1, 1, file);
            fread(&g, 1, 1, file);
            fread(&r, 1, 1, file);
            fread(&a, 1, 1, file);
            image[i][j].r = r;
            image[i][j].g = g;
            image[i][j].b = b;
            image[i][j].a = a;
        }

        fseek(file, calculatePadding(headerDIB.width * 4), SEEK_CUR);
        i += change;
    }
}

void BMP::readColorTable()
{
    unsigned char r, g, b, a;
    int colors = colorTableSize();
    colorTable = new struct pixel4[colors];

    for(int i = 0; i < colors; i++)
    {
        fread(&b, 1, 1, file);
        fread(&g, 1, 1, file);
        fread(&r, 1, 1, file);
        fread(&a, 1, 1, file);
        colorTable[i].r = r;
        colorTable[i].g = g;
        colorTable[i].b = b;
        colorTable[i].a = a;
//        std::cout << i << ". " << int(r) << ' ' << int(g) << ' ' << int(b) << ' ' << int(a) << std::endl;
    }
}

void BMP::getDirection(int height, int& i, int& change)
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