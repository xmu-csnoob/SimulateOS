#ifndef PCB_H
#define PCB_H
#include <stdio.h>
#include "page.h"
#define MAX_PROCESS
typedef enum{
    RUNNING,
    READY,
    BLOCKED,
    TERMINATED
} process_state;

typedef struct{
    size_t pid;
    process_state state;
    size_t page_table[PAGE_NUMS];
} PCB;

PCB pcb_table[MAX_PROCESS];
size_t current_pid = 0;
#endif