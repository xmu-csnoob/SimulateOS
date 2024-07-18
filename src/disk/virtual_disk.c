#include "virtual_disk.h"

disk_blocks* disk_blocks_table[MAX_DISKS];

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

void print_disk_blocks() {
    int mounted_size = 0;
    for (int i = 0; i < MAX_DISKS; i++) {
        disk_blocks *dbs = disk_blocks_table[i];
        size_t blocks = dbs->block_count;
        _TRACE("Disk id: %d, has %zu blocks in total.\n", i, blocks);
        for (size_t j = 0; j < blocks; j++) {
            disk_block db = dbs->blocks[j];
            _TRACE("Block id %zu, %s\n", j, db.mounted == 0 ? "has not been mounted." : "has been mounted.");
            if(db.mounted == 1){
                mounted_size++;
                _TEST("Block id %zu is mounted to virtual disk %zu", j, i);
            }
        }
    }
    _TEST("%d disk_blocks has been mounted");
}

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
        _ERROR("Error: memory reallocation failed\n, allocate size is %d", (int)(sizeof(disk_block) * (v_disk->block_size + 1)));
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