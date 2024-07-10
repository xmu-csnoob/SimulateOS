#ifndef MMU_H
#define MMU_H
#include <stdio.h>
#include "blocks.h"
#include "virtual.h"
size_t translate_virtual_address(size_t virtual_address){
    size_t virtual_page = virtual_address / V_PAGE_SIZE;
    size_t offset = virtual_address % V_PAGE_SIZE;
    if(virtual_page < 0 || virtual_page >= V_PAGE_NUMS){
        printf("Error: Invalid virtual address \n");
    }

    if(!page_table[virtual_page].valid){
        printf("Error: Page Not Mapped \n");
    }

    size_t physical_page = page_table[virtual_page].physical_page;
    size_t physical_address = physical_page * BLOCK_SIZE + offset;

    return physical_address;
}

BYTE* access_memory(size_t pos){
    pos = translate_virtual_address(pos);
    if(pos >= 0 && pos < MEMORY_SIZE){
         return &memory[pos]; 
    }
    return NULL;
}

void assign_memory(size_t pos, BYTE* data, size_t size){
    pos = translate_virtual_address(pos);
    if(pos >= 0 && pos < MEMORY_SIZE){
        if(pos + size <= MEMORY_SIZE){
            memcpy(&memory[pos], data, size);
        }else{
            memcpy(&memory[pos], data, MEMORY_SIZE - pos);
            printf("Warning: Data truncated as it exceeds memory boundary. \n");
        }
    }else{
        printf("Error: Invalid memory pos for assignment. \n");
    }
}
#endif