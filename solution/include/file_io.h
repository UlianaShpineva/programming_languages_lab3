#ifndef FILE_IO
#define FILE_IO

#include <stdio.h>
#include <stdbool.h>

bool read_file(char* filename, const FILE** file);
bool write_file(char* filename, const FILE** file);
bool close_file(FILE* file);

#endif
