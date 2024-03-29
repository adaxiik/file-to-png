#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <math.h>

#include "png_wrapper.h"

typedef struct
{
    size_t original_size;
    size_t imageDimension;
    char *buffer;
} File;

void fillEmptyBytes(File *input)
{
    // size of pixels needed
    size_t realSize = (input->original_size / sizeof(Pixel)) + (input->original_size % sizeof(Pixel)) + sizeof(size_t) + 8; // data, size, and eight extra to be sure

    // dimensions of image
    input->imageDimension = sqrt(realSize) + 1; //+1 because of integers
    size_t size = input->imageDimension * input->imageDimension * sizeof(Pixel);
    input->buffer = realloc(input->buffer, size);

    // fill empty bytes
    //+8 offset because of size_t on the beginning
    for (size_t i = input->original_size + sizeof(size_t); i < size; i++)
        input->buffer[i] = (char)255;
}

File loadFile(const char *filename)
{
    FILE *f = fopen(filename, "rb");
    File output;
    if (!f)
    {
        fprintf(stderr, "Could not open file %s for reading: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    // get file size
    fseek(f, 0, SEEK_END);
    output.original_size = ftell(f);
    rewind(f);

    output.buffer = malloc(output.original_size + sizeof(size_t)); // well, not suitable for large files, but..
    if (!output.buffer)
    {
        fprintf(stderr, "Could not allocate %lu bytes\n", output.original_size);
        fclose(f);
        exit(EXIT_FAILURE);
    }
    // preappend size
    size_t *s = (size_t *)output.buffer;
    *s = output.original_size;

    // read file
    fread(output.buffer + sizeof(size_t), output.original_size, sizeof(char), f);
    fclose(f);

    fillEmptyBytes(&output);

    return output;
}

void saveFile(const char *filename, char *buffer)
{
    size_t fileSize = *(size_t *)buffer;

    printf("Reading %lu bytes\n", fileSize);

    FILE *f = fopen(filename, "wb");
    if (!f)
    {
        fprintf(stderr, "Could not open file %s for writing: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }
    fwrite(buffer + sizeof(size_t), sizeof(char), fileSize, f);
    fclose(f);
}

int main(int argc, char const *argv[])
{

    if (argc != 4)
        goto usage;

    if (!strcmp(argv[1], "-t"))
    {
        printf("Converting to .png\n");

        File file = loadFile(argv[2]);
        printf("Writing %lu bytes to image %lu x %lu\n", file.original_size, file.imageDimension, file.imageDimension);
        writePNG(argv[3], (Pixel *)file.buffer, file.imageDimension);
        free(file.buffer);
    }
    else if (!strcmp(argv[1], "-f"))
    {
        printf("Converting from .png\n");

        size_t size;
        Pixel *pixels = readPNG(argv[2], &size);
        printf("Read %lu x %lu image\n", size, size);
        saveFile(argv[3], (char *)pixels);
        free(pixels);
    }
    else
        goto usage;

    return 0;

usage:
    fprintf(stderr, "Usage: %s <mode> <input> <output>\n", argv[0]);
    fprintf(stderr, "<mode> can be -t (to .png) or -f (from .png)\n");
    exit(EXIT_FAILURE);

    return 0;
}
