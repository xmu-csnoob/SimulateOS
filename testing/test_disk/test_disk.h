#ifndef TEST_DISK_H
#define TEST_DISK_H

#include <assert.h>
#include "log.h"
#include "disk.h"
#include "virtual_disk.h"

void test_register_disks();
void test_disk_io();
void test_virtual_disk();
void test_virtual_disk_io();
void generate_disks();

void test_disks();

#endif // TEST_DISK_H
