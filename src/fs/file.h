#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>

#define MAX_FILENAME_LENGTH 256

typedef enum{
    FILE_TYPE_EXE,
    FILE_TYPE_TXT
} file_type;

typedef struct{
    char name[MAX_FILENAME_LENGTH];
    file_type type;
    size_t start;
    size_t size;
} file_entry;

#endif