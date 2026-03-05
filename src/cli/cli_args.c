#include "cli_args.h"

#include <stdio.h>
#include <string.h>

#define OPTIONS_FMT "  %-18s %s\n"

typedef struct {
    const char* flags;
    const char* description;
} cli_option_t;

static const cli_option_t cli_options[] = {
    { "-h, --help",     "Print this message and exit" },
    { "-v, --version",  "Print version" },
};

static void cli_print_help(void) {
    fprintf(stdout, "qbdbg - a lightweight x86-64 debugger\n\n");
    fprintf(stdout, "USAGE\n");
    fprintf(stdout, "    qbdbg [options] [program [args...]]\n\n");

    fprintf(stdout, "OPTIONS\n"); 
    for (size_t i = 0; i < sizeof(cli_options) / sizeof(cli_options[0]); ++i) {
        fprintf(stdout, OPTIONS_FMT, cli_options[i].flags, cli_options[i].description);
    }
    fprintf(stdout, "\n");
}

static void cli_print_version(void) {
    fprintf(stdout, "qbdbg version 0.1\n"); 
}

int cli_parse_arguments(int argc, char** argv, char*** target_argv) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--") == 0) {
            if (i + 1 < argc) {
                *target_argv = &argv[i + 1];
            }

            break;
        }

        if (argv[i][0] == '-') {
            if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
                cli_print_help();
                return 1;
            }

            if (strcmp(argv[i], "--version") == 0 || strcmp(argv[i], "-v") == 0) {
                cli_print_version();
                return 1;
            }

        }
        else {
            *target_argv = &argv[i];
            return 0;
        }
    }

    return 0;
}

