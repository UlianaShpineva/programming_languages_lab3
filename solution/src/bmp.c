#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include "bmp.h"
#include "image.h"

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

static void print_bmp_header(struct bmp_header const* header);

enum read_status from_bmp(FILE* in, struct image** img) {
    struct bmp_header header;
    (*img) = malloc(sizeof(struct image));
    if (!fread(&header, sizeof(struct bmp_header), 1, in)) {
        //fprintf(stderr, "Couldn't read header");
        return READ_INVALID_HEADER;
    }
    if (header.bfType != H_TYPE) {
        return READ_INVALID_SIGNATURE;
    }

    fseek(in, header.bOffBits, SEEK_SET);
    
    (*img)->height = (int64_t) header.biHeight;
    (*img)->width = (int64_t) header.biWidth;
    (*img)->data = malloc(sizeof(struct pixel) * header.biHeight * header.biWidth);

    uint16_t padding = (4 - (3 * (*img)->width) % 4) % 4;

    size_t str_cnt = 0;
    for (size_t i = 0; i < (*img)->height; i++) {
        if (!fread((str_cnt + (*img)->data), sizeof(struct pixel), header.biWidth, in)) {
            return READ_INVALID_BITS;
        }
        fseek(in, (long)padding, SEEK_CUR);
        str_cnt += header.biWidth;
    }
    return READ_OK;
}

enum write_status to_bmp(FILE* out, struct image const* img) {
    uint16_t padding = (4 - (3 * img->width) % 4) % 4;
    
    struct bmp_header* header = malloc(sizeof(struct bmp_header));

    header->bfType = H_TYPE;
    header->bfileSize = sizeof(struct bmp_header) + (uint32_t) ((padding + img->width * sizeof(struct pixel)) * img->height);
    header->bfReserved = H_RESERVED;
    header->bOffBits = sizeof(struct bmp_header);

    header->biSize = H_SIZE;
    header->biWidth = (uint32_t) img->width;
    header->biHeight = (uint32_t) img->height;
    header->biPlanes = H_PLANES;
    header->biBitCount = H_BIT_COUNT;
    header->biCompression = H_COMPRESSION;
    header->biSizeImage = (uint32_t)((padding + img->width * sizeof(struct pixel)) * img->height);
    header->biXPelsPerMeter = H_X_PELS_PER_METER;
    header->biYPelsPerMeter = H_Y_PELS_PER_METER;
    header->biClrUsed = H_CRL_USED;
    header->biClrImportant = H_CRL_IMPORTANT;
    
    if (!fwrite(header, sizeof(struct bmp_header), 1, out)) {
        return WRITE_ERROR_HEADER;
    }
    
    
    size_t str_cnt = 0;
    for (size_t i = 0; i < img->height; i++) {
        if (!fwrite((img->data) + str_cnt, sizeof(struct pixel), img->width, out)) {
            return WRITE_ERROR_BITS;
        }
        fseek(out, (long)padding, SEEK_CUR);
        str_cnt += img->width;
    }

    return WRITE_OK;
}
