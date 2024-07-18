#ifndef TYPE_H
#define TYPE_H

#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include "virtual.h"
#define INTEGER_BYTES 4
#define CHAR_BYTES 1

typedef enum {
    MY_INTEGER,
    MY_CHAR,
    NULLTYPE
} type;

typedef struct {
    size_t address;
    type t;
    size_t size;
} pointer;

pointer create_pointer(size_t address, type t, size_t size);

void assign_pointer(pointer p, void* data, size_t data_size);

pointer get_pointer(size_t address, type t, void* data, size_t data_size);

BYTE* solve_pointer(pointer p);

unsigned int readInteger(BYTE* data);

BYTE readChar(BYTE* data);

#endif
