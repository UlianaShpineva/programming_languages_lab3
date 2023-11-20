#include "image.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static struct image rotate(struct image const source) {
    struct image rotated = { .width = source.height, .height = source.width, .data = malloc(source.height * source.width * sizeof(struct pixel)) };
    size_t str_cnt = 0;
    for (size_t i = 0; i < rotated.height; i++) {
        for (size_t j = 0; j < rotated.width; j++) {
            rotated.data[str_cnt + j] = source.data[source.width * (j + 1) - i - 1];
        }
        str_cnt += rotated.width;
    }

    return rotated;
}

struct image image_rotation(struct image source, int angle) {
    size_t cnt;
    switch (angle) {
        case 90:
        case -270:
            cnt = 1;
            break;
        case 180:
        case -180:
            cnt = 2;
            break;
        case 270:
        case -90:
            cnt = 3;
            break;
        case 0:
        default:
            cnt = 0;
            break;
    }
    for (size_t i = 0; i < cnt; i++) {
        struct image tmp_img = rotate(source);
        free(source.data);
        source = tmp_img;
    }
    return source;
}
