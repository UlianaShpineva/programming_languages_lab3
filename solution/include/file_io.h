#ifndef FILE_IO
#define FILE_IO

#include <stdbool.h>
#include <stdio.h>

bool read_file(char* filename, FILE** file);
bool write_file(char* filename, FILE** file);
bool close_file(FILE* file);

#endif
