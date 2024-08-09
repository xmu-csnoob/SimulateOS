#!/bin/bash

function mkk() {
    mkdir -p build
    cd build
    cmake ..
    make kernel
    cd ..
}

function mkt() {
    mkdir -p build
    cd build
    cmake ..
    make test
    cd ..
}

function mk() {
    mkk
    mkt
}

function mc() {
    if [ -f "kernel" ]; then
        rm kernel
    fi

    if [ -f "test" ]; then
        rm test
    fi

    if [ -d "build" ]; then
        rm -r build
    fi
}


function setenv() {
    export $1=$2
}

function lg(){
    export LOG_LEVEL=$1
}