1. Memory Allocation: Stack vs. Heap
   The Stack: This is where local variables live (e.g., int x = 5;). Memory is managed automatically by the CPU. When a function finishes, its stack variables disappear instantly.

The Heap: This is a giant pool of free memory you can manually request during execution. If you need a buffer whose size changes, you ask for heap memory.

malloc() (Memory Allocation): A built-in C function used to grab chunks of memory from the Heap. It returns a pointer to the start of that block.

free(): Because the heap is manual, if you don't explicitly return memory using free(), it stays locked up forever. This is called a Memory Leak. Terrorists can use memory leaks to crash systems.

2. Pointers
   A Pointer is simply a variable that holds a memory address instead of a normal value.

If int a = 10;, then &a is the physical location in your RAM where 10 is stored. A pointer variable stores that exact address.

3. Header Files (.h) vs. Source Files (.c)
   .h (Headers): Think of these as blueprints or declarations. They tell the compiler, "Hey, a function named X exists, and it looks like this."

   .c (Source): This is where the actual muscle lives—the implementation and logic of those functions.

1. What is a REPL?REPL stands for Read-Eval-Print Loop. It is a simple infinite loop that forms the heartbeat of any interactive command-line environment: Read: Wait for the user to type a command and press Enter, then capture that string from standard input (stdin).Eval (Evaluate): Parse the string to determine what program the user wants to run and with what arguments.Print: Run the command and print the output or error messages to the screen (stdout/stderr).Loop: Start all over again, prompting the user for the next input.

1. Multi-Processing: The fork() System CallIn UNIX-like operating systems, a running program cannot simply magically start another program within its own process space without losing its identity. Instead, it must clone itself.fork(): A system call that creates an exact duplicate of the current process. The original program is called the Parent Process (your shell).The newly minted copy is called the Child Process. When fork() executes, it returns twice: it returns 0 inside the child process, and it returns the child's Process ID (PID) inside the parent process. This is how the code branches to perform different tasks.

1. Execution: The execvp() System Callexecvp(): This system call replaces the currently running process image with a brand-new program binary (like /bin/ls or /bin/pwd). When the child process calls execvp(), it completely wipes its own memory space and loads the new program. If it succeeds, the code never returns to your shell script from that child; the child turns into the executed program instead.

1. Built-in Commands vs. System CommandsThis distinction is vital for shell architecture:System Commands (External Binaries): Programs like ls, pwd, or cat are independent executable files compiled and stored in system folders like /bin. To run them, the shell must spin up a completely new process. Built-in Commands (Internal Shell Logic): Commands like cd or exit cannot be run as separate programs. If cd were an external program, it would change the directory of its own temporary process and leave your main shell exactly where it was! Therefore, built-ins are coded directly into the shell's logic using internal operating system APIs like chdir()

======================codecrafters====================================

1. The exit Builtin
   The exit builtin terminates the shell. Builtin commands are handled directly by the shell without starting a new process.

When your shell receives the exit command, it should terminate immediately.

2. The echo Builtin
   The echo builtin prints its arguments to stdout, with spaces between them, and a newline (\n) at the end.

3. The type Builtin
   The type builtin is used to determine how a command would be interpreted if it were used. It checks whether a command is a builtin, an executable file, or unrecognized.

The PATH Environment Variable
The PATH environment variable specifies a list of directories where the shell should look for executable programs.

For example, if the PATH is set to /dir1:/dir2:/dir3, the shell would search for executables in /dir1, then /dir2, and finally /dir3, in that order.

you need to actually run external programs with arguments. The simplest way is to use fork() + execvp()

=====================================================

1. Function Prototypes (The Contract)
   The C compiler compiles files from top to bottom, one .c file at a time. If main.c tries to call a function written inside execute.c, the compiler will panic because it doesn't know what parameters that function accepts or what it returns.
   A Function Prototype is a single line that acts as a promise to the compiler: "Hey, a function with this signature exists somewhere else. Let us compile for now, and the Linker will stitch us together later."

2. Header Guards
   When a project grows, multiple files might include shell.h. Without protection, the compiler will read the same file multiple times, see duplicate definitions, and fail the build. We prevent this using a Header Guard:

C
#ifndef SHELL_H
#define SHELL_H
// Content goes here
#endif
If SHELL_H isn't defined yet, the compiler reads the contents and defines it. If another file tries to include it later, #ifndef evaluates to false, skipping the file completely and preventing duplicate definitions

When you use a standard function like fgets() to read text from the terminal, it grabs everything you typed up until you hit Enter

=============================================================
we use strtok(). Here is exactly how it alters your memory:

The Target: You pass strtok your input string: "echo Hello World".

The Slicing: It looks for your delimiter (a space " "). When it finds one, it dynamically replaces that space character with a null terminator (\0).

The Result: It returns a pointer directly to the beginning of that specific word block.

By running strtok repeatedly in a loop, we can populate an array of pointers (char \*args[]) pointing to each isolated word.

strtok(input, " ");: Scans input until it hits a space, drops a \0 right there, and returns the start of the word.

strtok(NULL, " ");: Passing NULL tells strtok to remember where it left off in the previous string and continue slicing.

pid_t pid = fork();: Clones the shell. Now, two completely separate processes are executing this exact same line simultaneously.

execvp(args[0], args): Tells the OS to look for the system command binary named in args[0] (e.g., ls or echo), load it, and replace the child's code space with it.

waitpid(pid, NULL, 0);: Runs only inside the main Parent process. It forces your interactive shell interface prompt to halt and wait until the child finishes executing its output.

===================================================================
When you have a modular project with multiple .c files, you can't just run them like a script. The compiler has to translate each individual code file into machine code before it can bind them together.

Compilation Step: The compiler takes src/main.c and src/execute.c and translates them into intermediate binary blocks called object files (main.o and execute.o).

Linking Step: The linker takes those pieces, resolves the blueprints (the header file connections), stitches them together, and outputs a single executable file.

Let's compile our modular architecture using GCC.

Bash

wsl -d ubuntu first
gcc -Iinclude src/main.c src/execute.c -o octo-shell

gcc: Invokes the GNU Compiler Collection.

-Iinclude: This flag tells the compiler, "Hey, look inside the include folder to find custom header files like #include "shell.h"."

src/main.c src/execute.c: We supply both source files so the compiler can read the UI loop and the execution brain at the same time.

-o octo-shell: Specifies the output binary name. Instead of a generic a.out, it compiles directly into a file named octo-shell.

Exactly! ls -la lists all files, including hidden files (those starting with a dot .). Behind the scenes, your parent shell calls fork(), duplicating itself. The newly created child process calls execvp(), replacing its own memory space with the machine code of the /bin/ls program, while the parent shell goes to sleep using waitpid(). When ls finishes displaying the files, the child terminates, and the parent shell wakes up to show the prompt again.

===============================================================

If you type cd .. right now, your shell will try to treat it like a system command—cloning a child process and running cd inside the child.

But as we explored in our roadmap concepts: If a child process changes its working directory, it changes it ONLY for itself and then dies, leaving your main shell exactly where it started.

Therefore, commands like cd must be executed as Built-in Commands directly inside the parent process using the operating system's internal chdir() API.
