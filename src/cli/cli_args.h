#pragma once

#ifndef CLI_ARGS_H
#define CLI_ARGS_H

/**
 * @brief Parses arguments provided for the debugger and 
 * @param argc Argument count passed to `main` function
 * @param argv Pointer to arguments array passed to `main` function 
 * @param target_argv Pointer to target arguments variable holding a pointer to target program arguments list
 * @returns 0 on success or lack of arguemnts; 1 otherwise;
 */
int cli_parse_arguments(int argc, char** argv, char*** target_argv);

#endif // CLI_ARGS_H
