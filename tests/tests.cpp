#include "tests.h"
#include "../bmp/utils.h"

#include <iostream>

void TEST_calculatePadding()
{
    std::cout << "Testing calculatePadding function..." << std::endl;

    int expectedValues[] = {0, 3, 2, 1, 0,
                            3, 2, 1, 0, 3,
                            2, 1, 0, 3, 2,
                            1, 0
                            };

    for(int i = 0; i < 17; i++)
    {
        int res = calculatePadding(i);
        if(res != expectedValues[i])
        {
            std::cout << "Error, result should be " << expectedValues[i] << ", but function returned " << res << std::endl;
            exit(1);
        }
    }

    std::cout << "Test passed!" << std::endl;
}

void TEST_loadingHelp(std::string filename)
{
    std::cout << "Testing loading file: " << filename << std::endl;

    std::string path = "../tests/test_images/";
    std::string pathBMP = path + filename + ".bmp";

    BMP file;
    int res = file.load(pathBMP);
    if(res != 0)
    {
        std::cout << "Error, loaded failed with error code: " << res << std::endl;
        exit(1);
    }

    std::string pathTXT = path + filename + ".txt";
    FILE *expected = fopen(pathTXT.c_str(), "rt");
    if(expected == nullptr)
    {
        std::cout << "Error, file with expected values not found" << std::endl;
        exit(1);
    }

    int expectedR, expectedG, expectedB;

    for(int i = 0; i < file.headerDIB.height; i++)
    {
        for(int j = 0; j < file.headerDIB.width; j++)
        {
            fscanf(expected, "%d %d %d\n", &expectedR, &expectedG, &expectedB);
            if(expectedR != file.image[i][j].r ||
               expectedG != file.image[i][j].g ||
               expectedB != file.image[i][j].b
               )
            {
                std::cout << "Error, pixel at " << i << ' ' << j
                          << " is: " << int(file.image[i][j].r) << ' '<<  int(file.image[i][j].g) << ' ' << int(file.image[i][j].b)
                          << " but should be: " << expectedR << ' ' << expectedG << ' ' << expectedB << std::endl;
                fclose(expected);
                exit(1);
            }
        }
    }
    fclose(expected);
    std::cout << "Test passed!" << std::endl;
}

void TEST_loading()
{
    TEST_loadingHelp("test_1bit_1");
    TEST_loadingHelp("test_1bit_2");

    TEST_loadingHelp("test_4bit_1");

    TEST_loadingHelp("test_8bit_1");
}



void T_startTests()
{
    std::cout << "--- Unit Tests ---" << std::endl;

    TEST_calculatePadding();
    TEST_loading();

    std::cout << "------------------" << std::endl << std::endl;
}

int T_imageToFile(std::string filename, struct pixel3 **image, int width, int height)
{
    FILE *f = fopen(filename.c_str(), "wt");
    if(f == nullptr) return 1;

    for(int i = 0; i < height; i++)
    {
        if(i == 0)
        {
            for(int j = 0; j < width; j++)
            {
                fprintf(f, "%03d           ", j);
            }
            fprintf(f, "\n");
        }
        for(int j = 0; j < width; j++)
        {
            int res = fprintf(f,"%03d %03d %03d | ", image[i][j].r, image[i][j].g, image[i][j].b);
            if(res < 0)
            {
                fclose(f);
                return 2;
            }
        }
        fprintf(f, "\n");
    }

    fclose(f);
    return 0;
}