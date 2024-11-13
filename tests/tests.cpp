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

void startTests()
{
    std::cout << "--- Unit Tests ---" << std::endl;

    TEST_calculatePadding();

    std::cout << "------------------" << std::endl << std::endl;
}