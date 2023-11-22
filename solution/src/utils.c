#include "image.h"
#include "bmp.h"
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


bool create_image(struct image* img, uint64_t width, uint64_t height) {
    *img = (struct image) { .width = width, .height = height, .data = malloc(height * width * sizeof(struct pixel)) };
    if (img->data == NULL) {
        return false;
    }
    return true;
}

void print_read_bmp_status(enum read_status read_status) {
    switch (read_status)
    {
    case READ_INVALID_HEADER:
        fprintf(stderr, "Couldn't read header");
        break;
    case READ_INVALID_SIGNATURE:
        fprintf(stderr, "Wrong file format");
        break;
    case READ_INVALID_BITS:
        fprintf(stderr, "Couldn't read data");
        break;
    case READ_OK:
    default:
        fprintf(stdout, "Successfully read file.");
        break;
    }
}

void print_write_bmp_status(enum write_status write_status) {
    switch (write_status)
    {
    case WRITE_ERROR_HEADER:
        fprintf(stderr, "Couldn't write header");
        break;
    case WRITE_ERROR_BITS:
        fprintf(stderr, "Couldn't write data");
        break;
    case WRITE_OK:
    default:
        fprintf(stdout, "Successfully write file.");
        break;
    }
}
