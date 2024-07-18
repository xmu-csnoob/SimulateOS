#ifndef PCB_H
#define PCB_H
#include <stdio.h>
#include "allocate.h"
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

#endif