#include "test_cpu.h"

void test_cpu_init() {
    _TEST("Running test_cpu_init...");
    CPU cpu;
    init_cpu(&cpu);
    assert(cpu.pc == 0);
    assert(cpu.ir == 0);
    assert(cpu.zf == 0);
    assert(cpu.sf == 0);
    assert(cpu.state == CPU_RUNNING);
    for (int i = 0; i < REGISTERS_NUM; i++) {
        assert(cpu.registers[i] == 0);
    }
    _TEST("test_cpu_init passed.");
}

void test_cpu_nop() {
    _TEST("Running test_cpu_nop...");
    CPU cpu;
    init_cpu(&cpu);
    instruction inst = {NOP, 0, 0, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 1);
    _TEST("test_cpu_nop passed.");
}

void test_cpu_load() {
    _TEST("Running test_cpu_load...");
    CPU cpu;
    init_cpu(&cpu);
    instruction inst = {LOAD, 0, 42, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.registers[0] == 42);
    assert(cpu.pc == 1);
    _TEST("test_cpu_load passed.");
}

void test_cpu_add() {
    _TEST("Running test_cpu_add...");
    CPU cpu;
    init_cpu(&cpu);
    cpu.registers[1] = 10;
    cpu.registers[2] = 5;
    instruction inst = {ADD, 0, 1, 2};
    execute_instruction(&cpu, &inst);
    assert(cpu.registers[0] == 15);
    assert(cpu.pc == 1);
    _TEST("test_cpu_add passed.");
}

void test_cpu_sub() {
    _TEST("Running test_cpu_sub...");
    CPU cpu;
    init_cpu(&cpu);
    cpu.registers[1] = 10;
    cpu.registers[2] = 5;
    instruction inst = {SUB, 0, 1, 2};
    execute_instruction(&cpu, &inst);
    assert(cpu.registers[0] == 5);
    assert(cpu.pc == 1);
    _TEST("test_cpu_sub passed.");
}

void test_cpu_cmp() {
    _TEST("Running test_cpu_cmp...");
    CPU cpu;
    init_cpu(&cpu);
    cpu.registers[1] = 10;
    cpu.registers[2] = 10;
    instruction inst = {CMP, 1, 2, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.zf == 1);
    assert(cpu.sf == 0);

    cpu.registers[2] = 5;
    execute_instruction(&cpu, &inst);
    assert(cpu.zf == 0);
    assert(cpu.sf == 0);

    cpu.registers[2] = 15;
    execute_instruction(&cpu, &inst);
    assert(cpu.zf == 0);
    assert(cpu.sf == 1);
    _TEST("test_cpu_cmp passed.");
}

void test_cpu_jump() {
    _TEST("Running test_cpu_jump...");
    CPU cpu;
    init_cpu(&cpu);
    instruction inst = {JUMP, 5, 0, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 5);
    _TEST("test_cpu_jump passed.");
}

void test_cpu_je() {
    _TEST("Running test_cpu_je...");
    CPU cpu;
    init_cpu(&cpu);
    cpu.zf = 1;
    instruction inst = {JE, 5, 0, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 5);

    cpu.zf = 0;
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 6);
    _TEST("test_cpu_je passed.");
}

void test_cpu_jne() {
    _TEST("Running test_cpu_jne...");
    CPU cpu;
    init_cpu(&cpu);
    cpu.zf = 0;
    instruction inst = {JNE, 5, 0, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 5);

    cpu.zf = 1;
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 6);
    _TEST("test_cpu_jne passed.");
}

void test_cpu_jl() {
    _TEST("Running test_cpu_jl...");
    CPU cpu;
    init_cpu(&cpu);
    cpu.sf = 1;
    instruction inst = {JL, 5, 0, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 5);

    cpu.sf = 0;
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 6);
    _TEST("test_cpu_jl passed.");
}

void test_cpu_jg() {
    _TEST("Running test_cpu_jg...");
    CPU cpu;
    init_cpu(&cpu);
    cpu.sf = 0;
    cpu.zf = 0;
    instruction inst = {JG, 5, 0, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 5);

    cpu.sf = 1;
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 6);
    _TEST("test_cpu_jg passed.");
}

void test_cpu_jz() {
    _TEST("Running test_cpu_jz...");
    CPU cpu;
    init_cpu(&cpu);
    cpu.zf = 1;
    instruction inst = {JZ, 5, 0, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 5);

    cpu.zf = 0;
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 6);
    _TEST("test_cpu_jz passed.");
}

void test_cpu_jlz() {
    _TEST("Running test_cpu_jlz...");
    CPU cpu;
    init_cpu(&cpu);
    cpu.zf = 1;
    cpu.sf = 1;
    instruction inst = {JLZ, 5, 0, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 5);

    cpu.zf = 0;
    cpu.sf = 0;
    execute_instruction(&cpu, &inst);
    assert(cpu.pc == 6);
    _TEST("test_cpu_jlz passed.");
}

void test_cpu_halt() {
    _TEST("Running test_cpu_halt...");
    CPU cpu;
    init_cpu(&cpu);
    instruction inst = {HALT, 0, 0, 0};
    execute_instruction(&cpu, &inst);
    assert(cpu.state == CPU_HALTED);
    _TEST("test_cpu_halt passed.");
}

void test_cpu_mmu() {
    _TEST("Running test_cpu_mmu...");
    // 测试虚拟内存管理单元
    size_t address = 100;
    size_t translated_address = translate_virtual_address(address);
    _TRACE("test_cpu_mmu : translated address is %zu, actual address is %zu", translated_address, address);
    assert(translated_address == 4);

    BYTE data[10] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A};
    assign_memory(address, data, 10);

    BYTE* accessed_data = access_memory(address);
    assert(accessed_data != NULL);
    for (int i = 0; i < 10; i++) {
        assert(accessed_data[i] == data[i]);
    }
    _TEST("test_cpu_mmu passed.");
}

void test_cpu() {
    init_memory();
    init_pages();
    for(int i = 0; i < 4; i++)
        allocate_page(i, 3 - i);

    _TEST("Running CPU tests...");
    test_cpu_init();
    test_cpu_nop();
    test_cpu_load();
    test_cpu_add();
    test_cpu_sub();
    test_cpu_cmp();
    test_cpu_jump();
    test_cpu_je();
    test_cpu_jne();
    test_cpu_jl();
    test_cpu_jg();
    test_cpu_jz();
    test_cpu_jlz();
    test_cpu_halt();
    test_cpu_mmu();
    _TEST("All CPU tests passed.");
}
