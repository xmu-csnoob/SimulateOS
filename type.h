#ifndef TYPE_H
#define TYPE_H

#include <stdio.h>
#include <stdlib.h>
#include "memory.h"

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
} pointer;

pointer create_pointer(size_t address, type t) {
    pointer p;
    if (address >= 0 && address < MEMORY_SIZE) {
        p.address = address;
        p.t = t;
    } else {
        p.address = 0;
        p.t = NULLTYPE;
    }
    return p;
}

void assign_pointer(pointer p, void* data) {
    if (p.address >= 0 && p.address < MEMORY_SIZE) {
        if (p.t == INTEGER) {
            if (p.address + INTEGER_BYTES <= MEMORY_SIZE) {
                unsigned char int_data[INTEGER_BYTES];
                int value = *((int*)data);
                int_data[0] = (value >> 24) & 0xFF;
                int_data[1] = (value >> 16) & 0xFF;
                int_data[2] = (value >> 8) & 0xFF;
                int_data[3] = value & 0xFF;
                assign_memory(p.address, int_data, INTEGER_BYTES);
            } else {
                printf("Warning: address out of bounds for INTEGER assignment\n");
            }
        } else if (p.t == CHAR) {
            if (p.address + CHAR_BYTES <= MEMORY_SIZE) {
                unsigned char char_data = *((unsigned char*)data);
                assign_memory(p.address, &char_data, CHAR_BYTES);
            } else {
                printf("Warning: address out of bounds for CHAR assignment\n");
            }
        } else {
            printf("Warning: invalid type for pointer assignment\n");
        }
    } else {
        printf("Warning: invalid address for pointer assignment\n");
    }
}

unsigned char* solve_pointer(pointer p) {
    unsigned char *data = NULL;
    if (p.address >= 0 && p.address < MEMORY_SIZE) {
        if (p.t == INTEGER) {
            data = (unsigned char*)malloc(INTEGER_BYTES);
            unsigned char *mem_data = access_memory(p.address);
            if (mem_data != NULL) {
                memcpy(data, mem_data, INTEGER_BYTES);
            }
        } else if (p.t == CHAR) {
            data = (unsigned char*)malloc(CHAR_BYTES);
            unsigned char *mem_data = access_memory(p.address);
            if (mem_data != NULL) {
                data[0] = mem_data[0];
            }
        }
    }
    return data;
}

unsigned int readInteger(unsigned char* data) {
    if (data == NULL) {
        printf("Warning: data is NULL\n");
        return 0;
    }
    unsigned int combined = (data[0] << 24) | (data[1] << 16) | (data[2] << 8) | data[3];
    return combined;
}

unsigned char readChar(unsigned char* data) {
    if (data == NULL) {
        printf("Warning: data is NULL\n");
        return 0;
    }
    return *data;
}

#endif
