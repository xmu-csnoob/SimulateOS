#ifndef VIRTUAL_DISK_H
#define VIRTUAL_DISK_H

#include "disk.h"

#define DISK_BLOCK_SIZE 64
#define MAX_VIRTUAL_DISKS 4

typedef struct {
    size_t block_id;
    size_t disk_id;
    int mounted;
} disk_block;

typedef struct {
    size_t block_count;
    disk_block blocks[];
} disk_blocks;

typedef struct {
    char* name;
    size_t block_size;
    size_t size;
    disk_block* mounted_blocks;
} virtual_disk;

extern disk_blocks* disk_blocks_table[MAX_DISKS];
extern virtual_disk* virtual_disks[MAX_VIRTUAL_DISKS];
extern int virtual_disk_count;

// functions for dividing physical disks

void init_disk_blocks();
void print_disk_blocks();

// functions for virutal blocks
int mount_disk_block(virtual_disk* v_disk, size_t disk_id, size_t block_id);
virtual_disk* create_virtual_disk(const char* name);
void free_virtual_disk(virtual_disk* v_disk);

unsigned char read_virtual_disk_at(int virtual_disk_id, size_t address);
int write_virtual_disk_at(int virtual_disk_id, size_t address, unsigned char byte);

unsigned char* read_bytes_virtual_disk_at(int virtual_disk_id, size_t address, size_t length);
int write_bytes_virtual_disk_at(int virtual_disk_id, size_t address, const unsigned char* bytes, size_t length);

#endif
