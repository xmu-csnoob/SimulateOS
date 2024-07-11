#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>

#define REGISTERS_NUM 8

// #define DEBUG_PRINT

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

void init_cpu(CPU* cpu) {
    for (int i = 0; i < REGISTERS_NUM; i++) {
        cpu->registers[i] = 0;
    }
    cpu->pc = 0;
    cpu->ir = 0;
    cpu->zf = 0;
    cpu->sf = 0;
    cpu->state = CPU_RUNNING;
}

void execute_instruction(CPU* cpu, instruction* inst) {
    #ifdef DEBUG_PRINT
    printf("\n-------------cpu execute start-----------\n");
    printf("\npc = %d\n", cpu->pc);
    #endif

    switch (inst->opcode) {
        case NOP:
            break;
        case LOAD:
            #ifdef DEBUG_PRINT
            printf("\n LOAD\n");
            #endif
            cpu->registers[inst->operand1] = inst->operand2;
            break;
        case ADD:
            #ifdef DEBUG_PRINT
            printf("\n ADD \n");
            #endif
            cpu->registers[inst->operand1] = cpu->registers[inst->operand2] + cpu->registers[inst->operand3];
            break;
        case SUB:
            #ifdef DEBUG_PRINT
            printf("\n SUB \n");
            #endif
            cpu->registers[inst->operand1] = cpu->registers[inst->operand2] - cpu->registers[inst->operand3];
            break;
        case CMP:
            #ifdef DEBUG_PRINT
            printf("\n CMP \n");
            #endif
            cpu->zf = (cpu->registers[inst->operand1] == cpu->registers[inst->operand2]) ? 1 : 0;
            cpu->sf = (cpu->registers[inst->operand1] < cpu->registers[inst->operand2]) ? 1 : 0;
            break;
        case JE:
            #ifdef DEBUG_PRINT
            printf("\n JE \n");
            #endif
            if (cpu->zf == 1) {
                cpu->pc = inst->operand1;
                #ifdef DEBUG_PRINT
                printf("\n pc = %d\n", cpu->pc);
                #endif
                return;
            }
            break;
        case JNE:
            #ifdef DEBUG_PRINT
            printf("\n JNE \n");
            #endif
            if (cpu->zf != 1) {
                cpu->pc = inst->operand1;
                #ifdef DEBUG_PRINT
                printf("\n pc = %d\n", cpu->pc);
                #endif
                return;
            }
            break;
        case JUMP:
            #ifdef DEBUG_PRINT
            printf("\n JUMP \n");
            printf("\n pc = %d\n", cpu->pc);
            #endif
            cpu->pc = inst->operand1;
            return;
        case HALT:
            #ifdef DEBUG_PRINT
            printf("\n HALT \n");
            #endif
            cpu->state = CPU_HALTED;
            return;
        default:
            break;
    }
    cpu->pc++;
    #ifdef DEBUG_PRINT
    printf("\n pc = %d\n", cpu->pc);
    printf("\n-------------cpu execute end-----------\n");
    #endif
}

void print_cpu_state(CPU *cpu) {
    #ifdef DEBUG_PRINT
    printf("-------------CPU State Start--------------\n");
    printf("PC: %d\n", cpu->pc);
    printf("IR: %d\n", cpu->ir);
    printf("ZF: %d\n", cpu->zf);
    printf("SF: %d\n", cpu->sf);
    for (int i = 0; i < REGISTERS_NUM; i++) {
        printf("R%d: %d\n", i, cpu->registers[i]);
    }
    printf("State: %s\n", cpu->state == CPU_RUNNING ? "RUNNING" : "HALTED");
    printf("-------------CPU State End--------------\n");
    #endif
}

#endif
