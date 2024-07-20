#include "filesystem.h"

file_system* system_file_system;

// 创建文件系统
file_system* create_file_system(virtual_disk** v_disks) {
    size_t v_disks_size = sizeof(v_disks) / sizeof(virtual_disk*);
    _TRACE("create_file_system : create fs, has %zu virtual disks", v_disks_size);
    if(v_disks_size > FS_MAX_VIRTUAL_DISKS){
        _ERROR("create_file_system : fail on creating fs, trying to mount %zu virtual disks which is larger than the limit of %d virtual disks", v_disks_size, FS_MAX_VIRTUAL_DISKS);
    }
    file_system* fs = (file_system*)malloc(sizeof(file_system));
    fs->v_disk_count = 0;
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
    if(parent != NULL)
        add_entity(parent, entity);
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
    _TRACE("find_entity : find %s in %s, dir has %zu child entities.", name, parent->name, parent->children_count);

    if (parent == NULL || name == NULL || name[0] == '\0') {
        return NULL;
    }

    char path[256];
    strncpy(path, name, sizeof(path) - 1);
    path[sizeof(path) - 1] = '\0';

    char *token = strtok(path, "/");
    file_system_entity *current = parent;

    while (token != NULL) {
        _TRACE("find_entity : find %s in %s", token, current->name);

        if (strcmp(token, ".") == 0) {
            // Skip the current directory symbol
            token = strtok(NULL, "/");
            continue;
        } else if (strcmp(token, "..") == 0) {
            // Move to the parent directory
            if (current->parent != NULL) {
                current = current->parent;
            }
            token = strtok(NULL, "/");
            continue;
        }

        if (current->type != DIR_TYPE) {
            _TRACE("find_entity : %s is not a directory", current->name);
            return NULL;
        }

        int found = 0;
        for (int i = 0; i < current->children_count; i++) {
            if (strcmp(current->children[i]->name, token) == 0) {
                current = current->children[i];
                found = 1;
                break;
            }
        }

        if (!found) {
            _TRACE("find_entity : can't find %s in %s", token, current->name);
            return NULL;
        }

        token = strtok(NULL, "/");
    }

    return current;
}


// 打印实体
void print_entity(file_system_entity *entity, const char* prefix) {
    printf("%s/%s ", prefix, entity->name);

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

void print_dir(file_system_entity *dir) {
    for(int i = 0; i < dir->children_count; i++){
        printf("%s%c ", dir->children[i]->name, (dir->children[i]->type == DIR_TYPE) ? '/' : ' ');
    }
}


char* get_entity_absolute_path(file_system_entity* entity) {
    if (entity == NULL) {
        return NULL;
    }
    // Calculate the length of the resulting path
    size_t name_length = strlen(entity->name);
    size_t parent_path_length = entity->parent ? strlen(get_entity_absolute_path(entity->parent)) : 0;
    size_t total_length = parent_path_length + 1 + name_length + 1; // 1 for '/' and 1 for null terminator

    // Allocate memory for the result
    char* path = (char*)malloc(total_length * sizeof(char));
    if (path == NULL) {
        _TRACE("Memory allocation failed");
        return NULL;
    }

    // Construct the path
    path[0] = '\0'; // Initialize the path to an empty string
    if (entity->parent) {
        strcat(path, get_entity_absolute_path(entity->parent));
    }
    strcat(path, "/");
    strcat(path, entity->name);

    _TRACE("get_entity_absolute_path : path is %s", path);
    return path;
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
