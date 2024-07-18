#ifndef DISK_IO_H
#define DISK_IO_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "log.h"

#define META_DATA_ROWS 2
#define BYTES_PER_VIRTUAL_BYTE 3
#define VIRTUAL_BYTES_PER_ROW 16
#define BYTES_PER_DATA_ROW (VIRTUAL_BYTES_PER_ROW * BYTES_PER_VIRTUAL_BYTE + 1)

size_t get_data_head_offset(FILE* file);

size_t get_byte_offset(FILE* file, size_t address);

void write_at(FILE* file, size_t address, unsigned char value);

void write_buffer_at(FILE* file, size_t address, const unsigned char* buffer, size_t bufferSize);

unsigned char read_at(FILE* file, size_t address);

void read_buffer_at(FILE* file, size_t address, unsigned char* buffer, size_t bufferSize);

#endif // DISK_IO_H
