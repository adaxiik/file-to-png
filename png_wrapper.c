#include "png_wrapper.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
void writePNG(const char *filename, Pixel *pixels, size_t size)
{
    size_t width = size;
    size_t height = size;
    size_t channels = 3;
    stbi_write_png(filename, width, height, channels, pixels, width * channels);
}

Pixel *readPNG(const char *filename, size_t *sizeOfImage)
{
    int width, height, channels;
    Pixel *pixels = (Pixel *)stbi_load(filename, &width, &height, &channels, 3);
    if (!pixels)
    {
        fprintf(stderr, "Could not load image %s: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (width != height)
    {
        fprintf(stderr, "Image %s is not square\n", filename);
        exit(EXIT_FAILURE);
    }
    *sizeOfImage = width;
    return pixels;
}
