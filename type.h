#ifndef TYPE_H
#define TYPE_H
#include "stdio.h"
#include "memory.h"
typedef enum{
    INTERGER,
    CHAR,
    NULLTYPE
} type;

typedef struct{
    size_t address;
    type t;
} pointer;

pointer create_pointer(size_t address, type t){
    pointer p;
    if(address >=0 && address < MEMORY_SIZE){
        p.address = address;
        p.t = t;
    }else{
        p.address = NULL;
        p.t = NULLTYPE;
    }
    return p;
}