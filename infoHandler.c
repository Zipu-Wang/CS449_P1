#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

void display_CBM_info(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error: Unable to open file %s.\n", filename);
        return;
    }

    CBMHeader cbmHeader;
    fread(&cbmHeader, sizeof(CBMHeader), 1, file);

    if (cbmHeader.magic != 0x7449) {
        printf("The format is not supported.\n");
        fclose(file);
        return;
    }

    printf("=== CBM Header ===\n");
    printf("Magic: 0x%x\n", cbmHeader.magic);
    printf("Width: %u\n", cbmHeader.width);
    printf("Height: %u\n", cbmHeader.height);
    printf("Number of colors: %u\n", cbmHeader.num_colors);
    printf("Color array offset: %u\n", cbmHeader.color_array_offset);
    printf("Image array offset: %u\n", cbmHeader.image_array_offset);
    printf("\n");

    fseek(file, cbmHeader.color_array_offset, SEEK_SET);
    printf("=== Palette (R, G, B) ===\n");
    for (int i = 0; i < cbmHeader.num_colors; i++) {
        RGBColor color;
        fread(&color, sizeof(RGBColor), 1, file);
        printf("Color %d: (%u, %u, %u)\n", i, color.r, color.g, color.b);
    }

    fclose(file);
}

void display_BMP_info(const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (!file) {
        printf("Error: Unable to open file %s.\n", filename);
        return;
    }

    CBMHeader cbmHeader;
    fread(&cbmHeader, sizeof(CBMHeader), 1, file);

    if (cbmHeader.magic != 0x7449) {
        printf("The format is not supported.\n");
        fclose(file);
        return;
    }

    uint32_t row_size = (cbmHeader.width * 3);
    uint32_t padding = (4 - (row_size % 4)) % 4;

    BMPHeader bmpHeader;
    bmpHeader.magic[0] = 'B';
    bmpHeader.magic[1] = 'M';
    bmpHeader.file_size = 54 + (row_size + padding) * cbmHeader.height;
    bmpHeader.reserved1 = 0;
    bmpHeader.reserved2 = 0;
    bmpHeader.offset = 54;

    DIBHeader dibHeader;
    dibHeader.size = 40;
    dibHeader.width = cbmHeader.width;
    dibHeader.height = cbmHeader.height;
    dibHeader.planes = 1;
    dibHeader.bits_per_pixel = 24;
    dibHeader.compression = 0;
    dibHeader.image_size = 0;
    dibHeader.x_resolution = 0;
    dibHeader.y_resolution = 0;
    dibHeader.n_colors = 0;
    dibHeader.important_colors = 0;

    printf("=== BMP Header ===\n");
    printf("Type: %c%c\n", bmpHeader.magic[0], bmpHeader.magic[1]);
    printf("Size: %u\n", bmpHeader.file_size);
    printf("Reserved 1: %u\n", bmpHeader.reserved1);
    printf("Reserved 2: %u\n", bmpHeader.reserved2);
    printf("Image offset: %u\n", bmpHeader.offset);
    printf("\n");

    printf("=== DIB Header ===\n");
    printf("Size: %u\n", dibHeader.size);
    printf("Width: %u\n", dibHeader.width);
    printf("Height: %u\n", dibHeader.height);
    printf("# color planes: %u\n", dibHeader.planes);
    printf("# bits per pixel: %u\n", dibHeader.bits_per_pixel);
    printf("Compression scheme: %u\n", dibHeader.compression);
    printf("Image size: %u\n", dibHeader.image_size);
    printf("Horizontal resolution: %u\n", dibHeader.x_resolution);
    printf("Vertical resolution: %u\n", dibHeader.y_resolution);
    printf("# colors in palette: %u\n", dibHeader.n_colors);
    printf("# important colors: %u\n", dibHeader.important_colors);
    printf("\n");

    fclose(file);
}


