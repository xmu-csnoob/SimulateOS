// testing/test_disk.h
#ifndef TEST_DISK_H
#define TEST_DISK_H

#include <stdio.h>
#include "log.h"
#include "disk.h"
#include "virtual_disk.h"
#include "assert.h"

void test_disks();
void test_disk_io();
void test_disk_blocks();
void test_virtual_disk();
void test_virtual_disk_io();

void create_disk_file(const char *filename, const char *disk_name, size_t disk_size);

void generate_disks();


#endif // TEST_DISK_H
