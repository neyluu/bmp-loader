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
        default:
            break;
    }


    fclose(file);
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

int BMP::getColorIndex(struct pixel4 color)
{
    // This method could be inefficient for large files

    for(int i = 0; i < colorTableSize(); i++)
    {
        if(colorTable[i] == color) return i;
    }

    return -1;
}

int BMP::saveImage1()
{

    return 0;
}
int BMP::saveImage8 ()
{
    int res;

    for(int i = 0; i < headerDIB.height; i++)
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

        res = fwrite(0, 1, calculatePadding(savedBytes), file);
        if(res != calculatePadding(savedBytes)) return 1;
    }

    return 0;
}
