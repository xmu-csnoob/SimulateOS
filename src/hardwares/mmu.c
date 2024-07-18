#include "mmu.h"

size_t translate_virtual_address(size_t virtual_address){
    size_t virtual_page = virtual_address / V_PAGE_SIZE;
    size_t offset = virtual_address % V_PAGE_SIZE;
    if(virtual_page < 0 || virtual_page >= V_PAGE_NUMS){
        printf("\nError: Invalid virtual address \n");
        return 0;
    }

    if(!page_table[virtual_page].valid){
        printf("\nError: Page Not Mapped \n");
        return -1;
    }

    size_t physical_page = page_table[virtual_page].physical_page;
    size_t physical_address = physical_page * BLOCK_SIZE + offset;

    return physical_address;
}

BYTE* access_memory(size_t pos){
    pos = translate_virtual_address(pos);
    if(pos == -1){
        printf("\nWarning: trying to access unauthorized memory.\n");
        return NULL;
    }
    if(pos >= 0 && pos < MEMORY_SIZE){
         return &memory[pos]; 
    }
    return NULL;
}

void assign_memory(size_t pos, BYTE* data, size_t size){
    pos = translate_virtual_address(pos);
    if(pos == -1){
        printf("\nWarning: trying to assign unauthorized memory.\n");
        return;
    }
    if(pos >= 0 && pos < MEMORY_SIZE){
        if(pos + size <= MEMORY_SIZE){
            memcpy(&memory[pos], data, size);
        }else{
            memcpy(&memory[pos], data, MEMORY_SIZE - pos);
            printf("\nWarning: Data truncated as it exceeds memory boundary. \n");
        }
    }else{
        printf("\nError: Invalid memory pos for assignment. \n");
    }
}