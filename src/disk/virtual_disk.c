#include "virtual_disk.h"

disk_blocks* disk_blocks_table[MAX_DISKS];

virtual_disk* virtual_disks[MAX_VIRTUAL_DISKS];

int virtual_disk_count = 0;

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
            t->blocks[j].occupied = 0;
            t->blocks[j].mounted_virtual_disk_id = -1;
        }
        disk_blocks_table[i] = t;
    }
}

void print_disk_blocks() {
    int mounted_size = 0;
    for (int i = 0; i < MAX_DISKS; i++) {
        disk_blocks *dbs = disk_blocks_table[i];
        size_t blocks = dbs->block_count;
        _TRACE("Disk id: %d, has %zu blocks in total.", i, blocks);
        for (int j = 0; j < blocks; j++) {
            disk_block db = dbs->blocks[j];
            _TRACE("Block id %d, %s", j, db.mounted == 0 ? "has not been mounted." : "has been mounted.");
            if(db.mounted == 1){
                mounted_size++;
                _TRACE("Block id %d is mounted to virtual disk %d", j, i);
            }
        }
    }
    _TRACE("%d disk_blocks has been mounted", mounted_size);
}

int mount_disk_block(size_t v_disk_id, size_t disk_id, size_t block_id) {
    virtual_disk* v_disk = virtual_disks[v_disk_id];
    disk_block db = disk_blocks_table[disk_id]->blocks[block_id];
    if (db.mounted == 1) {
        _ERROR("Error: trying to mount an already mounted block at disk %zu, block %zu\n", disk_id, block_id);
        return 0;
    }
    disk_blocks_table[disk_id]->blocks[block_id].mounted = 1;

    // Reallocate memory to accommodate the new block
    v_disk->mounted_blocks = (disk_block*)realloc(v_disk->mounted_blocks, sizeof(disk_block) * (v_disk->block_count + 1));
    if (v_disk->mounted_blocks == NULL) {
        _ERROR("Error: memory reallocation failed\n, allocate size is %d", (int)(sizeof(disk_block) * (v_disk->block_count + 1)));
        return -1;
    }
    
    v_disk->mounted_blocks[v_disk->block_count] = db;
    v_disk->size += DISK_BLOCK_SIZE;
    v_disk->mounted_blocks[v_disk->block_count].mounted = 1;
    v_disk->mounted_blocks[v_disk->block_count].mounted_virtual_disk_id = v_disk_id;
    v_disk->block_count++;
    _TRACE("mount disk %zu block %zu to virtual disk %d", disk_id, block_id, virtual_disk_count);
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
    v_disk->block_count = 0;
    v_disk->size = 0;
    v_disk->mounted_blocks = NULL;
    v_disk->id = virtual_disk_count;
    virtual_disks[virtual_disk_count++] = v_disk;
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

unsigned char read_virtual_disk_at(int virtual_disk_id, size_t address) {
    virtual_disk* disk = virtual_disks[virtual_disk_id];
    int block_id = address / DISK_BLOCK_SIZE;
    int block_offset = address % DISK_BLOCK_SIZE;
    disk_block* db = &disk->mounted_blocks[block_id];
    size_t disk_offset = db->block_id * DISK_BLOCK_SIZE + block_offset;
    unsigned char data = read_at(physical_disks[db->disk_id].file, disk_offset);
    return data;
}

int write_virtual_disk_at(int virtual_disk_id, size_t address, unsigned char byte) {
    _TRACE("write_virtual_disk_at : trying to write %hx to virtual disk %d at address %zu", byte, virtual_disk_id, address);
    virtual_disk* disk = virtual_disks[virtual_disk_id];
    if(virtual_disk_id < 0 || virtual_disk_id >= MAX_VIRTUAL_DISKS){
        _ERROR("write_virtual_disk_at : virtual disk id %d is not a valid value.", virtual_disk_id);
        return -1;
    }
    if(disk == NULL){
        _ERROR("write_virtual_disk_at : can not find virtual disk %d.", virtual_disk_id);
        return 0;
    }
    _TRACE("write_virtual_disk_at : find virtual disk %zu : %s, has %zu blocks", disk->id, disk->name, disk->block_count);
    int block_id = address / DISK_BLOCK_SIZE;
    int block_offset = address % DISK_BLOCK_SIZE;
    disk_block* db = &disk->mounted_blocks[block_id];
    size_t disk_offset = db->block_id * DISK_BLOCK_SIZE + block_offset;
    write_at(physical_disks[db->disk_id].file, disk_offset, byte);
    return 1;
}

unsigned char* read_bytes_in_block(int virtual_disk_id, int block_id, size_t address, size_t length){
    virtual_disk* v_disk = virtual_disks[virtual_disk_id];
    disk_block* db = &v_disk->mounted_blocks[block_id];
    if(address + length >= DISK_BLOCK_SIZE){
        _ERROR("read block address out of bound : %zu", address + length);
    }
    size_t offset = block_id * DISK_BLOCK_SIZE + length;
    unsigned char* data;
    read_buffer_at(physical_disks[db->disk_id].file, offset, data, length);
    return data;
}

unsigned char* read_bytes_virtual_disk_at(int virtual_disk_id, size_t address, size_t length) {
    _TRACE("read_bytes_virtual_disk_at : trying to read %zu bytes on address %zu from virtual disk %s.", length, address, virtual_disks[virtual_disk_id]->name);
    unsigned char *data = (unsigned char *)malloc(length * sizeof(unsigned char*));
    if (!data) {
        _ERROR("Memory allocation failed.");
        return NULL;
    }

    size_t bytes_read = 0;
    virtual_disk* disk = virtual_disks[virtual_disk_id];

    while (bytes_read < length) {
        size_t block_id = (address + bytes_read) / DISK_BLOCK_SIZE;
        size_t block_offset = (address + bytes_read) % DISK_BLOCK_SIZE;
        size_t bytes_to_read = (length >= DISK_BLOCK_SIZE) ? DISK_BLOCK_SIZE - block_offset : length;
        disk_block* db = &disk->mounted_blocks[block_id];
        size_t disk_offset = db->block_id * DISK_BLOCK_SIZE + block_offset;

        unsigned char* buffer = (unsigned char*)malloc(bytes_to_read * sizeof(unsigned char *));
        read_buffer_at(physical_disks[db->disk_id].file, disk_offset, buffer, bytes_to_read);
        memcpy(data + bytes_read, buffer, bytes_to_read);
        bytes_read += bytes_to_read;
    }
    _TRACE("read_bytes_virtual_disk_at : success on reading %zu bytes on address %zu from virtual disk %s, data is %s", length, address, virtual_disks[virtual_disk_id]->name, data);
    return data;
}

int write_bytes_virtual_disk_at(int virtual_disk_id, size_t address, const unsigned char *bytes, size_t length) {
    size_t bytes_written = 0;
    virtual_disk* disk = virtual_disks[virtual_disk_id];

    while (bytes_written < length) {
        size_t block_id = (address + bytes_written) / DISK_BLOCK_SIZE;
        size_t block_offset = (address + bytes_written) % DISK_BLOCK_SIZE;
        size_t bytes_to_write = (length - bytes_written > DISK_BLOCK_SIZE - block_offset) ? DISK_BLOCK_SIZE - block_offset : length - bytes_written;

        disk_block* db = &disk->mounted_blocks[block_id];
        size_t disk_offset = db->block_id * DISK_BLOCK_SIZE + block_offset;

        write_buffer_at(physical_disks[db->disk_id].file, disk_offset, bytes + bytes_written, bytes_to_write);

        bytes_written += bytes_to_write;
    }

    return 0;
}

