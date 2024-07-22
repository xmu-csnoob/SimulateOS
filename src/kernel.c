// src/kernel.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "virtual.h"
#include "process.h"
#include "disk.h"
#include "cpu.h"
#include "filesystem.h"
#include "log.h"

// Shell command handler
void handle_shell_command(const char *command);
void init_system();

file_system_entity* current_dir;

int main() {
    // 初始化日志级别
    init_log_level();
    
    // 初始化系统
    init_system();

    // 启动shell
    char command[256];
    printf("Welcome to the SimulateOS Shell\n");
    current_dir = system_file_system->root;
    while (1) {
        printf("SimulateOS > : %s# ", get_entity_absolute_path(current_dir));
        if (fgets(command, sizeof(command), stdin) != NULL) {
            // 移除换行符
            command[strcspn(command, "\n")] = '\0';
            handle_shell_command(command);
        }
    }
    return 0;
}

void init_system() {
    // 初始化各个模块
    init_memory();
    init_blocks();
    init_pages();

    // 注册磁盘
    const char* disks[MAX_DISKS] = {
        "src/hardwares/disks/disk0.disk",
        "src/hardwares/disks/disk1.disk",
        "src/hardwares/disks/disk2.disk",
        "src/hardwares/disks/disk3.disk"
    };
    for (int i = 0; i < MAX_DISKS; i++) {
        register_disk(disks[i]);
    }
    init_disk_blocks();
    // 创建文件系统
    virtual_disk *v_disk = create_virtual_disk("RootDisk");
    virtual_disk* v_disks[1];
    v_disks[0] = v_disk;
    for (int i = 0; i < 4; i++) {
        mount_disk_block(v_disk->id, i, 0);
        mount_disk_block(v_disk->id, i, 1);
    }
    file_system *fs = create_file_system(v_disks);
    system_file_system = fs;
    _INFO("File system created with root directory: %s", fs->root->name);
}

void handle_shell_command(const char *command) {
    if (strcmp(command, "exit") == 0) {
        printf("Exiting SimulateOS...\n");
        exit(0);
    } else if (strcmp(command, "ld") == 0) {
        for (int i = 0; i < current_physical_disk_size; i++) {
            printf("Disk %d: %s, Size: %zu bytes\n", i, physical_disks[i].name, physical_disks[i].size);
        }
    } else if (strncmp(command, "wd", 2) == 0) {
        size_t disk_num, address;
        unsigned char value;
        sscanf(command + 2, "%zu %zu %hhx", &disk_num, &address, &value);
        if (disk_num < current_physical_disk_size) {
            write_at(physical_disks[disk_num].file, address, value);
            printf("Wrote %02X to disk %zu at address %zu\n", value, disk_num, address);
        } else {
            printf("Invalid disk number\n");
        }
    } else if (strncmp(command, "rd", 2) == 0) {
        size_t disk_num, address;
        sscanf(command + 2, "%zu %zu", &disk_num, &address);
        if (disk_num < current_physical_disk_size) {
            unsigned char value = read_at(physical_disks[disk_num].file, address);
            printf("Read %02X from disk %zu at address %zu\n", value, disk_num, address);
        } else {
            printf("Invalid disk number\n");
        }
    } else if (strncmp(command, "touch", 5) == 0) {
        char filename[256];
        sscanf(command + 5, "%s", filename);
        if (current_dir != NULL) {
            file_system_entity *file = create_entity(filename, FILE_TYPE, current_dir);
            printf("File %s created\n", filename);
        } else {
            printf("Current dir does not exist\n");
            current_dir = system_file_system->root;
        }
    } else if (strncmp(command, "mkdir", 5) == 0) {
        char dir_name[256];
        sscanf(command + 5, "%s", dir_name);
        if (current_dir != NULL) {
            file_system_entity *file = create_entity(dir_name, DIR_TYPE, current_dir);
            printf("Directory %s created\n", dir_name);
        } else {
            printf("Current dir does not exist\n");
            current_dir = system_file_system->root;
        }
    } else if (strcmp(command, "ls") == 0) {
        if (current_dir != NULL) {
            print_dir(current_dir);
            printf("\n");
        } else {
            printf("Failed to list files\n");
        }
    } else if (strncmp(command, "cd", 2) == 0){
        char dir_name[256];
        if (sscanf(command + 3, "%255s", dir_name) == 1) {
            _TRACE("dir_name is %s", dir_name);
            file_system_entity* dir = find_entity(current_dir, dir_name);
            _TRACE("dir is NULL : %d", dir == NULL);
            if (dir == NULL) {
                printf("%s : No such file or directory.\n", dir_name);
                return;
            }
            _TRACE("dir type is %s", dir->type == FILE_TYPE ? "File" : "Directory");
            if (dir->type != DIR_TYPE) {
                printf("%s : Not a directory.\n", dir_name);
                return;
            }
            _TRACE("move to directory %s", dir->name);
            current_dir = dir;
        } else {
            // If no directory name is provided, change to the root directory
            current_dir = system_file_system->root;
        }
    }else if(strcmp(command, "clear") == 0){
        system("clear");
    }else if(strncmp(command, "set_log_level", 13) == 0){
        int log_level;
        sscanf(command + 13, "%d", &log_level);
        set_log_level(log_level);
    }else{
        printf("Unknown command : %s\n", command);
    }
}
