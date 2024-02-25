#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "struct.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: ./cbm2bmp [OPTION] CBM_FILENAME [BMP_FILENAME]\n");
        return 1;
    }

    if (strcmp(argv[1], "--info") == 0) {
        display_CBM_info(argv[2]);
    } else if (strcmp(argv[1], "--bmp-info") == 0) {
        display_BMP_info(argv[2]);
    } else if (strcmp(argv[1], "--convert") == 0) {
        convert_CBM_to_BMP(argv[2], argv[3]);
    } else {
        fprintf(stderr, "Invalid arguments.\n");
        return 1;
    }

    return 0;
}
