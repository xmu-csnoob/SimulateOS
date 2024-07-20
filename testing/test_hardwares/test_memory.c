#include "test_memory.h"

void test_memory_init() {
    _TEST("Running test_memory_init...");
    init_memory();
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        assert(memory[i] == 0x00);
    }
    _TEST("test_memory_init passed.");
}

void test_memory_access_physical() {
    _TEST("Running test_memory_access_physical...");
    init_memory();
    BYTE* byte = access_physical_memory(10);
    assert(byte != NULL);
    assert(*byte == 0x00);
    _TEST("test_memory_access_physical passed.");
}

void test_memory_assign_physical() {
    _TEST("Running test_memory_assign_physical...");
    init_memory();
    BYTE data[] = {0x01, 0x02, 0x03};
    assign_physical_memory(10, data, sizeof(data));
    for (size_t i = 0; i < sizeof(data); i++) {
        assert(memory[10 + i] == data[i]);
    }
    _TEST("test_memory_assign_physical passed.");
}

void test_memory_boundary_check() {
    _TEST("Running test_memory_boundary_check...");
    init_memory();
    BYTE data[MEMORY_SIZE + 10];
    memset(data, 0xFF, sizeof(data));
    assign_physical_memory(0, data, sizeof(data));
    for (size_t i = 0; i < MEMORY_SIZE; i++) {
        assert(memory[i] == 0xFF);
    }
    _TEST("test_memory_boundary_check passed.");
}

void test_memory() {
    _TEST("Running all memory tests...");
    test_memory_init();
    test_memory_access_physical();
    test_memory_assign_physical();
    test_memory_boundary_check();
    _TEST("All memory tests passed.");
}
