#pragma pack(1)
typedef struct {
    uint8_t  magic[2];
    uint32_t file_size;
    uint16_t reserved1, reserved2;
    uint32_t offset;
} BMPHeader;

typedef struct {
    uint32_t size, width, height;
    uint16_t planes, bits_per_pixel;
    uint32_t compression, image_size, x_resolution, y_resolution, n_colors, important_colors;
} DIBHeader;

typedef struct {
    uint8_t r, g, b;
} RGBColor;

typedef struct {
    uint16_t magic;
    uint32_t width;
    uint32_t height;
    uint32_t num_colors;
    uint32_t color_array_offset;
    uint32_t image_array_offset;
} CBMHeader;
#pragma pack(4)
