#include "image.h"
#include "bmp.h"
#include "file_io.h"
#include"rotation.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define ERROR_EXIT_CODE 1

int main( int argc, char** argv ) {
    if (argc != 4) {
        fprintf(stderr, "There should be 3 args.\nYour input: %d args.\n", (argc - 1));
        return ERROR_EXIT_CODE;
    }
    char* inp_filename = argv[1];
    char* outp_filename = argv[2];
    int inp_angle = atoi(argv[3]);

    if (!(inp_angle % 90 == 0 && inp_angle <= 270 && inp_angle >= -270)) {
        fprintf(stderr, "Wrong angle arg %d.\nAvaliable angle inputs: 0, 90, -90, 180, -180, 270, -270.", inp_angle);
        return ERROR_EXIT_CODE;
    }

    FILE* inp_file;
    bool open_inp_file_status = read_file(inp_filename, &inp_file);
    if (!open_inp_file_status)
    {
        fprintf(stderr, "Couldn't open input file %s", inp_filename);
        return ERROR_EXIT_CODE;
    }

    struct image* img;
    enum read_status read_bmp_status = from_bmp(inp_file, &img);
    switch (read_bmp_status)
    {
    case READ_INVALID_HEADER:
        fprintf(stderr, "Couldn't read header");
        return ERROR_EXIT_CODE;
        break;
    case READ_INVALID_SIGNATURE:
        fprintf(stderr, "Wrong file format");
        return ERROR_EXIT_CODE;
        break;
    case READ_INVALID_BITS:
        fprintf(stderr, "Couldn't read data");
        return ERROR_EXIT_CODE;
        break;
    case READ_OK:
    default:
        fprintf(stdout, "Successfully read file.");
        break;
    }

    struct image rotated = { 0 };
    rotated = image_rotation(*img, inp_angle);
    free((*img).data);
    bool close_inp_file_status = close_file(inp_file);
    if (!close_inp_file_status) {
        fprintf(stderr, "Couldn't close input file %s", inp_filename);
        return ERROR_EXIT_CODE;
    }

    FILE* outp_file;
    bool open_outp_file_status = write_file(outp_filename, &outp_file);
    if (!open_outp_file_status)
    {
        fprintf(stderr, "Couldn't open output file %s", outp_filename);
        return ERROR_EXIT_CODE;
    }
    
    enum write_status write_bmp_status = to_bmp(outp_file, &rotated);
    free(rotated.data);
    switch (write_bmp_status)
    {
    case WRITE_ERROR_HEADER:
        fprintf(stderr, "Couldn't write header");
        return ERROR_EXIT_CODE;
        break;
    case WRITE_ERROR_BITS:
        fprintf(stderr, "Couldn't write data");
        return ERROR_EXIT_CODE;
        break;
    case WRITE_OK:
    default:
        fprintf(stdout, "Successfully write file.");
        break;
    }

    bool close_outp_file_status = close_file(outp_file);
    if (!close_outp_file_status) {
        fprintf(stderr, "Couldn't close output file %s", outp_filename);
        return ERROR_EXIT_CODE;
    }

    return 0;
}
