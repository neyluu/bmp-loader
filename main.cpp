#include <iostream>

#include "bmp/bmp.h"

int main ()
{

    BMP file;

    file.load("../images/4.bmp");
    file.printHeader();
    file.printImage();
}



