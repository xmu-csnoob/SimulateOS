#ifndef DISK_H
#define DISK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

#define MAX_DISKS 4
#define DISK_NAME_MAX_LENGTH 256

typedef struct {
    char name[DISK_NAME_MAX_LENGTH];
    size_t size;
    FILE *file;
} disk;

extern disk physical_disks[MAX_DISKS]; // 声明外部变量
extern int current_physical_disk_size; // 声明外部变量

size_t validate_disk(const char *disk_file, char *out_name); // 声明函数
void register_disk(const char *disk_file); // 声明函数

#endif // DISK_H
