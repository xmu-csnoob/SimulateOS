// testing/test.c
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "test_disk.h"
#include "test_filesystem.h"
#include "test_hardwares.h"
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
    init_log_level();
    generate_disks();
    // 运行测试
    _TEST("Running hardware tests...");
    test_hardwares();

    _TEST("Running disk tests...");
    test_disks();

    _TEST("Running filesystem tests...");
    test_file_system();

    _TEST("Running process tests...");
    // test_process();

    return 0;
}
