cmake_minimum_required(VERSION 3.10)

# 设置项目名称
project(SimulateOS)

# 添加包含目录
include_directories(${CMAKE_SOURCE_DIR}/src/common 
                    ${CMAKE_SOURCE_DIR}/src/hardwares 
                    ${CMAKE_SOURCE_DIR}/src/memory 
                    ${CMAKE_SOURCE_DIR}/src/process
                    ${CMAKE_SOURCE_DIR}/src/disk 
                    ${CMAKE_SOURCE_DIR}/testing)

# 查找所有源文件
file(GLOB KERNEL_SOURCES
    "${CMAKE_SOURCE_DIR}/src/kernel.c"
    "${CMAKE_SOURCE_DIR}/src/hardwares/*.c"
    "${CMAKE_SOURCE_DIR}/src/memory/*.c"
    "${CMAKE_SOURCE_DIR}/src/process/*.c"
    "${CMAKE_SOURCE_DIR}/src/disk/*.c"
    "${CMAKE_SOURCE_DIR}/src/common/log.c"
)

file(GLOB TEST_SOURCES
    "${CMAKE_SOURCE_DIR}/testing/main.c"
    "${CMAKE_SOURCE_DIR}/testing/test_disk/*.c"
    "${CMAKE_SOURCE_DIR}/testing/test_hardwares/*.c"
    "${CMAKE_SOURCE_DIR}/testing/test_memory/*.c"
    "${CMAKE_SOURCE_DIR}/testing/test_process/*.c"
    "${CMAKE_SOURCE_DIR}/src/hardwares/*.c"
    "${CMAKE_SOURCE_DIR}/src/memory/*.c"
    "${CMAKE_SOURCE_DIR}/src/process/*.c"
    "${CMAKE_SOURCE_DIR}/src/disk/*.c"
    "${CMAKE_SOURCE_DIR}/src/common/log.c"
)

# 添加可执行文件
add_executable(kernel ${KERNEL_SOURCES})
add_executable(test ${TEST_SOURCES})

# 为 test 可执行文件设置链接器选项
if (WIN32)
    target_link_options(test PRIVATE "-mconsole")
endif()
