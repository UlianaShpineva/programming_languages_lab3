#include <stdbool.h>
#include <stdio.h>

bool read_file(char* filename, FILE** file) {
    if (filename == NULL || (*file = fopen(filename, "rb")) == NULL) return false;
    return true;
}

bool write_file(char* filename, FILE** file) {
    if (filename == NULL || (*file = fopen(filename, "wb")) == NULL) return false;
    return true;
}

bool close_file(FILE* file) {
    if (fclose(file) != 0) return false;
    return true;
}
