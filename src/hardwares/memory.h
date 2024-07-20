#ifndef MEMORY_H
#define MEMORY_H
#define MEMORY_SIZE 1024
#include <stdio.h>
#include <string.h>
#include "log.h"

typedef unsigned char BYTE;

extern BYTE memory[MEMORY_SIZE];

#define MEMORY_START_ADDRESS 0
#define MEMORY_END_ADDRESS MEMORY_SIZE - 1

void init_memory();

BYTE* access_physical_memory(size_t pos);

void assign_physical_memory(size_t pos, BYTE* data, size_t size);

void printMemory();

#endif