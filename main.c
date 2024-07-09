#include <stdio.h>
#include "type.h"
#include "memory.h"

void print_memory_content(int start, int end) {
    for (int i = start; i < end; i++) {
        printf("memory[%d] = 0x%02X\n", i, memory[i]);
    }
    printf("\n");
}

int main() {
    // 初始化内存
    init_memory();

    // 打印初始化后的内存状态
    printf("Initial memory state:\n");
    print_memory_content(0, 16); // 打印前16个字节

    // 测试整数数组赋值
    int int_array[2] = {80, 100};
    pointer int_ptr = get_pointer(0, INTEGER, int_array, 2);

    // 测试字符数组赋值
    char char_array[3] = {'A', 'B', 'C'};
    pointer char_ptr = get_pointer(8, CHAR, char_array, 3);

    // 打印内存状态以观察写入结果
    printf("\nMemory state after assignments:\n");
    print_memory_content(0, 16);

    // 读取整数数据
    unsigned char* retrieved_int_data = solve_pointer(int_ptr);
    for (int i = 0; i < 2; i++) {
        unsigned int int_value = readInteger(&retrieved_int_data[i * INTEGER_BYTES]);
        printf("Retrieved integer value %d: %d\n", i, int_value);
    }
    free(retrieved_int_data);

    // 读取字符数据
    unsigned char* retrieved_char_data = solve_pointer(char_ptr);
    for (int i = 0; i < 3; i++) {
        unsigned char char_value = readChar(&retrieved_char_data[i]);
        printf("Retrieved char value %d: %c\n", i, char_value);
    }
    free(retrieved_char_data);

    return 0;
}
