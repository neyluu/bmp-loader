#include <iostream>

#include "bmp/bmp.h"

int main ()
{

    BMP file;

    file.load("../images/8.bmp");

    file.printHeader();
    file.printImage();

}



