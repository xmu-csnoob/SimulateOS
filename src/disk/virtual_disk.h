#ifndef VIRTUAL_DISK_H
#define VIRTUAL_DISK_H

#include "disk_block.h"

typedef struct {
    char* name;
    size_t block_size;
    size_t size;
    disk_block* mounted_blocks;
} virtual_disk;

int mount_disk_block(virtual_disk* v_disk, size_t disk_id, size_t block_id) {
    disk_block db = disk_blocks_table[disk_id]->blocks[block_id];
    if (db.mounted == 1) {
        _ERROR("Error: trying to mount an already mounted block at disk %zu, block %zu\n", disk_id, block_id);
        return 0;
    }
    disk_blocks_table[disk_id]->blocks[block_id].mounted = 1;

    // Reallocate memory to accommodate the new block
    v_disk->mounted_blocks = (disk_block*)realloc(v_disk->mounted_blocks, sizeof(disk_block) * (v_disk->block_size + 1));
    if (v_disk->mounted_blocks == NULL) {
        _ERROR("Error: memory reallocation failed\n, allocate size is %d", sizeof(disk_block) * (v_disk->block_size + 1));
        return -1;
    }
    
    v_disk->mounted_blocks[v_disk->block_size] = db;
    v_disk->block_size++;
    v_disk->size += DISK_BLOCK_SIZE;
    return 1;
}

// Function to create a new virtual disk
virtual_disk* create_virtual_disk(const char* name) {
    virtual_disk* v_disk = (virtual_disk*)malloc(sizeof(virtual_disk));
    if (v_disk == NULL) {
        fprintf(stderr, "Error: memory allocation failed for virtual disk\n");
        return NULL;
    }
    v_disk->name = strdup(name);
    v_disk->block_size = 0;
    v_disk->size = 0;
    v_disk->mounted_blocks = NULL;
    return v_disk;
}

// Function to free a virtual disk
void free_virtual_disk(virtual_disk* v_disk) {
    if (v_disk) {
        free(v_disk->name);
        free(v_disk->mounted_blocks);
        free(v_disk);
    }
}

#endif
