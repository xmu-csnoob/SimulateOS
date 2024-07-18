// src/test.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "memory.h"
#include "virtual.h"
#include "process.h"
#include "disk.h"
#include "cpu.h"
#include "log.h"
#include "virtual_disk.h"

// 测试函数声明
void test_memory();
void test_cpu();
void test_disks();
void test_disk_io();
void test_disk_blocks();
void test_virtual_disk();
void test_process();

// 工具函数声明
void print_memory_content(int start, int end);
void generate_disks();
void create_disk_file(const char *filename, const char* disk_name, size_t disk_size);

int main() {
    generate_disks();
    
    _INFO("Running memory tests...\n");
    test_memory();
    
    _INFO("Running CPU tests...\n");
    test_cpu();
    
    _INFO("Running disk tests...\n");
    test_disks();
    test_disk_io();
    test_disk_blocks();
    test_virtual_disk();
    
    _INFO("Running process tests...\n");
    test_process();
    
    return 0;
}

void print_memory_content(int start, int end) {
    for (int i = start; i < end; i++) {
        printf("memory[%d] = 0x%02X\n", i, memory[i]);
    }
    printf("\n");
}

void test_memory() {
    _INFO("Initializing memory...\n");
    init_memory();
    init_blocks();
    init_pages();
    _INFO("Memory initialization complete.\n");

    // Perform memory operations
    BYTE data[] = {0xDE, 0xAD, 0xBE, 0xEF};
    assign_memory(0, data, sizeof(data));
    BYTE *retrieved_data = access_memory(0);
    
    _INFO("Memory content at address 0:\n");
    for (size_t i = 0; i < sizeof(data); i++) {
        printf("0x%02X ", retrieved_data[i]);
    }
    printf("\n");
}

void test_cpu() {
    CPU cpu;
    init_cpu(&cpu);

    instruction program[8];
    program[0] = (instruction){LOAD, 2, 5, 0};  // R2 = 5
    program[1] = (instruction){LOAD, 1, 1, 0};  // R1 = 1
    program[2] = (instruction){ADD, 2, 2, 1};   // R2 = R0 + R1 -> R2 = 5 + 1 = 6
    program[3] = (instruction){LOAD, 3, 10, 0}; // R3 = 10
    program[4] = (instruction){CMP, 2, 3, 0};   // Compare R2 and R3
    program[5] = (instruction){JNE, 1, 0, 0};   // If R2 != R0, jump to instruction at address 1 (loop)
    program[6] = (instruction){JE, 7, 0, 0};    // If R2 == R0, jump to instruction at address 7
    program[7] = (instruction){HALT, 0, 0, 0};  // Halt

    while (cpu.state == CPU_RUNNING) {
        cpu.ir = cpu.pc;
        execute_instruction(&cpu, &program[cpu.ir]);
        print_cpu_state(&cpu);
    }
}

void test_disks() {
    const char* disks[MAX_DISKS] = {
        "../src/hardwares/disks/disk0.disk",
        "../src/hardwares/disks/disk1.disk",
        "../src/hardwares/disks/disk2.disk",
        "../src/hardwares/disks/disk3.disk"
    };

    for (int i = 0; i < MAX_DISKS; i++) {
        register_disk(disks[i]);
    }
}

void generate_disks() {
    create_disk_file("../src/hardwares/disks/disk0.disk", "Samsung 990 pro", 256);
    create_disk_file("../src/hardwares/disks/disk1.disk", "Samsung 990 pro", 512);
    create_disk_file("../src/hardwares/disks/disk2.disk", "Samsung 990 pro", 1024);
    create_disk_file("../src/hardwares/disks/disk3.disk", "Samsung 990 pro", 2048);
}

void create_disk_file(const char *filename, const char *disk_name, size_t disk_size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Failed to create file");
        return;
    }

    // 写入磁盘元数据
    fprintf(file, "Disk Name: %s\n", disk_name);
    fprintf(file, "Disk Size: %zu bytes\n", disk_size);

    // 写入二进制数据
    for (size_t i = 0; i < disk_size; i++) {
        fprintf(file, "00 "); // 每个字节用 "00 " 表示
        if ((i + 1) % 16 == 0) {
            fprintf(file, "\n"); // 每行 16 个字节
        }
    }

    fclose(file);
    printf("Disk file '%s' created successfully.\n", filename);
}

void test_disk_io() {
    FILE* disk0 = physical_disks[0].file;
    
    // 写入单个字节
    size_t address = 0;
    unsigned char write_value = 0x41; // 'A'
    write_at(disk0, address, write_value);
    _INFO("Wrote byte %02X at address %zu\n", write_value, address);
    
    // 读取单个字节
    unsigned char read_value = read_at(disk0, address);
    _INFO("Read byte %02X at address %zu\n", read_value, address);

    // 写入多个字节
    unsigned char buffer_to_write[] = {0x42, 0x43, 0x44, 0x45}; // 'B', 'C', 'D', 'E'
    write_buffer_at(disk0, address + 1, buffer_to_write, sizeof(buffer_to_write));
    _INFO("Wrote buffer at address %zu\n", address + 1);
    
    // 读取多个字节
    unsigned char buffer_to_read[sizeof(buffer_to_write)];
    read_buffer_at(disk0, address + 1, buffer_to_read, sizeof(buffer_to_read));
    for (size_t i = 0; i < sizeof(buffer_to_read); i++) {
        _INFO("%02X ", buffer_to_read[i]);
    }
    printf("\n");
}

void test_disk_blocks() {
    init_disk_blocks();
    printDiskBlocks();
}

void test_virtual_disk() {
    virtual_disk* v_disk = create_virtual_disk("TestVirtualDisk");
    if (v_disk == NULL) {
        printf("Failed to create virtual disk\n");
        return;
    }

    // 初始化物理磁盘块
    init_disk_blocks();

    // 挂载一些块（假设磁盘0和块0存在且未挂载）
    if (!mount_disk_block(v_disk, 0, 0)) {
        printf("Failed to mount block 0 on disk 0\n");
    }
    if (!mount_disk_block(v_disk, 0, 1)) {
        printf("Failed to mount block 1 on disk 0\n");
    }

    // 打印虚拟磁盘信息
    printf("Virtual disk %s has size %zu and block count %zu\n", v_disk->name, v_disk->size, v_disk->block_size);

    for (size_t i = 0; i < v_disk->block_size; i++) {
        disk_block db = v_disk->mounted_blocks[i];
        printf("Block %zu: Disk ID %zu, Block ID %zu, Mounted %d\n", i, db.disk_id, db.block_id, db.mounted);
    }
    printDiskBlocks();

    // 释放虚拟磁盘
    free_virtual_disk(v_disk);
}

void test_process() {
    init_memory();
    init_blocks();
    init_pages();
    create_process(80);
    print_block(0);
    print_block(1);
    print_block(2);
    printf("%zu\n", page_table[pcb_table[0].page_table[0]].physical_page);
    printf("%zu\n", page_table[pcb_table[0].page_table[1]].physical_page);
    printf("%zu\n", page_table[pcb_table[0].page_table[2]].physical_page);
}
