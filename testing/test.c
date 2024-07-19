// testing/test.c
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "test_disk.h"
#include "test_filesystem.h"
// #include "test_hardwares.h"
// #include "test_memory.h"
// #include "test_process.h"

#ifdef _WIN32
void set_log_level(const char *level) {
    char env_str[50];
    snprintf(env_str, sizeof(env_str), "LOG_LEVEL=%s", level);
    _putenv(env_str);
}
#else
void set_log_level(const char *level) {
    setenv("LOG_LEVEL", level, 1);
}
#endif

int main() {
    // 初始化日志级别

    set_log_level("-1");

    init_log_level();

    // 运行测试
    _TEST("Running hardware tests...");
    // test_hardwares();

    _TEST("Running memory tests...");
    // test_memory();

    _TEST("Running CPU tests...");
    // test_cpu();

    _TEST("Running disk tests...");
    generate_disks();
    test_disks();
    test_disk_io();
    test_disk_blocks();
    test_virtual_disk();
    test_virtual_disk_io();

    _TEST("Running filesystem tests...");
    test_filesystem();

    _TEST("Running process tests...");
    // test_process();

    return 0;
}
