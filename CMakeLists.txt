cmake_minimum_required(VERSION 3.20)

project(QBDBG VERSION 0.1 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "Setting build type to `Debug` - no build type specified")

    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS 
        "Debug" 
        "Release" 
        "RelWithDebInfo" 
        "MinSizeRel"
    )
endif()

# Include our own modules from `/cmake`
list(APPEND CMAKE_MODULE_PATH
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake"
)
include(compiler_warnings)

enable_testing()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})

add_subdirectory(src)
