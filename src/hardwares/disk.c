#include "disk.h"

disk physical_disks[MAX_DISKS]; // 定义外部变量
int current_physical_disk_size = 0; // 定义外部变量

// 验证磁盘文件并返回磁盘大小
size_t validate_disk(const char *disk_file, char *out_name) {
    FILE *file = fopen(disk_file, "r");
    if (file == NULL) {
        _ERROR("Failed to open disk file '%s'", disk_file);
        return 0; // 文件不存在
    }

    char buffer[256];
    size_t disk_size = 0;

    if (fgets(buffer, sizeof(buffer), file) != NULL && strncmp(buffer, "Disk Name: ", 11) == 0) {
        strncpy(out_name, buffer + 11, DISK_NAME_MAX_LENGTH - 1);
        out_name[DISK_NAME_MAX_LENGTH - 1] = '\0';
        char *newline = strchr(out_name, '\n');
        if (newline) {
            *newline = '\0';
        }
    } else {
        fclose(file);
        _WARN("Invalid disk file '%s': Missing disk name", disk_file);
        return (size_t)-1;
    }

    if (fgets(buffer, sizeof(buffer), file) != NULL && strncmp(buffer, "Disk Size: ", 11) == 0) {
        sscanf(buffer + 11, "%zu", &disk_size);
    } else {
        fclose(file);
        _WARN("Invalid disk file '%s': Missing disk size", disk_file);
        return (size_t)-1;
    }

    fclose(file);

    if (disk_size > 0 && strlen(out_name) > 0) {
        _INFO("Validated disk '%s' with size %zu bytes", out_name, disk_size);
        return disk_size;
    } else {
        _WARN("Invalid disk file '%s'", disk_file);
        return (size_t)-1; // 无效的磁盘文件
    }
}

// 注册磁盘文件
void register_disk(const char *disk_file) {
    if (current_physical_disk_size >= MAX_DISKS) {
        _ERROR("Cannot register disk: Maximum number of disks reached, disks size is %d", current_physical_disk_size);
        return;
    }

    char disk_name[DISK_NAME_MAX_LENGTH];
    size_t existing_size = validate_disk(disk_file, disk_name);
    if (existing_size == (size_t)-1) {
        _ERROR("Disk '%s' is invalid.", disk_file);
        return;
    } else if (existing_size == 0) {
        _ERROR("Disk '%s' does not exist.", disk_file);
        return;
    }

    physical_disks[current_physical_disk_size].size = existing_size;
    strncpy(physical_disks[current_physical_disk_size].name, disk_name, DISK_NAME_MAX_LENGTH - 1);
    physical_disks[current_physical_disk_size].name[DISK_NAME_MAX_LENGTH - 1] = '\0';

    FILE *file = fopen(disk_file, "rb+");
    if (file == NULL) {
        _ERROR("Failed to open disk file '%s' for reading and writing", disk_file);
        return;
    }

    physical_disks[current_physical_disk_size].file = file;
    current_physical_disk_size++;

    _INFO("Disk '%s' registered successfully with size %zu bytes.", disk_name, existing_size);
}

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