#pragma once

#ifndef CLI_ARGS_H
#define CLI_ARGS_H

/**
 * @brief Parses arguments provided for the debugger and 
 *
 *  To distinguish between debugger and debugee arguments passed through command line arguments a `--` separator is used. 
 *  All arguments provided after this operator are debugees flags and arguments. By setting `target_argv` to next argument
 *  after the separator.
 *
 * @param argc Argument count passed to `main` function
 * @param argv Pointer to arguments array passed to `main` function 
 * @param target_argv Pointer to debugee arguments list pointer 
 * @returns 0 on success or lack of arguemnts; 1 otherwise;
 */
int cli_parse_arguments(int argc, char** argv, char*** target_argv);

#endif // CLI_ARGS_H
