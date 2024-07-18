#include "disk_io.h"

size_t get_data_head_offset(FILE* file) {
    fseek(file, 0, SEEK_SET);
    char buffer[256];
    int line_count = 0;
    long pos = 0;

    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_count++;
        if (line_count == META_DATA_ROWS) {
            pos = ftell(file);
            break;
        }
    }
    return pos;
}

size_t get_byte_offset(FILE* file, size_t address) {
    size_t pos = get_data_head_offset(file);
    size_t line = address / VIRTUAL_BYTES_PER_ROW;
    size_t order = address % VIRTUAL_BYTES_PER_ROW;
    pos += (line * BYTES_PER_DATA_ROW);
    pos += (order * BYTES_PER_VIRTUAL_BYTE);
    fseek(file, 0, SEEK_SET);
    return pos;
}

void write_at(FILE* file, size_t address, unsigned char value) {
    size_t pos = get_byte_offset(file, address);
    fseek(file, pos, SEEK_SET);
    char hex_value[3];
    snprintf(hex_value, sizeof(hex_value), "%02X", value);
    fputc(hex_value[0], file);
    fputc(hex_value[1], file);
    fflush(file); // 确保数据写入文件
}

void write_buffer_at(FILE* file, size_t address, const unsigned char* buffer, size_t bufferSize) {
    for (size_t i = 0; i < bufferSize; i++) {
        write_at(file, address + i, buffer[i]);
    }
}

unsigned char read_at(FILE* file, size_t address) {
    size_t pos = get_byte_offset(file, address);
    fseek(file, pos, SEEK_SET);
    char hex_byte[3] = {0};
    fread(hex_byte, 1, 2, file);
    return (unsigned char)strtol(hex_byte, NULL, 16);
}

void read_buffer_at(FILE* file, size_t address, unsigned char* buffer, size_t bufferSize) {
    for (size_t i = 0; i < bufferSize; i++) {
        buffer[i] = read_at(file, address + i);
    }
}
