#include <stdio.h>
#include "type.h"
#include "memory.h"

void print_memory_content(int start, int end) {
    for (int i = start; i < end; i++) {
        printf("memory[%d] = 0x%02X\n", i, memory[i]);
    }
}

int main() {
    init_memory();

    printf("Initial memory state:\n");
    print_memory_content(0, 16); // 打印前16个字节

    int int_data = 123817;
    pointer int_ptr = create_pointer(0, INTEGER);
    assign_pointer(int_ptr, &int_data);

    char char_data = 'A';
    pointer char_ptr = create_pointer(8, CHAR);
    assign_pointer(char_ptr, &char_data);

    printf("\nMemory state after assignments:\n");
    print_memory_content(0, 16);

    unsigned char* retrieved_int_data = solve_pointer(int_ptr);
    unsigned int int_value = readInteger(retrieved_int_data);
    printf("\nRetrieved integer value: %d\n", int_value);
    free(retrieved_int_data);

    unsigned char* retrieved_char_data = solve_pointer(char_ptr);
    unsigned char char_value = readChar(retrieved_char_data);
    printf("Retrieved char value: %c\n", char_value);
    free(retrieved_char_data);

    return 0;
}
