#ifndef LOG_H
#define LOG_H

#include <stdio.h>

// 定义日志级别
#define LOG_LEVEL_ERROR 0
#define LOG_LEVEL_WARN 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_DEBUG 3
#define LOG_LEVEL_TRACE 4
#define LOG_LEVEL_TEST 5

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_TRACE
#endif

// 根据平台设置颜色
#ifdef _WIN32
#include <windows.h>
#define ANSI_COLOR_RED 4
#define ANSI_COLOR_GREEN 2
#define ANSI_COLOR_YELLOW 6
#define ANSI_COLOR_BLUE 1
#define ANSI_COLOR_MAGENTA 5
#define ANSI_COLOR_CYAN 3
#define ANSI_COLOR_RESET 7
#else
#define ANSI_COLOR_RED "\x1b[31m"
#define ANSI_COLOR_GREEN "\x1b[32m"
#define ANSI_COLOR_YELLOW "\x1b[33m"
#define ANSI_COLOR_BLUE "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN "\x1b[36m"
#define ANSI_COLOR_RESET "\x1b[0m"
#endif

// 日志打印宏
#ifdef _WIN32
#define LOG_PRINT(level, color, fmt, ...) do { \
    if (level <= LOG_LEVEL) { \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color); \
        if (fmt[0] == '\0') { \
            printf("[%s][%s:%d]\n", #level, __FILE__, __LINE__); \
        } else { \
            printf("[%s][%s:%d] " fmt "\n", #level, __FILE__, __LINE__, ##__VA_ARGS__); \
        } \
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); \
    } \
} while (0)
#else
#define LOG_PRINT(level, color, fmt, ...) do { \
    if (level <= LOG_LEVEL) { \
        printf("%s[%s][%s:%d] " fmt "%s\n", color, #level, __FILE__, __LINE__, ##__VA_ARGS__, ANSI_COLOR_RESET); \
    } \
} while (0)
#endif

// 定义具体日志级别的宏
#define _ERROR(fmt, ...) LOG_PRINT(LOG_LEVEL_ERROR, ANSI_COLOR_RED, fmt, ##__VA_ARGS__)
#define _WARN(fmt, ...) LOG_PRINT(LOG_LEVEL_WARN, ANSI_COLOR_YELLOW, fmt, ##__VA_ARGS__)
#define _INFO(fmt, ...) LOG_PRINT(LOG_LEVEL_INFO, ANSI_COLOR_GREEN, fmt, ##__VA_ARGS__)
#define _DEBUG(fmt, ...) LOG_PRINT(LOG_LEVEL_DEBUG, ANSI_COLOR_CYAN, fmt, ##__VA_ARGS__)
#define _TRACE(fmt, ...) LOG_PRINT(LOG_LEVEL_TRACE, ANSI_COLOR_BLUE, fmt, ##__VA_ARGS__)
#define _TEST(fmt, ...) LOG_PRINT(LOG_LEVEL_TEST, ANSI_COLOR_MAGENTA, fmt, ##__VA_ARGS__)

#endif // LOG_H
