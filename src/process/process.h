#ifndef PROCESS_H
#define PROCESS_H
#include <stdio.h>
#include "virtual.h"

#define MAX_PROCESS 16

typedef enum{
    RUNNING,
    READY,
    BLOCKED,
    TERMINATED
} process_state;

typedef struct{
    size_t pid;
    process_state state;
    size_t* page_table;
    size_t page_num;
} PCB;

extern PCB pcb_table[MAX_PROCESS];
extern size_t current_pid;

int create_process(size_t required_memory);

#endif