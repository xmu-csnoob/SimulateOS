#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <windows.h>

// ANSI color codes
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"

#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_TRACE 4

#define LOG_LEVEL 0

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_TRACE
#endif

#define LOG_PRINT(level, color, fmt, ...) do { \
    if (level <= LOG_LEVEL) { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); \
        printf("[%s][%s:%d] " fmt "\n", #level, __FILE__, __LINE__, __VA_ARGS__); \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
    } \
} while (0)

#define _ERROR(fmt, ...) LOG_PRINT(LOG_LEVEL_ERROR, FOREGROUND_RED, fmt, __VA_ARGS__)
#define _WARN(fmt, ...) LOG_PRINT(LOG_LEVEL_WARN, FOREGROUND_RED | FOREGROUND_GREEN, fmt, __VA_ARGS__)
#define _INFO(fmt, ...) LOG_PRINT(LOG_LEVEL_INFO, FOREGROUND_GREEN, fmt, __VA_ARGS__)
#define _DEBUG(fmt, ...) LOG_PRINT(LOG_LEVEL_DEBUG, FOREGROUND_GREEN | FOREGROUND_BLUE, fmt, __VA_ARGS__)
#define _TRACE(fmt, ...) LOG_PRINT(LOG_LEVEL_TRACE, FOREGROUND_BLUE, fmt, __VA_ARGS__)

#endif // LOG_H
