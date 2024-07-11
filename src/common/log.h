#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <windows.h>

// ANSI color codes for Windows
#define ANSI_COLOR_RED 4
#define ANSI_COLOR_GREEN 2
#define ANSI_COLOR_YELLOW 6
#define ANSI_COLOR_BLUE 1
#define ANSI_COLOR_MAGENTA 5
#define ANSI_COLOR_CYAN 3
#define ANSI_COLOR_RESET 7

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_TRACE 4

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_TRACE
#endif

#define LOG_PRINT(level, color, fmt, ...) do { \
    if (level <= LOG_LEVEL) { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); \
        if (strlen(fmt) == strlen("") + 1) { \
            printf("[%s][%s:%d] " fmt "\n", #level, __FILE__, __LINE__); \
        } else { \
            printf("[%s][%s:%d] " fmt "\n", #level, __FILE__, __LINE__, __VA_ARGS__); \
        } \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
    } \
} while (0)

#define _ERROR(fmt, ...) LOG_PRINT(LOG_LEVEL_ERROR, ANSI_COLOR_RED, fmt, ##__VA_ARGS__)
#define _WARN(fmt, ...) LOG_PRINT(LOG_LEVEL_WARN, ANSI_COLOR_YELLOW, fmt, ##__VA_ARGS__)
#define _INFO(fmt, ...) LOG_PRINT(LOG_LEVEL_INFO, ANSI_COLOR_GREEN, fmt, ##__VA_ARGS__)
#define _DEBUG(fmt, ...) LOG_PRINT(LOG_LEVEL_DEBUG, ANSI_COLOR_CYAN, fmt, ##__VA_ARGS__)
#define _TRACE(fmt, ...) LOG_PRINT(LOG_LEVEL_TRACE, ANSI_COLOR_BLUE, fmt, ##__VA_ARGS__)

#endif // LOG_H
