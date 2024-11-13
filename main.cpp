#include <iostream>

#include "bmp/bmp.h"
#include "tests/tests.h"

int main ()
{
    startTests();


    BMP file;

    file.load("../images/1_20x20.bmp");

    file.printHeader();
    file.printImage();

}



