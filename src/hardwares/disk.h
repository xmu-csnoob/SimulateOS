#ifndef DISK_H
#define DISK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

#define MAX_DISKS 4

typedef struct {
    char* name;
    size_t size;
} disk;

disk physical_disks[MAX_DISKS];
int current_physical_disk_size = 0;

// 验证磁盘文件并返回磁盘大小
size_t validate_disk(const char* disk_file, char* out_name, size_t name_size) {
    FILE* file = fopen(disk_file, "r");
    if (file == NULL) {
        _ERROR("Failed to open disk file '%s'", disk_file);
        return 0; // 文件不存在
    }

    char buffer[256];
    size_t disk_size = 0;
    int found_data_section = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        if (strncmp(buffer, "Disk Name: ", 11) == 0) {
            strncpy(out_name, buffer + 11, name_size - 1);
            out_name[name_size - 1] = '\0';
            // 去掉换行符
            char* newline = strchr(out_name, '\n');
            if (newline) {
                *newline = '\0';
            }
        } else if (strncmp(buffer, "Disk Size: ", 11) == 0) {
            sscanf(buffer + 11, "%zu", &disk_size);
        } else if (strncmp(buffer, "Data:", 5) == 0) {
            found_data_section = 1;
            break;
        }
    }

    fclose(file);

    if (disk_size > 0 && found_data_section && strlen(out_name) > 0) {
        _INFO("Validated disk '%s' with size %zu bytes", out_name, disk_size);
        return disk_size;
    } else {
        _WARN("Invalid disk file '%s'", disk_file);
        return (size_t)-1; // 无效的磁盘文件
    }
}

// 注册磁盘文件
void register_disk(const char* disk_file) {
    if (current_physical_disk_size >= MAX_DISKS) {
        _ERROR("Cannot register disk: Maximum number of disks reached, disk size is %d", current_physical_disk_size);
        return;
    }

    char disk_name[256];
    size_t existing_size = validate_disk(disk_file, disk_name, sizeof(disk_name));
    if (existing_size == (size_t)-1) {
        _ERROR("Disk '%s' is invalid.", disk_file);
        return;
    } else if (existing_size == 0) {
        _ERROR("Disk '%s' does not exist.", disk_file);
        return;
    }

    physical_disks[current_physical_disk_size].name = strdup(disk_name);
    physical_disks[current_physical_disk_size].size = existing_size;
    current_physical_disk_size++;

    _INFO("Disk '%s' registered successfully with size %zu bytes.", disk_name, existing_size);
}

#endif
