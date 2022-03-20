#include "png_wrapper.h"
#include <png.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
void writePNG(const char* filename, Pixel* pixels, size_t size)
{
    // http://zarb.org/~gc/html/libpng.html
    FILE* fp = fopen(filename, "wb");
    if (!fp)
    {
        fprintf(stderr, "Could not open file %s for writing: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        fprintf(stderr, "Could not allocate write struct\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        fprintf(stderr, "Could not allocate info struct\n");
        fclose(fp);
        png_destroy_write_struct(&png, NULL);
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png)))
    {
        fprintf(stderr, "Error during png creation\n");
        fclose(fp);
        png_destroy_write_struct(&png, &info);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, size, size, 8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    //png_set_compression_level(png, 9);
    png_write_info(png, info);
    png_byte* row = (png_byte*)malloc(3 * size * sizeof(png_byte));
    if (!row)
    {
        fprintf(stderr, "Could not allocate row\n");
        fclose(fp);
        png_destroy_write_struct(&png, &info);
        return;
    }
    for (int y = 0; y < size; y++)
    {
        for (int x = 0; x < size; x++)
        {
            row[3 * x + 0] = pixels[y * size + x].red;
            row[3 * x + 1] = pixels[y * size + x].green;
            row[3 * x + 2] = pixels[y * size + x].blue;
        }
        png_write_row(png, row);
    }
    free(row);
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    printf("Image %s successfully created\n", filename);
}

Pixel* readPNG(const char* filename, size_t* sizeOfImage){
    // http://zarb.org/~gc/html/libpng.html
    FILE* fp = fopen(filename, "rb");
    if (!fp)
    {
        fprintf(stderr, "Could not open file %s for reading: %s\n", filename, strerror(errno));
        exit(EXIT_FAILURE);
    }

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        fprintf(stderr, "Could not allocate read struct\n");
        fclose(fp);
        exit(EXIT_FAILURE);
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        fprintf(stderr, "Could not allocate info struct\n");
        fclose(fp);
        png_destroy_read_struct(&png, NULL, NULL);
        exit(EXIT_FAILURE);
    }

    if (setjmp(png_jmpbuf(png)))
    {
        fprintf(stderr, "Error during png creation\n");
        fclose(fp);
        png_destroy_read_struct(&png, &info, NULL);
        exit(EXIT_FAILURE);
    }

    png_init_io(png, fp);
    png_read_info(png, info);
    png_uint_32 width, height;
    int bit_depth, color_type, interlace_type;
    png_get_IHDR(png, info, &width, &height, &bit_depth, &color_type, &interlace_type, NULL, NULL);
    *sizeOfImage = width;
    
    Pixel* pixels = malloc(width * height * sizeof(Pixel));
    png_read_update_info(png, info);
    png_byte* row = (png_byte*)malloc(3 * width * sizeof(png_byte));
    if (!row)
    {
        fprintf(stderr, "Could not allocate row\n");
        fclose(fp);
        png_destroy_read_struct(&png, &info, NULL);
        exit(EXIT_FAILURE);
    }
    for (int y = 0; y < height; y++)
    {
        png_read_row(png, row, NULL);
        for (int x = 0; x < width; x++)
        {
            pixels[y * width + x].red = row[3 * x + 0];
            pixels[y * width + x].green = row[3 * x + 1];
            pixels[y * width + x].blue = row[3 * x + 2];
        }
    }
    free(row);
    png_read_end(png, NULL);
    png_destroy_read_struct(&png, &info, NULL);
    fclose(fp);
    return pixels;
}
