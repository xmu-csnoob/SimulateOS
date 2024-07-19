// testing/test_disk.c
#include "test_disk.h"

void test_disks() {
    const char* disks[MAX_DISKS] = {
        "../src/hardwares/disks/disk0.disk",
        "../src/hardwares/disks/disk1.disk",
        "../src/hardwares/disks/disk2.disk",
        "../src/hardwares/disks/disk3.disk"
    };
    for (int i = 0; i < MAX_DISKS; i++) {
        register_disk(disks[i]);
        _TEST("Test: register disk %d, disk name : %s, disk size : %zu", i, physical_disks[i].name, physical_disks[i].size);
    }
}

void test_disk_io() {
    FILE* disk0 = physical_disks[0].file;
    
    // 写入单个字节
    size_t address = 0;
    unsigned char write_value = 0x41; // 'A'
    write_at(disk0, address, write_value);
    _TEST("Wrote byte %02X at address %zu", write_value, address);
    
    // 读取单个字节
    unsigned char read_value = read_at(disk0, address);
    _TEST("Read byte %c at address %zu", read_value, address);

    // 写入多个字节
    unsigned char buffer_to_write[] = {0x42, 0x43, 0x44, 0x45}; // 'B', 'C', 'D', 'E'
    write_buffer_at(disk0, address + 1, buffer_to_write, sizeof(buffer_to_write));
    _TEST("Wrote bytes from address %zu to address %zu", address + 1, address + sizeof(buffer_to_write));
    
    // 读取多个字节
    unsigned char buffer_to_read[sizeof(buffer_to_write)];
    read_buffer_at(disk0, address, buffer_to_read, sizeof(buffer_to_read));
    unsigned char res[5];
    for (size_t i = 0; i < sizeof(buffer_to_read); i++) {
        res[i] = buffer_to_read[i];
    }
    _TEST("Read bytes from %zu to %zu : %s", address + 1, address + sizeof(buffer_to_write), res);
}

void test_disk_blocks() {
    init_disk_blocks();
    print_disk_blocks();
}

void test_virtual_disk() {
    virtual_disk* v_disk = create_virtual_disk("TestVirtualDisk");
    if (v_disk == NULL) {
        _TEST("Failed to create virtual disk\n");
        return;
    }

    // 初始化物理磁盘块
    init_disk_blocks();

    // 挂载一些块（假设磁盘0和块0存在且未挂载）
    if (!mount_disk_block(v_disk, 0, 0)) {
        _TEST("Failed to mount block 0 on disk 0");
    }
    if (!mount_disk_block(v_disk, 0, 1)) {
        _TEST("Failed to mount block 1 on disk 0");
    }

    // 打印虚拟磁盘信息
    _TEST("Virtual disk %s has size %zu and block count %zu", v_disk->name, v_disk->size, v_disk->block_size);

    for (size_t i = 0; i < v_disk->block_size; i++) {
        disk_block db = v_disk->mounted_blocks[i];
        _TEST("Block %zu: Disk ID %zu, Block ID %zu, Mounted %d", i, db.disk_id, db.block_id, db.mounted);
    }

    print_disk_blocks();
}
