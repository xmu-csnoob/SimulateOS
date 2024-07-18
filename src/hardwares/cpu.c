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

size_t translate_virtual_address(size_t virtual_address){
    size_t virtual_page = virtual_address / V_PAGE_SIZE;
    size_t offset = virtual_address % V_PAGE_SIZE;
    if(virtual_page < 0 || virtual_page >= V_PAGE_NUMS){
        printf("\nError: Invalid virtual address \n");
        return 0;
    }

    if(!page_table[virtual_page].valid){
        printf("\nError: Page Not Mapped \n");
        return -1;
    }

    size_t physical_page = page_table[virtual_page].physical_page;
    size_t physical_address = physical_page * BLOCK_SIZE + offset;

    return physical_address;
}

BYTE* access_memory(size_t pos){
    pos = translate_virtual_address(pos);
    if(pos == -1){
        printf("\nWarning: trying to access unauthorized memory.\n");
        return NULL;
    }
    if(pos >= 0 && pos < MEMORY_SIZE){
         return &memory[pos]; 
    }
    return NULL;
}

void assign_memory(size_t pos, BYTE* data, size_t size){
    pos = translate_virtual_address(pos);
    if(pos == -1){
        printf("\nWarning: trying to assign unauthorized memory.\n");
        return;
    }
    if(pos >= 0 && pos < MEMORY_SIZE){
        if(pos + size <= MEMORY_SIZE){
            memcpy(&memory[pos], data, size);
        }else{
            memcpy(&memory[pos], data, MEMORY_SIZE - pos);
            printf("\nWarning: Data truncated as it exceeds memory boundary. \n");
        }
    }else{
        printf("\nError: Invalid memory pos for assignment. \n");
    }
}