#include "allocate.h"

void allocate_memory_first_fit(size_t pid, size_t bytes) {
    size_t mapped_block[BLOCKS_NUM];
    size_t required_pages = (bytes + V_PAGE_SIZE - 1) / V_PAGE_SIZE;
    size_t pages[required_pages];

    memset(mapped_block, 0, sizeof(mapped_block));

    for (int i = 0; i < V_PAGE_NUMS; i++) {
        if (page_table[i].valid) {
            mapped_block[page_table[i].physical_page] = 1;
        }
    }

    size_t consecutive_free_blocks = 0;
    size_t start_block = 0;
    for (size_t i = 0; i < BLOCKS_NUM; i++) {
        if (mapped_block[i] == 0) {
            if (consecutive_free_blocks == 0) {
                start_block = i;
            }
            consecutive_free_blocks++;
            if (consecutive_free_blocks == required_pages) {
                break;
            }
        } else {
            consecutive_free_blocks = 0;
        }
    }

    if (consecutive_free_blocks < required_pages) {
        printf("Error: Not enough contiguous memory blocks available.");
    }
    size_t allocated_pages = 0;
    for (size_t i = 0; i < V_PAGE_NUMS && allocated_pages < required_pages; i++) {
        if (page_table[i].valid != 1) {
            size_t virtual_page = i;
            size_t physical_page = start_block + allocated_pages;
            page_table[virtual_page].physical_page = physical_page;
            pages[allocated_pages] = virtual_page;
            allocated_pages++;
            allocate_page(physical_page, virtual_page);
        }
    }
    pcb_table[pid].page_table = pages;
    pcb_table[pid].page_num = sizeof(pages) / sizeof(size_t);
}