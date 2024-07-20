#include "test_filesystem.h"
#include <assert.h>

void test_create_file_system() {
    _TEST("Running test_create_file_system...");

    virtual_disk *v_disk = create_virtual_disk("TestFileSystem");
    virtual_disk* v_disks[1];
    v_disks[0] = v_disk;

    file_system *fs = create_file_system(v_disks);
    _TEST("Created file system, root is /%s", fs->root->name);
    file_system_entity *root = fs->root;
    assert(root != NULL);
    assert(root->type == DIR_TYPE);

    free_virtual_disk(v_disk);
    _TEST("test_create_file_system passed.");
}

void test_add_entity() {
    _TEST("Running test_add_entity...");

    virtual_disk *v_disk = create_virtual_disk("TestAddEntity");
    virtual_disk* v_disks[1];
    v_disks[0] = v_disk;
    file_system *fs = create_file_system(v_disks);

    file_system_entity *root = fs->root;
    file_system_entity *dir1 = create_entity("dir1", DIR_TYPE, root);
    assert(dir1->parent = root);

    file_system_entity *file1 = create_entity("file1.txt", FILE_TYPE, dir1);
    assert(file1->parent = root);

    file_system_entity *file2 = create_entity("file2.txt", FILE_TYPE, dir1);
    assert(file2->parent = root);

    assert(dir1->children_count == 2);
    _TEST("Added entities to file system");

    free_virtual_disk(v_disk);
    _TEST("test_add_entity passed.");
}

void test_find_entity() {
    _TEST("Running test_find_entity...");

    virtual_disk *v_disk = create_virtual_disk("TestFindEntity");
    virtual_disk* v_disks[1];
    v_disks[0] = v_disk;
    file_system *fs = create_file_system(v_disks);

    file_system_entity *root = fs->root;
    file_system_entity *dir1 = create_entity("dir1", DIR_TYPE, root);
    assert(dir1->parent = root);

    file_system_entity *file1 = create_entity("file1.txt", FILE_TYPE, dir1);
    assert(file1->parent = root);

    file_system_entity *file2 = create_entity("file2.txt", FILE_TYPE, dir1);
    assert(file2->parent = root);

    file_system_entity *found_dir = find_entity(root, "dir1");
    assert(found_dir != NULL);
    assert(found_dir->type == DIR_TYPE);

    file_system_entity *found_file = find_entity(dir1, "file1.txt");
    assert(found_file != NULL);
    assert(found_file->type == FILE_TYPE);

    _TEST("Found entities in file system");

    free_virtual_disk(v_disk);
    _TEST("test_find_entity passed.");
}

void test_write_and_read_file() {
    _TEST("Running test_write_and_read_file...");

    virtual_disk *v_disk = create_virtual_disk("TestWriteAndReadFile");
    virtual_disk* v_disks[1];
    v_disks[0] = v_disk;
    file_system *fs = create_file_system(v_disks);

    file_system_entity *root = fs->root;
    file_system_entity *dir1 = create_entity("dir1", DIR_TYPE, root);
    assert(dir1->parent = root);

    file_system_entity *file1 = create_entity("file1.txt", FILE_TYPE, dir1);
    assert(file1->parent = root);

    // 写入文件内容测试
    const char *data = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbbcccccccccccccccccccccccccccccccccccccccdddddddddddddddddddddddddddddddddddeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeffffffffffffffffffffffffffffffffffffff";
    int ret = write_file(fs, file1, data, strlen(data));
    if (ret == -1) {
        _TEST("Write file failed, mounting more blocks to try again");
        assert(mount_disk_block(v_disk->id, 2, 0) == 1);
        assert(mount_disk_block(v_disk->id, 2, 1) == 1);
        assert(mount_disk_block(v_disk->id, 2, 2) == 1);
        assert(mount_disk_block(v_disk->id, 2, 3) == 1);
        assert(write_file(fs, file1, data, strlen(data)) != -1);
    }
    unsigned char *content = read_file(file1);
    _TEST("Read content is %s", content);
    assert(content != NULL);
    assert(strcmp((const char*)content, data) == 0);

    // 释放内存
    free(content);
    free_virtual_disk(v_disk);
    _TEST("test_write_and_read_file passed.");
}

void test_file_system() {
    _TEST("Running all filesystem tests...");
    test_create_file_system();
    test_add_entity();
    test_find_entity();
    test_write_and_read_file();
    _TEST("All filesystem tests passed.");
}
