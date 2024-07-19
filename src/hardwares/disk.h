#ifndef DISK_H
#define DISK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

#define MAX_DISKS 4
#define DISK_NAME_MAX_LENGTH 256
#define META_DATA_ROWS 2
#define BYTES_PER_VIRTUAL_BYTE 3
#define VIRTUAL_BYTES_PER_ROW 16
#define BYTES_PER_DATA_ROW ((VIRTUAL_BYTES_PER_ROW * BYTES_PER_VIRTUAL_BYTE) + 1)

typedef struct {
    char name[DISK_NAME_MAX_LENGTH];
    size_t size;
    FILE *file;
} disk;

extern disk physical_disks[MAX_DISKS]; // 声明外部变量
extern int current_physical_disk_size; // 声明外部变量

size_t validate_disk(const char *disk_file, char *out_name); // 声明函数
void register_disk(const char *disk_file); // 声明函数

size_t get_data_head_offset(FILE* file);
size_t get_byte_offset(FILE* file, size_t address);
void write_at(FILE* file, size_t address, unsigned char value);
void write_buffer_at(FILE* file, size_t address, const unsigned char* buffer, size_t bufferSize);
unsigned char read_at(FILE* file, size_t address);
void read_buffer_at(FILE* file, size_t address, unsigned char* buffer, size_t bufferSize);

#endif // DISK_H
