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
