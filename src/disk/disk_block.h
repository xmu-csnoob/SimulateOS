#ifndef DISK_BLOCK_H
#define DISK_BLOCK_H

#include <stdio.h>
#include <stdlib.h>
#include "disk.h"

#define DISK_BLOCK_SIZE 64

typedef struct {
    size_t block_id;
    size_t disk_id;
    int mounted;
} disk_block;

typedef struct {
    size_t block_count;
    disk_block blocks[];
} disk_blocks;

extern disk_blocks* disk_blocks_table[MAX_DISKS];

void init_disk_blocks();
void printDiskBlocks();

#endif
