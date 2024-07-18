#ifndef VIRTUAL_DISK_H
#define VIRTUAL_DISK_H

#include "disk_block.h"

typedef struct {
    char* name;
    size_t block_size;
    size_t size;
    disk_block* mounted_blocks;
} virtual_disk;

int mount_disk_block(virtual_disk* v_disk, size_t disk_id, size_t block_id);

virtual_disk* create_virtual_disk(const char* name);

void free_virtual_disk(virtual_disk* v_disk);

#endif
