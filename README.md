> God bless I don't have to write LLDB myself...

# qbdbg

`qbdbg` is a simple Linux debugger written in C using `ptrace` API. The sole idea of this project is to create a CLI debugger as a way of learning about debuggers, processors, signals, memory, breakpoints and process introspection.

## Architecture of the debugger

The debugger is split into three responsibility layers

- `cli` - handles user input, parsing of commands and display of CLI interface.
- `dbg` - handles state management of debugger context and prepares data to be forwarded to `os` layer.
- `os` - handles `ptrace` API and system calls.

CLI interface layer has no idea about the underlying OS layer. 
Commands and execution flow travel downwards through the layers, while state and data are bubbled back up. This unidirectional flow decouples them from responsibilities, allowing for cleaner and more maintainable style of the codebase.

After issuing `launch` command, the debugger uses `fork()` to spawn a child process. Child process calls `ptrace(PTRACE_TRACEME, ...)` to allow the debugger process to trace its execution. The control flow is redirected to the parent where the user can send in debugger commands through a terminal interface integrated into the debugger.

The architecture currently supports only Linux operating systems. Due to the projects architectural nature it allows for swapping out the `ptrace` backend with the `Win32` Debuggin API in the future to support Windows. 

# Features

- [x] Reading and writing CPU registers (x86_64 support only)
- [x] Spawning and attaching to a child processes
- [ ] Setting software breakpoints
- [ ] Support for multiple commands with abbreviations
- [ ] Handling multithreaded processes

# Prerequisites

- Linux (requires `sys/ptrace.h` and ELF binaries)
- GNU Readline libary (e.g.. `libreadline-dev`)
- C compiler (defaults to `clang`)
- `cmake` (3.20+)
- `make`

# Building the project

Before proceeding to building the project ensure you have installed all prerequisites. Build process has been simplified with a Makefile wrapper around CMake.

```bash
make run
```

## Available commands

- `make` or `make build` - Configures (if necessary) and compiles the project. Executable gets placed in `build/{BUILD_TYPE}/qbdg`.
- `make run` - Builds the project and runs the binary.
- `make configure` - Runs Cmake to generate build files without compiling the code.
- `make clean` - Deletes entire `/build` directory removing all compiled and cached files.

## Advanced configuration

It is possible to customize the build behavior by passing variables to `make` command. By default the script uses `clang` compiler and builds in `Debug` mode. 

Changing the build type is done by overriding the `BUILD_TYPE` variable:

```bash
make build BUILD_TYPE=Release
```

Changing the compiler is done by overriding the exported `CC` variable:

```bash
make build CC=gcc
```

# Usage

To run the debugger run the compiled binary in `/build/{BUILD_TYPE}` directory. 

```bash
# Debug mode
./build/Debug/qbdbg
```

Debugger supports `--help` and `-h` flags

```
qbdbg - a lightweight x86-64 debugger

USAGE
    qbdbg [options] [program [args...]]

OPTIONS
  -h, --help         Print this message and exit
  -v, --version      Print version
```

Once inside the debugger you will see the debugger CLI interface starting with `qbdbg>` prompt.

## List of commands

- `launch <path_to_binary> [flags...]` - Launches process to be debugged 
- `quit` - Exits the debugger 
- `rs <register> <value>` - Sets a register to a provided value
- `rg [(optional) registers...]` - Without any arguments all registers get displayed. It is possible to pass individual registers as arguments to get their values  
