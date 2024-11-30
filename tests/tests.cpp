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

void T_startTests()
{
    std::cout << "--- Unit Tests ---" << std::endl;

    TEST_calculatePadding();

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