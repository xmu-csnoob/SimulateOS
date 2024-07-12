cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(WorkProject)

# 添加包含目录
include_directories(${CMAKE_SOURCE_DIR}/src/common ${CMAKE_SOURCE_DIR}/src/hardwares ${CMAKE_SOURCE_DIR}/src/memory ${CMAKE_SOURCE_DIR}/src/process ${CMAKE_SOURCE_DIR}/src/disk)

# 查找所有源文件
file(GLOB SOURCES
    "${CMAKE_SOURCE_DIR}/src/main.c"
    "${CMAKE_SOURCE_DIR}/src/hardwares/*.c"
    "${CMAKE_SOURCE_DIR}/src/memory/*.c"
    "${CMAKE_SOURCE_DIR}/src/process/*.c"
    "${CMAKE_SOURCE_DIR}/src/disk/*.c"
)

# 添加可执行文件
add_executable(main ${SOURCES})
