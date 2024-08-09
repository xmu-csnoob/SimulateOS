#!/bin/bash

# 编译 Debug 版本
function mkd() {
    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    make ${1:+$1}  # 如果有参数传入则make该参数，否则不传参数直接make
    cd ..
}

# 编译 Release 版本
function mkr() {
    mkdir -p build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Release ..
    make ${1:+$1}  # 如果有参数传入则make该参数，否则不传参数直接make
    cd ..
}

# 清理构建目录并删除当前目录下的所有可执行文件
function mc() {
    # 删除build目录
    if [ -d "build" ]; then
        rm -rf build
    fi

    # 删除当前目录下的所有可执行文件
    find . -maxdepth 1 -type f -executable -exec rm -f {} \;
}

# 设置环境变量
function setenv() {
    export $1=$2
}

# 设置日志级别
function lg(){
    export LOG_LEVEL=$1
}
