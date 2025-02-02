#include "bmp.h"
#include "image.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

#define H_TYPE 0x4D42
#define H_RESERVED 0
#define H_SIZE 40
#define H_PLANES 1
#define H_BIT_COUNT 24
#define H_COMPRESSION 0
#define H_X_PELS_PER_METER 2834
#define H_Y_PELS_PER_METER 2834
#define H_CRL_USED 0
#define H_CRL_IMPORTANT 0

enum read_status from_bmp(FILE* in, struct image* img) {
    struct bmp_header header = { 0 };

    if (!fread(&header, sizeof(struct bmp_header), 1, in)) {
        return READ_INVALID_HEADER;
    }
    if (header.bfType != H_TYPE) {
        return READ_INVALID_SIGNATURE;
    }

    fseek(in, header.bOffBits, SEEK_SET);
    
    bool img_b = create_image(img, (int64_t)header.biWidth, (uint64_t)header.biHeight);
    if (!img_b) {
        return READ_INVALID_BITS;
    }
    uint16_t padding = (4 - (3 * img->width) % 4) % 4;

    size_t str_cnt = 0;
    for (size_t i = 0; i < img->height; i++) {
        if (!fread((str_cnt + img->data), sizeof(struct pixel), header.biWidth, in)) {
            free(img->data);
            return READ_INVALID_BITS;
        }
        fseek(in, (long)padding, SEEK_CUR);
        str_cnt += header.biWidth;
    }
    return READ_OK;
}

enum write_status to_bmp(FILE* out, struct image const* img) {
    uint16_t padding = (4 - (3 * img->width) % 4) % 4;
    
    struct bmp_header header = { 0 };
    
    header.bfType = H_TYPE;
    header.bfileSize = sizeof(struct bmp_header) + (uint32_t) ((padding + img->width * sizeof(struct pixel)) * img->height);
    header.bfReserved = H_RESERVED;
    header.bOffBits = sizeof(struct bmp_header);

    header.biSize = H_SIZE;
    header.biWidth = (uint32_t) img->width;
    header.biHeight = (uint32_t) img->height;
    header.biPlanes = H_PLANES;
    header.biBitCount = H_BIT_COUNT;
    header.biCompression = H_COMPRESSION;
    header.biSizeImage = (uint32_t)((padding + img->width * sizeof(struct pixel)) * img->height);
    header.biXPelsPerMeter = H_X_PELS_PER_METER;
    header.biYPelsPerMeter = H_Y_PELS_PER_METER;
    header.biClrUsed = H_CRL_USED;
    header.biClrImportant = H_CRL_IMPORTANT;
    
    if (!fwrite(&header, sizeof(struct bmp_header), 1, out)) {
        return WRITE_ERROR_HEADER;
    }
    
    
    for (size_t i = 0; i < img->height; i++) {
        if (!fwrite((img->data) + img->width * i, sizeof(struct pixel), img->width, out)) {
            return WRITE_ERROR_BITS;
        }
        fseek(out, (long)padding, SEEK_CUR);
    }
    return WRITE_OK;
}
