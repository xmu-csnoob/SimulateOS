#ifndef ALLOCATE_H
#define ALLOCATE_H

#include "blocks.h"
#include "virtual.h"
#include "pcb.h"

void allocate_memory_first_fit(size_t pid, size_t bytes);

#endif // ALLOCATE_H
