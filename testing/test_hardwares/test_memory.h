#ifndef TEST_MEMORY_H
#define TEST_MEMORY_H

#include <assert.h>
#include "memory.h"
#include "log.h"

void test_memory_init();
void test_memory_access_physical();
void test_memory_assign_physical();
void test_memory_boundary_check();
void test_memory();

#endif // TEST_MEMORY_H
