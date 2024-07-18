cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(SimulateOS)

# 添加包含目录
include_directories(${CMAKE_SOURCE_DIR}/src/common ${CMAKE_SOURCE_DIR}/src/hardwares ${CMAKE_SOURCE_DIR}/src/memory ${CMAKE_SOURCE_DIR}/src/process ${CMAKE_SOURCE_DIR}/src/disk)

# 查找所有共享的源文件
file(GLOB SHARED_SOURCES
    "${CMAKE_SOURCE_DIR}/src/hardwares/*.c"
    "${CMAKE_SOURCE_DIR}/src/memory/*.c"
    "${CMAKE_SOURCE_DIR}/src/process/*.c"
    "${CMAKE_SOURCE_DIR}/src/disk/*.c"
)

# 查找各自的源文件
file(GLOB KERNEL_SOURCES
    "${CMAKE_SOURCE_DIR}/src/kernel.c"
)

file(GLOB TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/src/test.c"
)

# 创建共享库
add_library(shared_lib ${SHARED_SOURCES})

# 添加可执行文件，并链接共享库
add_executable(kernel ${KERNEL_SOURCES})
target_link_libraries(kernel shared_lib)

add_executable(test ${TEST_SOURCES})
target_link_libraries(test shared_lib)
