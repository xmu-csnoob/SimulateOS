// testing/test.c
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "test_disk.h"
// #include "test_hardwares.h"
// #include "test_memory.h"
// #include "test_process.h"

int main() {
    // 初始化日志级别
    init_log_level();

    // 设置日志级别为 TEST
    set_log_level("5");

    // 运行测试
    _TEST("Running hardware tests...\n");
    // test_hardwares();

    _TEST("Running memory tests...\n");
    // test_memory();

    _TEST("Running CPU tests...\n");
    // test_cpu();

    _TEST("Running disk tests...\n");
    test_disks();
    test_disk_io();
    test_disk_blocks();
    test_virtual_disk();

    _TEST("Running process tests...\n");
    // test_process();

    return 0;
}
