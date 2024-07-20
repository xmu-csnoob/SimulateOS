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
    _TRACE("-------------cpu execute start-----------");
    _TRACE("pc = %d", cpu->pc);

    switch (inst->opcode) {
        case NOP:
            break;
        case LOAD:
            _TRACE("LOAD");
            cpu->registers[inst->operand1] = inst->operand2;
            break;
        case ADD:
            _TRACE("ADD");
            cpu->registers[inst->operand1] = cpu->registers[inst->operand2] + cpu->registers[inst->operand3];
            break;
        case SUB:
            _TRACE("SUB");
            cpu->registers[inst->operand1] = cpu->registers[inst->operand2] - cpu->registers[inst->operand3];
            break;
        case CMP:
            _TRACE("CMP");
            cpu->zf = (cpu->registers[inst->operand1] == cpu->registers[inst->operand2]) ? 1 : 0;
            cpu->sf = (cpu->registers[inst->operand1] < cpu->registers[inst->operand2]) ? 1 : 0;
            break;
        case JE:
            _TRACE("JE");
            if (cpu->zf == 1) {
                cpu->pc = inst->operand1;
                _TRACE("pc = %d", cpu->pc);
                return;
            }
            break;
        case JNE:
            _TRACE("JNE");
            if (cpu->zf != 1) {
                cpu->pc = inst->operand1;
                _TRACE("pc = %d", cpu->pc);
                return;
            }
            break;
        case JL:
            _TRACE("JL");
            if (cpu->sf == 1){
                cpu->pc = inst->operand1;
                _TRACE("pc = %d", cpu->pc);
                return;
            }
            break;
        case JG:
            _TRACE("JG");
            if (cpu->sf == 0){
                cpu->pc = inst->operand1;
                _TRACE("pc = %d", cpu->pc);
                return;
            }
            break;
        case JZ:
            _TRACE("JZ");
            if(cpu->zf == 1){
                cpu->pc = inst->operand1;
                _TRACE("pc = %d", cpu->pc);
                return;
            }
            break;
        case JNZ:
            _TRACE("JNZ");
            if(cpu->zf != 1){
                cpu->pc = inst->operand1;
                _TRACE("pc = %d", cpu->pc);
                return;
            }
            break;
        case JLZ:
            _TRACE("JLZ");
            if(cpu->zf == 1 && cpu->sf == 1){
                cpu->pc = inst->operand1;
                _TRACE("pc = %d", cpu->pc);
                return;
            }
            break;
        case JUMP:
            _TRACE("JUMP");
            _TRACE("pc = %d", cpu->pc);
            cpu->pc = inst->operand1;
            return;
        case HALT:
            _TRACE("HALT");
            cpu->state = CPU_HALTED;
            return;
        default:
            break;
    }
    cpu->pc++;
    _TRACE("pc = %d", cpu->pc);
    _TRACE("-------------cpu execute end-----------");
}

void print_cpu_state(CPU *cpu) {
    _TRACE("-------------CPU State Start--------------");
    _TRACE("PC: %d", cpu->pc);
    _TRACE("IR: %d", cpu->ir);
    _TRACE("ZF: %d", cpu->zf);
    _TRACE("SF: %d", cpu->sf);
    for (int i = 0; i < REGISTERS_NUM; i++) {
        _TRACE("R%d: %d", i, cpu->registers[i]);
    }
    _TRACE("State: %s", cpu->state == CPU_RUNNING ? "RUNNING" : "HALTED");
    _TRACE("-------------CPU State End--------------");
}

size_t translate_virtual_address(size_t virtual_address){
    size_t virtual_page = virtual_address / V_PAGE_SIZE;
    size_t offset = virtual_address % V_PAGE_SIZE;
    _TRACE("translate_virtual_address : translate virutal address %zu, virtual page id is %zu, page offset is %zu", virtual_address, virtual_page, offset);

    if(virtual_page < 0 || virtual_page >= V_PAGE_NUMS){
        printf("Error: Invalid virtual address");
        return 0;
    }

    if(!page_table[virtual_page].valid){
        printf("Error: Page Not Mapped");
        return -1;
    }

    size_t physical_page = page_table[virtual_page].physical_page;
    size_t physical_address = physical_page * BLOCK_SIZE + offset;

    return physical_address;
}

BYTE* access_memory(size_t pos){
    pos = translate_virtual_address(pos);
    if(pos == -1){
        printf("Warning: trying to access unauthorized memory.");
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
        printf("Warning: trying to assign unauthorized memory.");
        return;
    }
    if(pos >= 0 && pos < MEMORY_SIZE){
        if(pos + size <= MEMORY_SIZE){
            memcpy(&memory[pos], data, size);
        }else{
            memcpy(&memory[pos], data, MEMORY_SIZE - pos);
            printf("Warning: Data truncated as it exceeds memory boundary.");
        }
    }else{
        printf("Error: Invalid memory pos for assignment.");
    }
}