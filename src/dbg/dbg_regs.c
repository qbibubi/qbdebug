#include "dbg_regs.h"

#include "../os/os.h"

#include <string.h>

static void regs_from_native(dbg_regs_t* out, const struct user_regs_struct* native) {
    memcpy(out, native, sizeof(dbg_regs_t));
}

static void regs_to_native(struct user_regs_struct* native, const dbg_regs_t* regs) {
    memcpy(native, regs, sizeof(struct user_regs_struct));
}

dbg_result_t dbg_get_regs(dbg_t* dbg, dbg_regs_t* out_regs) {
    assert(dbg != NULL);
    assert(out_regs != NULL);

    struct user_regs_struct native;

    if (os_get_regs(dbg->pid, &native) == OS_ERR_GET_REGS) {
        return DBG_ERR_GET_REGS;
    }

    regs_from_native(out_regs, &native);

    return DBG_OK;
}

dbg_result_t dbg_set_regs(dbg_t* dbg, dbg_regs_t* regs) {
    assert(dbg != NULL);
    assert(regs != NULL);

    struct user_regs_struct native;
    if (os_get_regs(dbg->pid, &native) == OS_ERR_GET_REGS) {
        return DBG_ERR_GET_REGS;
    }

    regs_to_native(&native, regs);
    
    if (os_set_regs(dbg->pid, &native) == OS_ERR_SET_REGS) {
        return DBG_ERR_SET_REGS;
    }

    return DBG_OK;
}
