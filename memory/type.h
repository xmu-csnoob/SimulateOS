#ifndef TYPE_H
#define TYPE_H

#include <stdio.h>
#include <stdlib.h>
#include "mmu.h"
#include "virtual.h"
#define INTEGER_BYTES 4
#define CHAR_BYTES 1

typedef enum {
    INTEGER,
    CHAR,
    NULLTYPE
} type;

typedef struct {
    size_t address;
    type t;
    size_t size;
} pointer;

pointer create_pointer(size_t address, type t, size_t size) {
    pointer p;
    if (address >= 0 && address < MEMORY_SIZE) {
        p.address = address;
        p.t = t;
        p.size = size;
    } else {
        p.address = 0;
        p.t = NULLTYPE;
        p.size = 0;
    }
    return p;
}

void assign_pointer(pointer p, void* data, size_t data_size) {
    if (p.address >= 0 && p.address < V_MEMORY_SIZE) {
        if (p.t == INTEGER) {
            if (p.address + data_size * INTEGER_BYTES <= V_MEMORY_SIZE) {
                for (size_t i = 0; i < data_size; i++) {
                    BYTE int_data[INTEGER_BYTES];
                    int value = ((int*)data)[i];
                    int_data[0] = (value >> 24) & 0xFF;
                    int_data[1] = (value >> 16) & 0xFF;
                    int_data[2] = (value >> 8) & 0xFF;
                    int_data[3] = value & 0xFF;
                    assign_memory(p.address + i * INTEGER_BYTES, int_data, INTEGER_BYTES);
                }
            } else {
                printf("\nWarning: address out of bounds for INTEGER assignment\n");
            }
        } else if (p.t == CHAR) {
            if (p.address + data_size * CHAR_BYTES <= V_MEMORY_SIZE) {
                assign_memory(p.address, (BYTE*)data, data_size * CHAR_BYTES);
            } else {
                printf("\nWarning: address out of bounds for CHAR assignment\n");
            }
        } else {
            printf("\nWarning: invalid type for pointer assignment\n");
        }
    } else {
        printf("\nWarning: invalid address for pointer assignment\n");
    }
    p.size = data_size;  // Ensure size is set after assignment
}

pointer get_pointer(size_t address, type t, void* data, size_t data_size) {
    pointer p = create_pointer(address, t, data_size);
    assign_pointer(p, data, data_size);
    return p;  // Ensure pointer is returned
}

BYTE* solve_pointer(pointer p) {
    BYTE *data = NULL;
    if (p.address >= 0 && p.address < V_MEMORY_SIZE) {
        if (p.t == INTEGER) {
            data = (BYTE*)malloc(p.size * INTEGER_BYTES);
            BYTE *mem_data = access_memory(p.address);
            if (mem_data != NULL) {
                memcpy(data, mem_data, p.size * INTEGER_BYTES);
            }
        } else if (p.t == CHAR) {
            data = (BYTE*)malloc(p.size * CHAR_BYTES);
            BYTE *mem_data = access_memory(p.address);
            if (mem_data != NULL) {
                memcpy(data, mem_data, p.size * CHAR_BYTES);
            }
        }
    }
    return data;
}

unsigned int readInteger(BYTE* data) {
    if (data == NULL) {
        printf("\nWarning: data is NULL\n");
        return 0;
    }
    unsigned int combined = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    return combined;
}

BYTE readChar(BYTE* data) {
    if (data == NULL) {
        printf("\nWarning: data is NULL\n");
        return 0;
    }
    return *data;
}

#endif
