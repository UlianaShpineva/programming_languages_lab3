#include "image.h"
#include "bmp.h"
#include <stdbool.h>

bool create_image(struct image* img, uint64_t width, uint64_t height);
void print_read_bmp_status(enum read_status read_status);
void print_write_bmp_status(enum write_status write_status);