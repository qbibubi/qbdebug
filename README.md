> God bless I don't have to write LLDB myself...

# qbdbg

`qbdbg` is a simple Linux debugger written in C using `ptrace` API. The sole idea of this project is to create a CLI debugger as a way of learning about debuggers, processors, signals, memory, breakpoints and process introspection.

## Architecture of the debugger

The debugger is split into three responsibility layers

- `cli` - handles user input, parsing of commands and display of CLI interface.
- `dbg` - handles state management of debugger context and prepares data to be forwarded to `os` layer.
- `os` - handles `ptrace` API and system calls.

CLI interface layer has no idea about the underlying OS layer. Data travels downwards through the layers to decouple them from responsibilities. This allows for cleaner and more maintainable style of the codebase.

After issuing `launch` command, the debugger uses `fork()` to spawn a child process. Child process calls `ptrace(PTRACE_TRACEME)` to allow the debugger process to trace its execution. Then the control flow is redirected to the parent where the user can send in debugger commands through a terminal interface integrated into the debugger.

The architecture currently support only Linux operating systems. I am considering adding Windows OS support for the debugger in the future.

# Features

- Reading and writing CPU registers
- Spawning and attaching to a child processes
- [ ] Setting software breakpoints
- [ ] Support for multiple commands with abbreviations
- [ ] Handling multithreaded processes

# Prerequisites

- Linux (requires `sys/ptrace.h` and ELF binaries)
- GDB `readline`
- `gcc` or `clang` compiler
- `cmake` (3.13+)

# Build

To build the project you can use the `Makefile` provided in the root of the directory to build the debugger

```bash
make
```

Alternatively you can build it manually directly with `cmake`

```bash
cmake -S . -B build
cmake --build build
```

# Usage

To run the debugger run the compiled binary in `/build` directory of the project

```bash
./build/qbdbg
```

Debugger supports `--help` and `-h` flags

```bash
qbdbg - a lightweight x86-64 debugger

USAGE
    qbdbg [options] [program [args...]]

OPTIONS
  -h, --help         Print this message and exit
  -v, --version      Print version
```

Another option is to pass binary path as command line argument with flags

```bash
# example
./build/qbdbg /usr/bin/ls -l -a
```

Once inside the debugger you should see the debugger CLI interface starting with `qbdbg>` marker before entering commands.

## List of commands

- `launch <path_to_binary> [flags...]` - Launches a new process to be debugged (does not handle passing arguments yet)
- `quit` - Exits the debugger 
- `rs <register> <value>` - Sets a register to a provided value
- `rg [(optional) registers...]` - Without any arguments all registers get displayed. It is possible to pass individual registers as arguments to get their values  
