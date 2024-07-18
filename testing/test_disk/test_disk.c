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
    }
}

void test_disk_io() {
    FILE* disk0 = physical_disks[0].file;
    
    // 写入单个字节
    size_t address = 0;
    unsigned char write_value = 0x41; // 'A'
    write_at(disk0, address, write_value);
    _TEST("Wrote byte %02X at address %zu\n", write_value, address);
    
    // 读取单个字节
    unsigned char read_value = read_at(disk0, address);
    _TEST("Read byte %02X at address %zu\n", read_value, address);

    // 写入多个字节
    unsigned char buffer_to_write[] = {0x42, 0x43, 0x44, 0x45}; // 'B', 'C', 'D', 'E'
    write_buffer_at(disk0, address + 1, buffer_to_write, sizeof(buffer_to_write));
    _TEST("Wrote buffer at address %zu\n", address + 1);
    
    // 读取多个字节
    unsigned char buffer_to_read[sizeof(buffer_to_write)];
    read_buffer_at(disk0, address + 1, buffer_to_read, sizeof(buffer_to_read));
    for (size_t i = 0; i < sizeof(buffer_to_read); i++) {
        _TEST("%02X ", buffer_to_read[i]);
    }
    _TEST("\n");
}

void test_disk_blocks() {
    init_disk_blocks();
    printDiskBlocks();
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
        _TEST("Failed to mount block 0 on disk 0\n");
    }
    if (!mount_disk_block(v_disk, 0, 1)) {
        _TEST("Failed to mount block 1 on disk 0\n");
    }

    // 打印虚拟磁盘信息
    _TEST("Virtual disk %s has size %zu and block count %zu\n", v_disk->name, v_disk->size, v_disk->block_size);

    for (size_t i = 0; i < v_disk->block_size; i++) {
        disk_block db = v_disk->mounted_blocks[i];
        _TEST("Block %zu: Disk ID %zu, Block ID %zu, Mounted %d\n", i, db.disk_id, db.block_id, db.mounted);
    }
}
