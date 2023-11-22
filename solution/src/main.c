#include "image.h"
#include "bmp.h"
#include "file_io.h"
#include"rotation.h"
#include "utils.h"

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

    struct image img = { 0 };
    enum read_status read_bmp_status = from_bmp(inp_file, &img);
    print_read_bmp_status(read_bmp_status);
    if (read_bmp_status != READ_OK) {
        return ERROR_EXIT_CODE;
    }

    struct image rotated = { 0 };
    rotated = image_rotation(img, inp_angle);
    if(img.data) 
        free(img.data);
    bool close_inp_file_status = close_file(inp_file);
    if (!close_inp_file_status) {
        if (rotated.data)
            free(rotated.data);
        fprintf(stderr, "Couldn't close input file %s", inp_filename);
        return ERROR_EXIT_CODE;
    }

    FILE* outp_file;
    bool open_outp_file_status = write_file(outp_filename, &outp_file);
    if (!open_outp_file_status)
    {
        if (rotated.data)
            free(rotated.data);
        fprintf(stderr, "Couldn't open output file %s", outp_filename);
        return ERROR_EXIT_CODE;
    }
    
    enum write_status write_bmp_status = to_bmp(outp_file, &rotated);
    if (rotated.data)
        free(rotated.data);
    print_write_bmp_status(write_bmp_status);
    if (write_bmp_status != WRITE_OK) {
        return ERROR_EXIT_CODE;
    }
    bool close_outp_file_status = close_file(outp_file);
    if (!close_outp_file_status) {
        fprintf(stderr, "Couldn't close output file %s", outp_filename);
        return ERROR_EXIT_CODE;
    }

    return 0;
}
