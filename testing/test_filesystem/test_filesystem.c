#include "test_filesystem.h"

void test_filesystem() {
    virtual_disk *v_disk = create_virtual_disk("TestVirtualDisk");
    file_system *fs = create_file_system(v_disk);
    
    file_system_entity *root = fs->root;
    assert(root != NULL);
    assert(root->type == DIR_TYPE);
    
    file_system_entity *dir1 = create_entity("dir1", DIR_TYPE);
    add_entity(root, dir1);
    
    file_system_entity *file1 = create_entity("file1.txt", FILE_TYPE);
    add_entity(dir1, file1);

    file_system_entity *file2 = create_entity("file2.txt", FILE_TYPE);
    add_entity(dir1, file2);
    
    assert(dir1->children_count == 2);
    
    file_system_entity *found_dir = find_entity(root, "dir1");
    assert(found_dir != NULL);
    assert(found_dir->type == DIR_TYPE);
    
    file_system_entity *found_file = find_entity(dir1, "file1.txt");
    assert(found_file != NULL);
    assert(found_file->type == FILE_TYPE);
    
    print_filesystem(fs);
}

