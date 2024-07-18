#include "process.h"

int create_process(size_t required_memory){
    PCB pcb;
    pcb.pid = current_pid++;
    pcb.state = READY;
    pcb_table[pcb.pid] = pcb;
    allocate_memory_first_fit(pcb.pid, required_memory);
}