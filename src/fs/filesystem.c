#include "filesystem.h"

// 创建文件系统
file_system* create_file_system(virtual_disk* v_disks) {
    file_system* fs = (file_system*)malloc(sizeof(file_system));
    fs->root = create_entity("root", DIR_TYPE);
    fs->v_disks = v_disks;
    return fs;
}

// 创建文件系统实体
file_system_entity* create_entity(const char* name, file_type type) {
    file_system_entity* entity = (file_system_entity*)malloc(sizeof(file_system_entity));
    strncpy(entity->name, name, MAX_FILE_NAME);
    entity->type = type;
    entity->size = 0;
    entity->content = NULL;
    entity->parent = NULL;
    entity->children_count = 0;
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
