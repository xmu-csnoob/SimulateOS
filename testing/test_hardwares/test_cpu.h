#ifndef TEST_CPU_H
#define TEST_CPU_H

#include <assert.h>
#include "cpu.h"
#include "memory.h"

void test_cpu_init();
void test_cpu_nop();
void test_cpu_load();
void test_cpu_add();
void test_cpu_sub();
void test_cpu_cmp();
void test_cpu_jump();
void test_cpu_je();
void test_cpu_jne();
void test_cpu_jl();
void test_cpu_jg();
void test_cpu_jz();
void test_cpu_jlz();
void test_cpu_halt();
void test_cpu_mmu();

void test_cpu();

#endif // TEST_CPU_H
