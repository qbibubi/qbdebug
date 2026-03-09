add_executable(qbdbg
    main.c

    os/os.c

    dbg/dbg.c
    dbg/dbg_breakpoint.c
    dbg/dbg_regs.c
    dbg/dbg_step.c

    cli/cli.c
    cli/cli_args.c
    cli/cli_breakpoint.c
    cli/cli_regs.c
    cli/cli_step.c
)

target_link_libraries(qbdbg PRIVATE
    readline
    qbdbg_warnings
)

target_compile_definitions(qbdbg PRIVATE
    $<$<CONFIG:Debug>:ENABLE_DEBUG_LOGGING>
    $<$<CONFIG:Release>:NDBEUG>
    QBDBG_VERSION="${CMAKE_PROJECT_VERSION}"
)

if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(qbdbg PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
    )
    target_link_options(qbdbg PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
    )
endif()
