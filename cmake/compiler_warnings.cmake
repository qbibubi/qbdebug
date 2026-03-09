add_library(qbdbg_warnings INTERFACE)

target_compile_options(qbdbg_warnings INTERFACE
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wstrict-prototypes
)

if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(qbdbg_warnings INTERFACE
        -Wformat=2
        -Wconversion
    )
endif()
