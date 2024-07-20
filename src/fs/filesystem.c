#include "filesystem.h"

// 创建文件系统
file_system* create_file_system(virtual_disk** v_disks) {
    size_t v_disks_size = sizeof(v_disks) / sizeof(virtual_disk*);
    _TRACE("create_file_system : create fs, has %zu virtual disks", v_disks_size);
    if(v_disks_size > FS_MAX_VIRTUAL_DISKS){
        _ERROR("create_file_system : fail on creating fs, trying to mount %zu virtual disks which is larger than the limit of %d virtual disks", v_disks_size, FS_MAX_VIRTUAL_DISKS);
    }
    file_system* fs = (file_system*)malloc(sizeof(file_system));
    for(int i = 0; i < v_disks_size; i++){
        fs->v_disks[i] = v_disks[i];
        fs->v_disk_count++;
    }
    fs->root = create_entity("root", DIR_TYPE, NULL);
    _TRACE("create_file_system : success on creating fs, has %zu virtual disks", fs->v_disk_count);
    return fs;
}

// 创建文件系统实体
file_system_entity* create_entity(const char* name, file_type type, file_system_entity *parent) {
    file_system_entity* entity = (file_system_entity*)malloc(sizeof(file_system_entity));
    strncpy(entity->name, name, MAX_FILE_NAME);
    entity->type = type;
    entity->size = 0;
    entity->parent = parent;
    entity->children_count = 0;
    entity->block_count = 0;
    return entity;
}

// 添加子实体到实体
int add_entity(file_system_entity *parent, file_system_entity *child) {
    if (parent->type != DIR_TYPE) {
        _ERROR("%s is not a directory.", parent->name);
        return -1;
    }
    if (parent->children_count >= MAX_FILES) {
        _ERROR("Directory %s has reached its maximum number of files.", parent->name);
        return -1;
    }

    parent->children[parent->children_count++] = child;
    child->parent = parent;

    return parent->children_count;
}

// 查找实体
file_system_entity* find_entity(file_system_entity *parent, const char *name) {
    if (strcmp(parent->name, name) == 0) {
        return parent;
    }
    if (parent->type == DIR_TYPE) {
        for (int i = 0; i < parent->children_count; i++) {
            file_system_entity *child = parent->children[i];
            file_system_entity *res = find_entity(child, name);
            if (res != NULL) {
                return res;
            }
        }
    }
    return NULL;
}

// 打印实体
void print_entity(file_system_entity *entity, const char* prefix) {
    printf("%s/%s\n", prefix, entity->name);

    char* new_prefix = (char*)malloc(strlen(prefix) + strlen(entity->name) + 2); // +2 for '/' and '\0'
    sprintf(new_prefix, "%s/%s", prefix, entity->name);

    if (entity->type == DIR_TYPE) {
        for (int i = 0; i < entity->children_count; i++) {
            print_entity(entity->children[i], new_prefix);
        }
    }

    free(new_prefix);
}

// 打印文件系统
void print_filesystem(file_system *fs) {
    print_entity(fs->root, "");
}

// 写入文件内容
int write_file(file_system* fs, file_system_entity *file, const char *data, size_t length) {
    if (file->type != FILE_TYPE) {
        _ERROR("write_file : %s is not a file.", file->name);
        return -1;
    }

    size_t blocks_needed = (length + DISK_BLOCK_SIZE - 1) / DISK_BLOCK_SIZE;
    if (blocks_needed > MAX_FILES) {
        _ERROR("write_file : File size exceeds the maximum number of blocks.");
        return -1;
    }
    _TRACE("write_file : written data length is %zu, blocks needed is %zu", length, blocks_needed);
    file->block_count = blocks_needed;
    file->size = length;

    // 遍历所有虚拟磁盘和它们的块，找到足够的块来存储文件数据
    size_t blocks_allocated = 0;
    disk_block* allocated_blocks[blocks_needed];
    for (int vd = 0; vd < fs->v_disk_count && blocks_allocated < blocks_needed; vd++) {
        virtual_disk* v_disk = fs->v_disks[vd];
        _TRACE("virtual disk is %s, has %zu blocks", v_disk->name, v_disk->block_count);
        for (int blk = 0; blk < v_disk->block_count && blocks_allocated < blocks_needed; blk++) {
            disk_block* block = &v_disk->mounted_blocks[blk];
            _TRACE("block occupied is %d", block->occupied);
            if (block->occupied == 0) {
                allocated_blocks[blocks_allocated++] = block;
                _TRACE("write_file: found available block %zu on physical disk %zu for file %s", block->block_id, block->disk_id, file->name);
            }
        }
    }
    if (blocks_allocated < blocks_needed) {
        _ERROR("write_file: Not enough blocks available to write the file.");
        return -1;
    }

    for(int i = 0; i < blocks_allocated; i++){
        file->blocks[i] = allocated_blocks[i];
    }

    // 写入文件内容
    size_t data_pos = 0;
    for (size_t i = 0; i < blocks_allocated; i++) {
        disk_block* block = file->blocks[i];
        _TRACE("write_file: trying to write data to block %zu on physical disk %zu", block->block_id, block->disk_id);
        size_t written_bytes_len = ((length - data_pos) > DISK_BLOCK_SIZE) ? DISK_BLOCK_SIZE : length - data_pos;
        write_bytes_virtual_disk_at(block->mounted_virtual_disk_id, i * DISK_BLOCK_SIZE, data + data_pos, written_bytes_len);
        data_pos += written_bytes_len;
    }

    return length;
}

// 读取文件内容
unsigned char* read_file(file_system_entity *file) {
    if (file->type != FILE_TYPE) {
        _ERROR("read_file: %s is not a file.", file->name);
        return NULL;
    }

    size_t length = file->size;
    unsigned char *content = (unsigned char*)malloc(file->size * sizeof(unsigned char*));
    if (content == NULL) {
        _ERROR("read_file: Memory allocation failed for reading file %s.", file->name);
        return NULL;
    }

    size_t data_pos = 0;
    for (size_t i = 0; i < file->block_count; i++) {
        disk_block* block = file->blocks[i];
        size_t read_bytes_len = ((length - data_pos) > DISK_BLOCK_SIZE) ? DISK_BLOCK_SIZE : length - data_pos;
        _TRACE("read_file: trying to read data(%zu bytes) from block %zu on physical disk %zu", read_bytes_len, block->block_id, block->disk_id);
        unsigned char* data = read_bytes_virtual_disk_at(block->mounted_virtual_disk_id, i * DISK_BLOCK_SIZE, read_bytes_len);
        strcat(content, data);
    }

    return content;
}
