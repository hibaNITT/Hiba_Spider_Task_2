// This isolates the tokenization logic and handles the fork(), execvp(), and waitpid() cycle like on
// on CodeCrafters.

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include "shell.h"

void tokenize_and_execute(char *input)
{

    // Array of string pointers to hold our command arguments
    char *args[64];
    int i = 0;

    // Grabbing the very first token
    char *token = strtok(input, " ");

    // Loop to extract subsequent arguments until there are no more words
    while (token != NULL && i < 63)
    {

        // Store the pointer to this word in our array
        args[i] = token;
        i++;

        // str: The string to tokenize. On the first call, you pass the full string.
        // On subsequent calls, you pass NULL to continue tokenizing the same string.
        token = strtok(NULL, " ");
    }
    args[i] = NULL; // The final element must be NULL so execvp knows where to stop

    // If the user just hit enter without typing anything, exit early
    if (args[0] == NULL)
    {
        return;
    }

    // BUILT-IN COMMANDS
    // Handling 'cd' entirely inside the parent process
    if (strcmp(args[0], "cd") == 0)
    {

        // If the user didn't specify a directory, or typed 'cd' alone
        if (args[1] == NULL)
        {
            printf("octo-shell: cd: missing argument\n");
        }

        // chdir returns -1 if the path doesn't exist or is invalid
        else if (chdir(args[1]) != 0)
        {
            perror("octo-shell: cd");
        }

        return; // No forking needed for built-ins.
    }

    // ======================== EXECUTION =====================================

    // pid_t is a data type (usually an integer) defined in <sys/types.h>.
    // It represents a process ID (PID) — a unique number assigned by the operating system to each running process.

    // Forking splits our current process into a Parent and an identical Child process
    pid_t pid = fork();

    if (pid < 0)
    {
        // perror is a standard C library function used to print a human-readable error message to stderr.
        // It’s very handy when system calls fail (like fork, open, read, etc.) because it explains why they failed.
        perror("Fork failed");
    }
    else if (pid == 0)
    {

        // execvp(args[0], args): Tells the OS to look for the system command binary named in args[0] (e.g., ls or echo),
        //  load it, and replace the child's code space with it.

        // INSIDE THE CHILD PROCESS:
        // Try to replace the child's brain with the requested program
        if (execvp(args[0], args) == -1)
        {
            printf("%s: command not found\n", args[0]);
        }
        _exit(1); // breakingthe child process if execvp fails
    }
    else
    {
        // INSIDE THE PARENT PROCESS:
        // Runs only inside the main Parent process. It forces your interactive shell interface
        // prompt to halt and wait until the child finishes executing its output.
        waitpid(pid, NULL, 0);
    }
}