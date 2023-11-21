#include "image.h"
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

static struct image rotate(struct image const source) {
    struct image rotated = { 0 };
    
    rotated.width = source.height;
    rotated.height = source.width;
    rotated.data = malloc(source.height * source.width * sizeof(struct pixel));
    if (rotated.data != NULL && source.data) {
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
    rotated.width = source.width;
    rotated.height = source.height;
    rotated.data = malloc(source.height * source.width * sizeof(struct pixel));
    for (size_t i = 0; i < rotated.height * rotated.width; i++) {
        rotated.data[i] = source.data[i];
    }
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
        struct image tmp_img = rotate(rotated);
        //free(rotated.data);
        rotated = tmp_img;
    }
    return rotated;
}
