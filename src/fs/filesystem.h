#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "virtual_disk.h"
#include "log.h"

#define MAX_FILES 1024
#define MAX_FILE_NAME 256

// 文件类型
typedef enum {
    FILE_TYPE,
    DIR_TYPE
} file_type;

// 文件系统实体结构
typedef struct file_system_entity {
    char name[MAX_FILE_NAME];
    file_type type;
    size_t size;
    char *content; // 对于文件，存储内容；对于文件夹，存储子文件/文件夹
    struct file_system_entity *parent;
    struct file_system_entity *children[MAX_FILES];
    size_t children_count;
} file_system_entity;

// 文件系统结构
typedef struct {
    file_system_entity *root;
    virtual_disk *v_disks;
} file_system;

// 创建文件系统
file_system* create_file_system(virtual_disk *v_disks);

// 创建文件系统实体
file_system_entity* create_entity(const char *name, file_type type);

// 添加子实体到实体
int add_entity(file_system_entity *parent, file_system_entity *child);

// 查找实体
file_system_entity* find_entity(file_system_entity *parent, const char *name);

// 打印文件系统
void print_filesystem(file_system *fs);

#endif // FILESYSTEM_H
