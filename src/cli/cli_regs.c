#include "cli_regs.h"

#include "../dbg/dbg_regs.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define REGISTER_FMT            "\t%-8s 0x%016lx\n" 
#define REGISTER_FLAGS_FMT      "\t%-8s 0x%016lx\t"
#define REGISTER_ENTRY(name)    { #name, offsetof(dbg_regs_t, name) }

typedef struct {
    const char*     name;
    unsigned long   offset;
} dbg_reg_t;

static const dbg_reg_t registers[] = {
    REGISTER_ENTRY(r15),
    REGISTER_ENTRY(r14),
    REGISTER_ENTRY(r13),
    REGISTER_ENTRY(r12),
    REGISTER_ENTRY(rbp),
    REGISTER_ENTRY(rbx),
    REGISTER_ENTRY(r11),
    REGISTER_ENTRY(r10),
    REGISTER_ENTRY(r9),
    REGISTER_ENTRY(r8),
    REGISTER_ENTRY(rax),
    REGISTER_ENTRY(rcx),
    REGISTER_ENTRY(rdx),
    REGISTER_ENTRY(rsi),
    REGISTER_ENTRY(rdi),
    REGISTER_ENTRY(orig_rax),
    REGISTER_ENTRY(rip),
    REGISTER_ENTRY(cs),
    REGISTER_ENTRY(eflags),
    REGISTER_ENTRY(rsp),
    REGISTER_ENTRY(ss),
    REGISTER_ENTRY(gs_base),
    REGISTER_ENTRY(fs_base),
    REGISTER_ENTRY(ds),
    REGISTER_ENTRY(es),
    REGISTER_ENTRY(fs),
    REGISTER_ENTRY(gs),
};

typedef struct {
    const char* name;
    uint64_t    mask;
} rflags_bit_t;

static const rflags_bit_t rflags_bits[] = {
    { "CF", 0x0001 },
    { "PF", 0x0004 },
    { "AF", 0x0010 },
    { "ZF", 0x0040 },
    { "SF", 0x0080 },
    { "TF", 0x0100 },
    { "IF", 0x0200 },
    { "DF", 0x0400 },
    { "OF", 0x0800 },
};

static void cli_print_rflags(uint64_t value) {
    const size_t rflags_bits_size = sizeof(rflags_bits) / sizeof(rflags_bits[0]);
    for (size_t i = 0; i < rflags_bits_size; ++i) {
        if (value & rflags_bits[i].mask) {
            fprintf(stdout, "%s ", rflags_bits[i].name); 
        }
    }
}

static void cli_print_reg(const char* name, uint64_t value) {
    if (strcmp(name, "gs_base") == 0 || strcmp(name, "fs_base") == 0 || strcmp(name, "orig_rax") == 0) {
        return;
    }

    if (strcmp(name, "gs") == 0 && value == 0) {
        // value here has to be `gs_base` register
        fprintf(stdout, REGISTER_FMT, name, value);
        return;
    }
    
    if (strcmp(name, "fs") == 0 && value == 0) {
        // value here has to be `fs_base` register
        fprintf(stdout, REGISTER_FMT, name, value);
        return;
    }

    if (strcmp(name, "eflags") == 0) {
        fprintf(stdout, REGISTER_FLAGS_FMT, name, value);
        cli_print_rflags(value);
        fprintf(stdout, "\n"); 
        return;
    }

    fprintf(stdout, REGISTER_FMT, name, value);
}

void cli_get_regs(dbg_t* dbg, const user_input_t* input) {
    assert(dbg != NULL);
    assert(input != NULL);

    dbg_regs_t regs;

    const dbg_result_t result = dbg_get_regs(dbg, &regs); 
    const char* message = dbg_result_str(result);
    if (result != DBG_OK) {
        fprintf(stderr, ERROR_FMT, message); 
        return;
    }

    const size_t registers_size = sizeof(registers) / sizeof(registers[0]);

    if (input->argc == 0) {
        for (size_t i = 0; i < registers_size; ++i) {
            const char* name = registers[i].name;
            const uint64_t value = *(uint64_t*)((uint8_t*)&regs + registers[i].offset);

            cli_print_reg(name, value);
        }

        return;
    }

    for (size_t i = 0; i < input->argc; ++i) {
        const char* name = input->argv[i];

        int found = 0;

        for (size_t j = 0; j < registers_size; ++j) {
            if (strcmp(name, registers[j].name) != 0) {
                continue;
            }

            const uint64_t value = *(uint64_t*)((uint8_t*)&regs + registers[j].offset);

            cli_print_reg(name, value);

            found = 1;
            break;
        }

        if (!found) {
            fprintf(stderr, "unknown register: %s\n", name);
        }
    }
}

void cli_set_regs(dbg_t* dbg, const user_input_t* input) {
    assert(dbg != NULL);
    assert(input != NULL);

    if (input->argc == 0) {
        return;
    }

    dbg_regs_t regs;

    const dbg_result_t result = dbg_set_regs(dbg, &regs); 
    const char* message = dbg_result_str(result);
    if (result != DBG_OK) {
        fprintf(stderr, ERROR_FMT, message); 
    }
}
