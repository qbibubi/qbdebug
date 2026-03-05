#include "cli/cli.h"
#include "cli/cli_args.h"
#include "dbg/dbg.h"

int main(int argc, char* argv[]) {
    char** target_argv = NULL;

    if (cli_parse_arguments(argc, argv, &target_argv) != 0) {
        return 1;
    }

    dbg_t dbg;
    if (dbg_init(&dbg) != DBG_OK) {
        return 1; 
    }

    if (target_argv != NULL) {
        const dbg_result_t result = dbg_launch(&dbg, target_argv);
        const char* message = dbg_result_str(result);
        if (message) {
            fprintf(stderr, ERROR_FMT, message);
            return 1;
        }
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

