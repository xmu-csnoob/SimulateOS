#ifndef MMU_H
#define MMU_H
#include <stdio.h>
#include "blocks.h"
#include "virtual.h"
size_t translate_virtual_address(size_t virtual_address);

BYTE* access_memory(size_t pos);

void assign_memory(size_t pos, BYTE* data, size_t size);
#endif