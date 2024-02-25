#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "struct.h"

void convert_CBM_to_BMP(const char *cbm_filename, const char *bmp_filename) {
    FILE *cbm_file = fopen(cbm_filename, "rb");
    if (!cbm_file) {
        printf("Error: Unable to open CBM file %s.\n", cbm_filename);
        return;
    }

    CBMHeader cbmHeader;
    fread(&cbmHeader, sizeof(CBMHeader), 1, cbm_file);

    if (cbmHeader.magic != 0x7449) {
        printf("The format is not supported.\n");
        fclose(cbm_file);
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

    FILE *bmp_file = fopen(bmp_filename, "wb");
    if (!bmp_file) {
        printf("Error: Unable to create BMP file %s.\n", bmp_filename);
        fclose(cbm_file);
        return;
    }

    fwrite(&bmpHeader, sizeof(BMPHeader), 1, bmp_file);
    fwrite(&dibHeader, sizeof(DIBHeader), 1, bmp_file);

    RGBColor *palette = malloc(cbmHeader.num_colors * sizeof(RGBColor));
    fseek(cbm_file, cbmHeader.color_array_offset, SEEK_SET);
    for (int i = 0; i < cbmHeader.num_colors; i++) {
        fread(&palette[i], sizeof(RGBColor), 1, cbm_file);
    }
    fseek(cbm_file, cbmHeader.image_array_offset, SEEK_SET);

    for (int row = cbmHeader.height - 1; row >= 0; row--) {
        fseek(cbm_file, cbmHeader.image_array_offset + row * cbmHeader.width, SEEK_SET);
        for (int col = 0; col < cbmHeader.width; col++) {
            uint8_t index;
            fread(&index, sizeof(uint8_t), 1, cbm_file);

            if (index >= cbmHeader.num_colors) {
                printf("Error: Invalid color index detected in CBM file.\n");
                fclose(cbm_file);
                fclose(bmp_file);
                free(palette);
                return;
            }
            RGBColor color = palette[index];
            fwrite(&color.b, sizeof(uint8_t), 1, bmp_file); // Blue
            fwrite(&color.g, sizeof(uint8_t), 1, bmp_file); // Green
            fwrite(&color.r, sizeof(uint8_t), 1, bmp_file); // Red
        }

        for (int p = 0; p < padding; p++) {
            uint8_t pad_byte = 0;
            fwrite(&pad_byte, sizeof(uint8_t), 1, bmp_file);
        }
    }
    free(palette);
    fclose(cbm_file);
    fclose(bmp_file);
    printf("CBM file %s converted to BMP file %s.\n", cbm_filename, bmp_filename);
}
