#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "virtual_disk.h"
#include "log.h"

#define MAX_FILES 1024
#define MAX_FILE_NAME 256
#define FS_MAX_VIRTUAL_DISKS 4

typedef enum {
    FILE_TYPE,
    DIR_TYPE
} file_type;

typedef struct file_system_entity {
    char name[MAX_FILE_NAME];
    file_type type;
    size_t size;
    struct file_system_entity *parent;
    struct file_system_entity *children[MAX_FILES];
    size_t children_count;
    disk_block *blocks[MAX_FILES];  // 关联的磁盘块
    size_t block_count;
} file_system_entity;

typedef struct {
    file_system_entity *root;
    virtual_disk* v_disks[FS_MAX_VIRTUAL_DISKS];
    size_t v_disk_count;
} file_system;

extern file_system* system_file_system;

// 创建文件系统
file_system* create_file_system(virtual_disk** v_disks);

// 创建文件系统实体
file_system_entity* create_entity(const char *name, file_type type, file_system_entity *parent);

// 添加子实体到实体
int add_entity(file_system_entity *parent, file_system_entity *child);

// 查找实体
file_system_entity* find_entity(file_system_entity *parent, const char *name);

// 打印文件系统
void print_filesystem(file_system *fs);
void print_dir(file_system_entity *dir);

// 文件读写
int write_file(file_system* fs, file_system_entity *file, const char *data, size_t length);
unsigned char* read_file(file_system_entity *file);

#endif // FILESYSTEM_H
