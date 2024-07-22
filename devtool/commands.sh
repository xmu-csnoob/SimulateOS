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
    echo "over"
}

function mk() {
    mkk
    mkt
}

function mc(){
    rm kernel
    rm test
}

function setenv() {
    export $1=$2
}
