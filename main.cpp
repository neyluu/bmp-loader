#include <iostream>

#include "bmp/bmp.h"
#include "tests/tests.h"

int main ()
{
    T_startTests();

    BMP file;


    // int res = file.load("../images/32_2.bmp");
//    int res = file.load("../tests/test_images/test_24bit_1.bmp");
    int res = file.load("../images/8.bmp");
    if(res != 0)
    {
        std::cout << "Loading failed! " << res;
        return 1;
    }

    file.printHeader();
    file.printColorTable();
    file.printImage();

    // T_imageToFile("../images/test_output.txt", file.image, file.headerDIB.width, file.headerDIB.height);

    std::cout << file.save("test");
}



