#include "cpu.h"

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
    _TRACE("\n-------------cpu execute start-----------\n");
    _TRACE("\npc = %d\n", cpu->pc);

    switch (inst->opcode) {
        case NOP:
            break;
        case LOAD:
            _TRACE("\n LOAD\n");
            cpu->registers[inst->operand1] = inst->operand2;
            break;
        case ADD:
            _TRACE("\n ADD \n");
            cpu->registers[inst->operand1] = cpu->registers[inst->operand2] + cpu->registers[inst->operand3];
            break;
        case SUB:
            _TRACE("\n SUB \n");
            cpu->registers[inst->operand1] = cpu->registers[inst->operand2] - cpu->registers[inst->operand3];
            break;
        case CMP:
            _TRACE("\n CMP \n");
            cpu->zf = (cpu->registers[inst->operand1] == cpu->registers[inst->operand2]) ? 1 : 0;
            cpu->sf = (cpu->registers[inst->operand1] < cpu->registers[inst->operand2]) ? 1 : 0;
            break;
        case JE:
            _TRACE("\n JE \n");
            if (cpu->zf == 1) {
                cpu->pc = inst->operand1;
                _TRACE("\n pc = %d\n", cpu->pc);
                return;
            }
            break;
        case JNE:
            _TRACE("\n JNE \n");
            if (cpu->zf != 1) {
                cpu->pc = inst->operand1;
                _TRACE("\n pc = %d\n", cpu->pc);
                return;
            }
            break;
        case JUMP:
            _TRACE("\n JUMP \n");
            _TRACE("\n pc = %d\n", cpu->pc);
            cpu->pc = inst->operand1;
            return;
        case HALT:
            _TRACE("\n HALT \n");
            cpu->state = CPU_HALTED;
            return;
        default:
            break;
    }
    cpu->pc++;
    _TRACE("\n pc = %d\n", cpu->pc);
    _TRACE("\n-------------cpu execute end-----------\n");
}

void print_cpu_state(CPU *cpu) {
    _TRACE("-------------CPU State Start--------------\n");
    _TRACE("PC: %d\n", cpu->pc);
    _TRACE("IR: %d\n", cpu->ir);
    _TRACE("ZF: %d\n", cpu->zf);
    _TRACE("SF: %d\n", cpu->sf);
    for (int i = 0; i < REGISTERS_NUM; i++) {
        _TRACE("R%d: %d\n", i, cpu->registers[i]);
    }
    _TRACE("State: %s\n", cpu->state == CPU_RUNNING ? "RUNNING" : "HALTED");
    _TRACE("-------------CPU State End--------------\n");
}