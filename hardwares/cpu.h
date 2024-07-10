#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>

#define REGISTERS_NUM 8

typedef enum{
    CPU_RUNNING,
    CPU_HALTED
}cpu_state;

typedef enum{
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
}instruction_set;

typedef struct{
    instruction_set opcode;
    size_t operand1;
    size_t operand2;
    size_t operand3;
}instruction;

typedef struct{
    int registers[REGISTERS_NUM];
    int zf;
    int sf;
    int pc;
    int ir;
    cpu_state state;
}CPU;

void init_cpu(CPU* cpu){
    for(int i = 0; i < REGISTERS_NUM; i++){
        cpu->registers[i] = 0;
    }
    cpu->pc = 0;
    cpu->ir = 0;
    cpu->zf = 0;
    cpu->sf = 0;
    cpu->state = CPU_RUNNING;
}

void execute_instruction(CPU* cpu, instruction* instruction){
    
    switch (instruction->opcode)
    {
        case NOP:
            break;
        case LOAD:
            printf("\n LOAD\n");
            cpu->registers[instruction->operand1] = instruction->operand2;
            break;
        case ADD:
            printf("\n ADD \n");
            cpu->registers[instruction->operand1] = cpu->registers[instruction->operand2] + cpu->registers[instruction->operand3];
            break;
        case SUB:   
            printf("\n SUB \n");
            cpu->registers[instruction->operand1] = cpu->registers[instruction->operand2] - cpu->registers[instruction->operand3];
            break;
        case CMP:
            printf("\n CMP \n");
            if(instruction->operand1 == instruction->operand2){
                cpu->zf = 1;
            }else if(instruction->operand1 < instruction->operand2){
                cpu->sf = 1;
            }
            break;
        case JE:
            printf("\n JE \n");
            if(cpu->zf == 1){
                cpu->pc = instruction->operand1;
            }
            break;
        case JNE:
            printf("\n JNE \n");
            if(cpu->zf != 1){
                cpu->pc = instruction->operand1;
            }
            break;
        case JUMP:
            printf("\n JUMP \n");
            cpu->pc = instruction->operand1;
            break;
        case HALT:
            printf("\n HALT \n");
            cpu->state = CPU_HALTED;
        default:
            break;
    }
    cpu->pc++;
}

void print_cpu_state(CPU *cpu) {
    printf("-------------CPU State Start--------------:\n");
    printf("PC: %d\n", cpu->pc);
    printf("IR: %d\n", cpu->ir);
    printf("ZF: %d\n", cpu->zf);
    printf("SF: %d\n", cpu->sf);
    for (int i = 0; i < REGISTERS_NUM; i++) {
        printf("R%d: %d\n", i, cpu->registers[i]);
    }
    printf("State: %s\n", cpu->state == RUNNING ? "RUNNING" : "HALTED");
    printf("-------------CPU State End--------------:\n");
}

#endif