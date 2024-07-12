// src/kernel.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"
#include "memory.h"
#include "mmu.h"
#include "allocate.h"
#include "virtual.h"
#include "process.h"
#include "disk.h"
#include "cpu.h"
#include "log.h"
#include "disk_io.h"

// Shell command handler
void handle_shell_command(const char *command);

int main() {
    // 初始化各个模块
    init_memory();
    init_blocks();
    init_pages();

    // 注册磁盘
    const char* disks[MAX_DISKS] = {
        "../src/hardwares/disks/disk0.disk",
        "../src/hardwares/disks/disk1.disk",
        "../src/hardwares/disks/disk2.disk",
        "../src/hardwares/disks/disk3.disk"
    };
    for (int i = 0; i < MAX_DISKS; i++) {
        register_disk(disks[i]);
    }

    // 启动shell
    char command[256];
    printf("Welcome to the SimulateOS Shell\n");
    while (1) {
        printf("SimulateOS> ");
        if (fgets(command, sizeof(command), stdin) != NULL) {
            // 移除换行符
            command[strcspn(command, "\n")] = '\0';
            handle_shell_command(command);
        }
    }
    return 0;
}

void handle_shell_command(const char *command) {
    if (strcmp(command, "exit") == 0) {
        printf("Exiting SimulateOS...\n");
        exit(0);
    } else if (strcmp(command, "list_disks") == 0) {
        for (int i = 0; i < current_physical_disk_size; i++) {
            printf("Disk %d: %s, Size: %zu bytes\n", i, physical_disks[i].name, physical_disks[i].size);
        }
    } else if (strncmp(command, "write_disk", 10) == 0) {
        size_t disk_num, address;
        unsigned char value;
        sscanf(command + 11, "%zu %zu %hhx", &disk_num, &address, &value);
        if (disk_num < current_physical_disk_size) {
            write_at(physical_disks[disk_num].file, address, value);
            printf("Wrote %02X to disk %zu at address %zu\n", value, disk_num, address);
        } else {
            printf("Invalid disk number\n");
        }
    } else if (strncmp(command, "read_disk", 9) == 0) {
        size_t disk_num, address;
        sscanf(command + 10, "%zu %zu", &disk_num, &address);
        if (disk_num < current_physical_disk_size) {
            unsigned char value = read_at(physical_disks[disk_num].file, address);
            printf("Read %02X from disk %zu at address %zu\n", value, disk_num, address);
        } else {
            printf("Invalid disk number\n");
        }
    } else {
        printf("Unknown command: %s\n", command);
    }
}
