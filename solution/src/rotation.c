#include "image.h"
#include "utils.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static struct image rotate(struct image const source) {
    struct image rotated = { 0 };
    bool img_b = create_image(&rotated, source.height, source.width);
    if (img_b && source.data) {
        for (size_t i = 0; i < rotated.height; i++) {
            for (size_t j = 0; j < rotated.width; j++) {
                rotated.data[rotated.width * i + j] = source.data[source.width * (j + 1) - i - 1];
            }
        }
    }
    return rotated;
}

struct image image_rotation(struct image source, int angle) {
    struct image rotated = { 0 };
    bool img_b = create_image(&rotated, source.width, source.height);
    if (img_b) {
        for (size_t i = 0; i < rotated.height * rotated.width; i++) {
            rotated.data[i] = source.data[i];
        }
        size_t cnt = angle < 0 ? angle / 90 + 4 : angle / 90;
        for (size_t i = 0; i < cnt; i++) {
            struct image tmp_img = rotate(rotated);
            free(rotated.data);
            rotated = tmp_img;
        }
    }
    return rotated;
}

