#include <stdio.h>
#include "type.h"
#include "memory.h"
#include "mmu.h"
#include "allocate.h"
#include "virtual.h"
#include "process.h"
#include "cpu.h"
void print_memory_content(int start, int end);
void test_virtual_memory();
void test_process();
void test_cpu();
int main() {
    // test_virtual_memory();
    // test_process();
    test_cpu();
    return 0;
}

void print_memory_content(int start, int end) {
    for (int i = start; i < end; i++) {
        printf("memory[%d] = 0x%02X\n", i, memory[i]);
    }
    printf("\n");
}

void test_cpu(){
    CPU cpu;
    init_cpu(&cpu);
    
    instruction program[6];
    program[0] = (instruction){LOAD, 0, 5, 0};   // R0 = 5
    program[1] = (instruction){LOAD, 1, 1, 0};  // R1 = 1
    int start = 2;
    program[2] = (instruction){ADD, 2, 2, 1};    // R2 = R2 + R1
    program[3] = (instruction){CMP, 2, 0, 0};    // compare: R2 R0
    program[4] = (instruction){JNE, 2, 0, 0};    // R3 = R1 - R0
    program[5] = (instruction){JE, 6, 0, 0};    // R3 = R1 - R0
    program[6] = (instruction){HALT, 0, 0, 0};   // HALT
    int count = 0;
    while(cpu.state == CPU_RUNNING){
        count++;
        if(count > 10)
            break;
        cpu.ir = cpu.pc;
        execute_instruction(&cpu, &program[cpu.ir]);
        print_cpu_state(&cpu);
    }
}

void test_process(){
    init_memory();
    init_blocks();
    init_pages();
    create_process(80);
    print_block(0);
    print_block(1);
    print_block(2);
    printf("%d",page_table[pcb_table[0].page_table[0]].physical_page);
    printf("%d",page_table[pcb_table[0].page_table[1]].physical_page);
    printf("%d",page_table[pcb_table[0].page_table[2]].physical_page);
}

void test_virtual_memory(){
    init_memory();
    init_blocks();
    init_pages();
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