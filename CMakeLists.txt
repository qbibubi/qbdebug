cmake_minimum_required(VERSION 3.20)

project(QBDBG VERSION 0.1 LANGUAGES C)

set(CMAKE_C_COMPILER clang)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_definitions( -DQBDBG_VERSION="${CMAKE_PROJECT_VERSION}" )

add_executable(qbdbg
    src/main.c
    src/os/os.c

    src/dbg/dbg.c
    src/dbg/dbg_breakpoint.c
    src/dbg/dbg_regs.c
    src/dbg/dbg_step.c

    src/cli/cli.c
    src/cli/cli_args.c
    src/cli/cli_breakpoint.c
    src/cli/cli_regs.c
    src/cli/cli_step.c
)

target_link_libraries(qbdbg PRIVATE
    readline
)

target_compile_options(qbdbg PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wstrict-prototypes
    -Werror=unused-result
    -g
)
