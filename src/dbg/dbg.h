#pragma once

#ifndef DBG_H
#define DBG_H

#include <sys/user.h>
#include <sys/types.h>
#include <errno.h>
#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_BREAKPOINTS 128
#define INTERRUPT_BYTE  0xCC

typedef enum {
    DBG_OK,

    DBG_ERR_PTRACE,
    DBG_ERR_FORK,
    DBG_ERR_EXEC,
    DBG_ERR_DETACH,
    DBG_ERR_WAIT,

    DBG_ERR_CONTINUE,
    DBG_ERR_SINGLE_STEP,
    DBG_ERR_READ_MEM,
    DBG_ERR_WRITE_MEM,
    DBG_ERR_GET_REGS,
    DBG_ERR_SET_REGS,

    DBG_ERR_ATTACH,     // os_attach

    DBG_ERR_MEM_READ,   // os_read_mem 
    DBG_ERR_MEM_WRITE,  // os_write_mem
    DBG_ERR_REG_READ,   // os_get_regs
    DBG_ERR_REG_WRITE,  // os_set_regs

    DBG_ERR_NOT_RUNNING,
    DBG_ERR_ALREADY_RUNNING,
    DBG_ERR_NOT_STOPPED,

    /**
     * Breakpoints
     */
    DBG_ERR_BP_DUPLICATE,
    DBG_ERR_BP_NOT_FOUND,
    DBG_ERR_TABLE_FULL,
    DBG_ERR_BP_READ,        // os_read_mem (original byte)
    DBG_ERR_BP_WRITE,       // os_write_mem (failed writing interrupt)

    DBG_ERR_INVALID_ADDR,
    DBG_ERR_INVALID_ARG
} dbg_result_t;

typedef struct {
    uint64_t address;
    unsigned long original_byte;
    int enabled;
} breakpoint_t;

typedef struct {
    breakpoint_t table[MAX_BREAKPOINTS];
    int count;
} breakpoint_table_t;

#ifdef __x86_64

typedef struct {
    uint64_t r15, r14, r13, r12, rbp, rbx, r11, r10, r9, r8;
    uint64_t rax, rcx, rdx, rsi, rdi, orig_rax;
    uint64_t rip, cs, eflags, rsp, ss, fs_base, gs_base, ds, es, fs, gs;
} dbg_regs_t;

#else
#   error ARCHITECTURE NOT SUPPORTED
#endif // __X86_64

typedef enum {
    DBG_STOPPED,
    DBG_RUNNING,
    DBG_STEPPING,
    DBG_EXITED,
} dbg_state_t;

typedef struct dbg_t {
    pid_t pid;
    dbg_state_t state;
    breakpoint_table_t breakpoints;
    int running;
    int last_signal;
    int exit_code;
    int stepping_over_bp;
} dbg_t;

dbg_result_t dbg_init(dbg_t* dbg);
dbg_result_t dbg_quit(dbg_t* dbg);
dbg_result_t dbg_launch(dbg_t* dbg, char** argv);
dbg_result_t dbg_wait(dbg_t* dbg, const pid_t pid);
const char* dbg_result_str(dbg_result_t result);

#endif // DBG_H
