#include "image.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

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

struct image image_rotation(struct image const source, int angle) {
    struct image rotated = { .width = source.width, .height = source.height, .data = malloc(source.height * source.width * sizeof(struct pixel))};

    for (size_t i = 0; i < source.width * source.height; ++i) {
        rotated.data[i] = source.data[i];
    }
    int cnt = 0;
    //printf("angle %d \n\n", angle);
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
        default:
            cnt = 0;
            return source;
            break;
    }
    for (size_t i = 0; i < cnt; i++) {
        struct image tmp_img = rotate(rotated);
        if ((&rotated)->data) {
            free((&rotated)->data);
        }
        rotated = tmp_img;
    }
    return rotated;
}