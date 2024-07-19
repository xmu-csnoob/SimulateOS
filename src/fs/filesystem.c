#include "filesystem.h"

// 创建文件系统
file_system* create_file_system(virtual_disk* v_disks) {
    file_system* fs = (file_system*)malloc(sizeof(file_system));
    fs->root = create_entity("root", DIR_TYPE, NULL);
    fs->v_disks = v_disks;
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
int write_file(file_system_entity *file, const char *data, size_t length) {
    if (file->type != FILE_TYPE) {
        _ERROR("%s is not a file.", file->name);
        return -1;
    }

    size_t blocks_needed = (length + DISK_BLOCK_SIZE - 1) / DISK_BLOCK_SIZE;
    if (blocks_needed > MAX_FILES) {
        _ERROR("File size exceeds the maximum number of blocks.");
        return -1;
    }
    _TEST("data length is %d, blocks needed is %zu", length, blocks_needed);
    file->block_count = blocks_needed;
    file->size = length;

    // 遍历所有虚拟磁盘和它们的块，找到足够的块来存储文件数据
    size_t blocks_allocated = 0;
    for (int vd = 0; vd <= virtual_disk_count && blocks_allocated < blocks_needed; vd++) {
        _TEST("virtual disk is %s, has %zu blocks", virtual_disks[vd]->name, virtual_disks[vd]->block_count);
        for (int blk = 0; blk < virtual_disks[vd]->block_count && blocks_allocated < blocks_needed; blk++) {
            _TEST("block mounted is %d", virtual_disks[vd]->mounted_blocks[blk].mounted);
            if (virtual_disks[vd]->mounted_blocks[blk].mounted == 0) {
                file->blocks[blocks_allocated++] = &virtual_disks[vd]->mounted_blocks[blk];
            }
        }
    }

    if (blocks_allocated < blocks_needed) {
        _ERROR("Not enough blocks available to write the file.");
        return -1;
    }

    // 写入文件内容
    for (size_t i = 0; i < length; i++) {
        size_t block_id = i / DISK_BLOCK_SIZE;
        size_t block_offset = i % DISK_BLOCK_SIZE;
        write_virtual_disk_at(file->blocks[block_id]->disk_id, file->blocks[block_id]->block_id * DISK_BLOCK_SIZE + block_offset, data[i]);
    }

    return length;
}

// 读取文件内容
char* read_file(file_system_entity *file, size_t *length) {
    if (file->type != FILE_TYPE) {
        _ERROR("%s is not a file.", file->name);
        return NULL;
    }

    *length = file->size;
    char *content = (char*)malloc(file->size);
    if (content == NULL) {
        _ERROR("Memory allocation failed for reading file %s.", file->name);
        return NULL;
    }

    for (size_t i = 0; i < file->size; i++) {
        size_t block_id = i / DISK_BLOCK_SIZE;
        size_t block_offset = i % DISK_BLOCK_SIZE;
        content[i] = read_virtual_disk_at(file->blocks[block_id]->disk_id, file->blocks[block_id]->block_id * DISK_BLOCK_SIZE + block_offset);
    }

    return content;
}
