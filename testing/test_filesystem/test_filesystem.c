#include "test_filesystem.h"

void test_filesystem() {
    virtual_disk *v_disk = create_virtual_disk("TestFileSystem");
    virtual_disk* v_disks[1];
    v_disks[0] = v_disk;
    mount_disk_block(v_disk->id, 2, 0);
    mount_disk_block(v_disk->id, 2, 1);
    _TEST("create virtual disk: %s, block num is %zu", virtual_disks[v_disk->id]->name, virtual_disks[v_disk->id]->block_count);
    file_system *fs = create_file_system(v_disks);
    _TEST("create file system, root is /%s", fs->root->name);
    file_system_entity *root = fs->root;
    assert(root != NULL);
    assert(root->type == DIR_TYPE);

    file_system_entity *dir1 = create_entity("dir1", DIR_TYPE, root);
    add_entity(root, dir1);

    file_system_entity *file1 = create_entity("file1.txt", FILE_TYPE, dir1);
    add_entity(dir1, file1);

    file_system_entity *file2 = create_entity("file2.txt", FILE_TYPE, dir1);
    add_entity(dir1, file2);

    assert(dir1->children_count == 2);

    file_system_entity *found_dir = find_entity(root, "dir1");
    assert(found_dir != NULL);
    assert(found_dir->type == DIR_TYPE);

    file_system_entity *found_file = find_entity(dir1, "file1.txt");
    assert(found_file != NULL);
    assert(found_file->type == FILE_TYPE);

    print_filesystem(fs);

    // 写入文件内容测试
    const char *data = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbcccccccccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddddddeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeffffffffffffffffffffffffffffffffffffff";
    int ret = write_file(fs, file1, data, strlen(data));
    if(ret == -1){
        _TEST("write file failed, mount more blocks to try again");
        mount_disk_block(v_disk->id, 2, 2);
        mount_disk_block(v_disk->id, 2, 3);
        write_file(fs, file1, data, strlen(data));
    }
    unsigned char *content = read_file(file1);
    _TEST("read content is %s", content);
    assert(content != NULL);


    // 释放内存
    free(content);
    free_virtual_disk(v_disk);
}
