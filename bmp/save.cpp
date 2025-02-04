#include <math.h>

#include "bmp.h"
#include "utils.h"

int BMP::save(std::string filename)
{
    if(!filename.ends_with(".bmp"))
    {
        filename += ".bmp";
    }

    FILE *f = fopen(filename.c_str(), "wb");
    if(f == nullptr) return 1;
    file = f;

    prepareHeaders();

    int res;

    res = saveHeaderFile();
    if(res == 1)
    {
        fclose(file);
        return 2;
    }

    res = saveHeaderDIB();
    if(res == 1)
    {
        fclose(file);
        return 3;
    }
    switch(headerDIB.bitCount)
    {
        case 1:
            saveColorTable();
            saveImage1();
            break;
        case 8:
            saveColorTable();
            saveImage8();
            break;
        case 24:
            res = saveImage24();
            break;
        case 32:
            res = saveImage32();
            break;
        default:
            break;
    }
    if(res == 1) return 4;

    res = updateFileSize();
    if(res == 1) return 4;

    return 0;
}
int BMP::saveHeaderFile()
{
    int res;
    res = fwrite(headerFile.signature, 1, 2, file);
    if(res != 2) return 1;

    res = fwrite(&headerFile.fileSize, sizeof(int32_t), 1, file);
    if(res != 1) return 1;

    res = fwrite(&headerFile.reserved, sizeof(uint8_t), 4, file);
    if(res != 4) return 1;

    res = fwrite(&headerFile.dataOffset, sizeof(int32_t), 1, file);
    if(res != 1) return 1;

    return 0;
}
int BMP::saveHeaderDIB()
{
    int res = fwrite(&headerDIB, sizeof(struct headerDIB), 1, file);
    if(res != 1) return 1;
    return 0;
}

int BMP::saveColorTable()
{
    int colors = colorTableSize();
    int res;

    for(int i = 0; i < colors; i++)
    {
        res = fwrite(&colorTable[i].b, 1, 1, file);
        if(res != 1) return 1;
        res = fwrite(&colorTable[i].g, 1, 1, file);
        if(res != 1) return 1;
        res = fwrite(&colorTable[i].r, 1, 1, file);
        if(res != 1) return 1;
        res = fwrite(&colorTable[i].a, 1, 1, file);
        if(res != 1) return 1;
    }

    return 0;
}

int BMP::updateFileSize()
{
    size_t newSize = ftell(file);
    if(newSize == headerFile.fileSize) return 0;

    fseek(file, 2, SEEK_SET);
    int res = fwrite(&newSize, sizeof(size_t), 1, file);

    if(res != 1) return 1;
    return 0;
}

void BMP::prepareHeaders()
{
    // headerFile.fileSize =
    headerFile.dataOffset = 54;
    if(headerDIB.bitCount <= 8)
    {
        int colorTableSize = BMP::colorTableSize() * 4;
        headerFile.dataOffset += colorTableSize;
    }
    else if(headerDIB.bitCount == 32)
    {
        // TODO temporary solution when lib dont support compressions, can cause problems in future
        headerDIB.compression = 0;
    }

    headerDIB.size = 40;
    headerDIB.height = abs(headerDIB.height);
}

int BMP::getColorIndex(struct pixel4 color)
{
    // This method could be inefficient for large files

    for(int i = 0; i < colorTableSize(); i++)
    {
        if(colorTable[i] == color) return i;
    }

    return -1;
}

uint8_t BMP::joinByte(int row, int col, int pixels)
{
    uint8_t byte = 0;

    int bits = col + 8 > headerDIB.width ? headerDIB.width - col : 8;

    for(int i = 0; i < bits; i++)
    {
        int bitValue = getColorIndex(image[row][col + i]);
        if(bitValue) byte |= (1 << (7 - i));
        else byte &= ~(1 << i);
    }

    for(int i = 7; i >= 0; i--)
        std::cout << ((byte >> i) & 1);
    std::cout << std::endl;


    return byte;
}

int BMP::saveImage1()
{
    int res;

    for(int i = headerDIB.height - 1; i >= 0; i--)
    {
        int savedBytes = 0;
        for(int j = 0; j < headerDIB.width; j += 8)
        {
            uint8_t byte = joinByte(i, j, 8);
            res = fwrite(&byte, sizeof(uint8_t), 1, file);
            if(res != 1) return 1;

            savedBytes++;
        }
        std::cout << std::endl;

        uint32_t dummy = 0;
        const int padding = calculatePadding(savedBytes);
        res = fwrite(&dummy, 1, padding, file);
        if(res != padding) return 1;
    }
    return 0;
}
int BMP::saveImage2()
{

    return 0;
}
int BMP::saveImage4()
{

    return 0;
}
int BMP::saveImage8 ()
{
    int res;

    for(int i = headerDIB.height - 1; i >= 0; i--)
    {
        int savedBytes = 0;
        for(int j = 0; j < headerDIB.width; j++)
        {
            int index = getColorIndex(image[i][j]);
            if(index == -1)
            {
                std::cout << "test";
                return 1;
            }

            uint8_t indexToSave = (uint8_t) index;

            res = fwrite(&indexToSave, sizeof(uint8_t), 1, file);
            if(res != 1) return 1;

            savedBytes++;
        }

        const int padding = calculatePadding(savedBytes);
        int dummy = 0;
        res = fwrite(&dummy, 1, padding, file);
        if(res != padding) return 1;
    }

    return 0;
}
int BMP::saveImage16()
{

    return 0;
}
int BMP::saveImage24()
{
    std::cout << "pos: " << ftell(file) << std::endl;

    int res;
    for(int i = headerDIB.height - 1; i >= 0; i--)
    {
        for(int j = 0; j < headerDIB.width; j++)
        {
            res = fwrite(&image[i][j].b, 1, 1, file);
            if(res != 1) return 1;
            res = fwrite(&image[i][j].g, 1, 1, file);
            if(res != 1) return 1;
            res = fwrite(&image[i][j].r, 1, 1, file);
            if(res != 1) return 1;
        }

        int padding = calculatePadding(headerDIB.width * 3);
        res = fwrite(0, 1, padding, file);
        if(res != padding) return 1;
    }
    return 0;
}
int BMP::saveImage32()
{
    std::cout << "pos: " << ftell(file) << std::endl;
    int res;
    for(int i = headerDIB.height - 1; i >= 0; i--)
    {
        for(int j = 0; j < headerDIB.width; j++)
        {
            res = fwrite(&image[i][j].b, 1, 1, file);
            if(res != 1) return 1;
            res = fwrite(&image[i][j].g, 1, 1, file);
            if(res != 1) return 1;
            res = fwrite(&image[i][j].r, 1, 1, file);
            if(res != 1) return 1;
            res = fwrite(&image[i][j].a, 1, 1, file);
            if(res != 1) return 1;
        }

        int padding = calculatePadding(headerDIB.width * 4);
        res = fwrite(0, 1, padding, file);
        if(res != padding) return 1;
    }
    
    return 0;
}
