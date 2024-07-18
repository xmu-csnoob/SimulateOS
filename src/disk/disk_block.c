#include "disk_block.h"

disk_blocks* disk_blocks_table[MAX_DISKS]; // 定义外部变量

void init_disk_blocks() {
    for (int i = 0; i < MAX_DISKS; i++) {
        disk d = physical_disks[i];
        size_t size = d.size;
        size_t blocks = size / DISK_BLOCK_SIZE;
        disk_blocks *t = (disk_blocks*)malloc(sizeof(disk_blocks) + sizeof(disk_block) * blocks);
        if (t == NULL) {
            _ERROR("Memory allocation failed for disk %d\n", i);
            return;
        }
        t->block_count = blocks;
        for (size_t j = 0; j < blocks; j++) {
            t->blocks[j].disk_id = i;
            t->blocks[j].block_id = j;
            t->blocks[j].mounted = 0;
        }
        disk_blocks_table[i] = t;
    }
}

void printDiskBlocks() {
    for (int i = 0; i < MAX_DISKS; i++) {
        disk_blocks *dbs = disk_blocks_table[i];
        size_t blocks = dbs->block_count;
        printf("Disk id: %d, has %zu blocks in total.\n", i, blocks);
        for (size_t j = 0; j < blocks; j++) {
            disk_block db = dbs->blocks[j];
            printf("Block id %zu, %s\n", j, db.mounted == 0 ? "has not been mounted." : "has been mounted.");
        }
    }
}
