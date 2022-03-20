#pragma once
#include <stdint.h>
#include <stdlib.h>

typedef struct
{
    uint8_t red;
    uint8_t green;
    uint8_t blue;
}Pixel;

void writePNG(const char* filename, Pixel* pixels, size_t size);
Pixel* readPNG(const char* filename, size_t* sizeOfImage);
