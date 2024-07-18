#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include "virtual.h"

#define REGISTERS_NUM 8

typedef enum {
    CPU_RUNNING,
    CPU_HALTED
} cpu_state;

typedef enum {
    NOP,
    LOAD,
    ADD,
    SUB,
    JUMP,
    CMP,
    JE,
    JNE,
    JL,
    JG,
    JZ,
    JLZ,
    HALT
} instruction_set;

typedef struct {
    instruction_set opcode;
    size_t operand1;
    size_t operand2;
    size_t operand3;
} instruction;

typedef struct {
    int registers[REGISTERS_NUM];
    int zf;
    int sf;
    int pc;
    int ir;
    cpu_state state;
} CPU;

void init_cpu(CPU* cpu);
void execute_instruction(CPU* cpu, instruction* inst);
void print_cpu_state(CPU *cpu);

//mmu
size_t translate_virtual_address(size_t virtual_address);
BYTE* access_memory(size_t pos);
void assign_memory(size_t pos, BYTE* data, size_t size);

#endif
