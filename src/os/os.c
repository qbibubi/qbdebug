#include "os.h"

#include <errno.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/wait.h>

os_result_t os_traceme(void) {
    if (ptrace(PTRACE_TRACEME, 0, NULL, NULL) < 0) {
        return OS_ERR_PTRACE;    
    }

    return OS_OK;
}

os_result_t os_fork(pid_t *out_pid) {
    const pid_t pid = fork();
    if (pid < 0) {
        return OS_ERR_FORK;
    }

    *out_pid = pid;

    return OS_OK;
}

os_result_t os_exec(const char *path, char *const argv[]) {
    execvp(path, argv); 
    return OS_ERR_EXEC;
}

os_result_t os_detach(const pid_t pid) {
    kill(pid, SIGKILL);
    return OS_OK;
}

os_result_t os_wait(const pid_t pid, int *out_status) {
    const pid_t result = waitpid(pid, out_status, 0);
    if (result == -1) {
        return OS_ERR_WAIT;
    }

    return OS_OK;
}

os_result_t os_continue(const pid_t pid, const int signal){
    if (ptrace(PTRACE_CONT, pid, NULL, (void*)(long)signal) == -1) {
        return OS_ERR_CONTINUE; 
    }

    return OS_OK;
}

os_result_t os_single_step(const pid_t pid) {
    if (ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL) == -1) {
        return OS_ERR_SINGLE_STEP;
    }

    return OS_OK;
}

os_result_t os_read_mem(const pid_t pid, const unsigned long addr, unsigned long *out_data) {
    errno = 0;
    const long int data = ptrace(PTRACE_PEEKDATA, pid, (void*)addr, NULL);
    if (data == -1 || errno != 0) {
        return OS_ERR_READ_MEM;
    }
    
    *out_data = (unsigned long)data;
    
    return OS_OK; 
}

os_result_t os_write_mem(const pid_t pid, const unsigned long addr, const unsigned long data) {
    if (ptrace(PTRACE_POKEDATA, pid, (void*)addr, (void*)data) == -1) {
        return OS_ERR_WRITE_MEM;
    }

    return OS_OK;
}

os_result_t os_get_regs(pid_t pid, struct user_regs_struct *regs_out) {
    if (ptrace(PTRACE_GETREGS, pid, NULL, regs_out) == -1) {
        return OS_ERR_GET_REGS;
    }

    return OS_OK;
}

os_result_t os_set_regs(pid_t pid, const struct user_regs_struct *regs) {
    if (ptrace(PTRACE_SETREGS, pid, NULL, regs) == -1) {
        return OS_ERR_SET_REGS;
    }

    return OS_OK;
}
