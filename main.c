#include <stdio.h>
#include "memory/type.h"
#include "memory/memory.h"
#include "memory/mmu.h"
#include "memory/allocate.h"
#include "process.h"
void print_memory_content(int start, int end);
void test_virtual_memory();
void test_process();

int main() {
    init_memory();
    init_blocks();
    // test_virtual_memory();
    test_process();
    return 0;
}

void print_memory_content(int start, int end) {
    for (int i = start; i < end; i++) {
        printf("memory[%d] = 0x%02X\n", i, memory[i]);
    }
    printf("\n");
}

void test_process(){
    create_process(80);
    print_block(0);
    print_block(1);
    print_block(2);
    printf("%d",page_table[pcb_table[0].page_table[0]].physical_page);
    printf("%d",page_table[pcb_table[0].page_table[1]].physical_page);
    printf("%d",page_table[pcb_table[0].page_table[2]].physical_page);
}

void test_virtual_memory(){
    allocate_page(0, 0);
    allocate_page(1, 1);
    printf("Initial memory state:\n");
    print_block(0);
    print_block(1);
    int int_array[2] = {128745224, 213187245};
    pointer int_ptr = get_pointer(0, INTEGER, int_array, 2);

    char char_array[3] = {'A', 'B', 'C'};
    pointer char_ptr = get_pointer(32, CHAR, char_array, 3);

    printf("\nMemory state after assignments:\n");
    print_block(0);
    print_block(1);

    
    BYTE* retrieved_int_data = solve_pointer(int_ptr);
    for (int i = 0; i < 2; i++) {
        unsigned int int_value = readInteger(&retrieved_int_data[i * INTEGER_BYTES]);
        printf("Retrieved integer value %d: %d\n", i, int_value);
    }
    free(retrieved_int_data);

    BYTE* retrieved_char_data = solve_pointer(char_ptr);
    for (int i = 0; i < 3; i++) {
        BYTE char_value = readChar(&retrieved_char_data[i]);
        printf("Retrieved char value %d: %c\n", i, char_value);
    }
    free(retrieved_char_data);
}