#include "test_disk.h"
#include <assert.h>

void test_register_disks() {
    _TEST("Running test_register_disks...");
    generate_disks();
    const char* disks[MAX_DISKS] = {
        "src/hardwares/disks/disk0.disk",
        "src/hardwares/disks/disk1.disk",
        "src/hardwares/disks/disk2.disk",
        "src/hardwares/disks/disk3.disk"
    };
    for (int i = 0; i < MAX_DISKS; i++) {
        register_disk(disks[i]);
        _TRACE("Registered disk %d, name: %s, size: %zu", i, physical_disks[i].name, physical_disks[i].size);
        assert(strcmp(physical_disks[i].name, "Samsung 990 pro") == 0);
        assert(physical_disks[i].size > 0);
    }
    _TEST("test_register_disks passed.");
}

void test_disk_io() {
    _TEST("Running test_disk_io...");
    generate_disks();
    register_disk("../src/hardwares/disks/disk0.disk");

    FILE* disk0 = physical_disks[0].file;
    
    // 写入单个字节
    size_t address = 0;
    unsigned char write_value = 0x41; // 'A'
    write_at(disk0, address, write_value);
    _TRACE("Wrote byte %02X at address %zu", write_value, address);
    
    // 读取单个字节
    unsigned char read_value = read_at(disk0, address);
    _TRACE("Read byte %c at address %zu", read_value, address);
    assert(read_value == write_value);

    // 写入多个字节
    unsigned char* buffer_to_write = (unsigned char*)malloc(4 * sizeof(unsigned char*));
    buffer_to_write[0] = 0x42;
    buffer_to_write[1] = 0x43;
    buffer_to_write[2] = 0x44;
    buffer_to_write[3] = 0x45;
    write_buffer_at(disk0, address + 1, buffer_to_write, 4);
    _TRACE("Wrote bytes %s from address %zu to address %zu", buffer_to_write, address + 1, address + 4);
    
    // 读取多个字节
    unsigned char buffer_to_read[sizeof(buffer_to_write)];
    read_buffer_at(disk0, address, buffer_to_read, sizeof(buffer_to_read));
    _TRACE("Read bytes from %zu to %zu: ", address + 1, address + sizeof(buffer_to_write));
    assert(memcmp(buffer_to_write, buffer_to_read + 1, sizeof(buffer_to_write)) == 0);

    _TEST("test_disk_io passed.");
}


void test_virtual_disk() {
    _TEST("Running test_virtual_disk...");
    generate_disks();
    init_disk_blocks();

    virtual_disk* v_disk = create_virtual_disk("TestVirtualDisk");
    assert(v_disk != NULL);
    _TRACE("Created virtual disk %zu : %s", v_disk->id, v_disk->name);
    
    // 挂载一些块（假设磁盘0和块0存在且未挂载）
    assert(mount_disk_block(v_disk->id, 0, 0));
    _TRACE("Mounted block 0 on disk 0");
    assert(mount_disk_block(v_disk->id, 1, 0));
    _TRACE("Mounted block 1 on disk 0");

    // 打印虚拟磁盘信息
    _TRACE("Virtual disk %s has size %zu and block count %zu", v_disk->name, v_disk->size, v_disk->block_count);

    for (size_t i = 0; i < v_disk->block_count; i++) {
        disk_block db = v_disk->mounted_blocks[i];
        _TRACE("Block %zu: Disk ID %zu, Block ID %zu, Mounted %d", i, db.disk_id, db.block_id, db.mounted);
        assert(db.mounted == 1);
    }

    print_disk_blocks();
    _TEST("test_virtual_disk passed.");
}

void test_virtual_disk_io() {
    _TEST("Running test_virtual_disk_io...");
    generate_disks();
    init_disk_blocks();

    virtual_disk* v_disk = virtual_disks[0];

    // 测试单字节读写
    size_t address = 16;
    unsigned char write_byte = 0xAB;
    write_virtual_disk_at(0, address, write_byte);
    unsigned char read_byte = read_virtual_disk_at(0, address);
    assert(read_byte == write_byte);
    _TEST("Single byte read/write passed at address %zu", address);

    // 测试多字节读写
    unsigned char write_buffer[128];
    for (int i = 0; i < 128; i++) {
        write_buffer[i] = (unsigned char)i;
    }
    write_bytes_virtual_disk_at(0, 0, write_buffer, 128);
    unsigned char* read_buffer = read_bytes_virtual_disk_at(0, 0, 128);
    assert(memcmp(write_buffer, read_buffer, 128) == 0);
    _TEST("Multiple byte read/write passed for 128 bytes");

    free(read_buffer);
    _TEST("test_virtual_disk_io passed.");
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
    _INFO("Disk file '%s' created successfully.\n", filename);
}

void generate_disks(){
    create_disk_file("src/hardwares/disks/disk0.disk", "Samsung 990 pro", 256);
    create_disk_file("src/hardwares/disks/disk1.disk", "Samsung 990 pro", 512);
    create_disk_file("src/hardwares/disks/disk2.disk", "Samsung 990 pro", 1024);
    create_disk_file("src/hardwares/disks/disk3.disk", "Samsung 990 pro", 2048);
}

void test_disks() {
    _TEST("Running all disk tests...");
    test_register_disks();
    test_disk_io();
    test_virtual_disk();
    test_virtual_disk_io();
    _TEST("All disk tests passed.");
}

