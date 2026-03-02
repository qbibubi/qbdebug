#include "cli/cli.h"
#include "dbg/dbg.h"
#include "help.h"

int main(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            help_print();
            return 0;
        }
    }

    dbg_t dbg;
    if (dbg_init(&dbg) != DBG_OK) {
        return 1; 
    }

    using_history();
    read_history(QBDBG_HISTORY);

    while (dbg.running) {
        char *line = readline(QBDBG_FMT);
        if (!line) {
            break;
        }

        if (*line) {
            add_history(line);
        }

        dispatch_command(&dbg, line);
        free(line);
    }

    write_history(QBDBG_HISTORY);
    return 0;
}

