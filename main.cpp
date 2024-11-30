#include <iostream>

#include "bmp/bmp.h"
#include "tests/tests.h"

int main ()
{
    T_startTests();

    BMP file;

    int res = file.load("../images/1_20x20.bmp");
//    int res = file.load("../images/1.bmp");
    if(res != 0)
    {
        std::cout << "Loading failed!";
        return 1;
    }

    file.printHeader();
    file.printImage();

    T_imageToFile("../images/test_output.txt", file.image, file.width, file.height);

}



