#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define MAX_INPUT_SIZE 1024

int main()
{
    char input[MAX_INPUT_SIZE];

    while (1)
    {
        // Print the prompt
        printf("octo-shell$ ");
        fflush(stdout); // Force stdout to flush since there's no newline

        // Read input
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break; // Handle Ctrl+D (EOF) gracefully
        }

        // Stripping the trailing newline character added by fgets
        // fgets() captures everything, including the \n when you press Enter.
        // Stripping it out using strcspn(input, "\n") is mandatory, otherwise strcmp(input, "exit") will compare "exit\n" to "exit" and fail!
        input[strcspn(input, "\n")] = '\0';

        // Eval & Print (Handling Built-ins)
        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting octo-shell cleanly...\n");
            break;
        }
        else if (strcmp(input, "pwd") == 0)
        {
            // A  built-in mock/system call for testing
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != NULL)
            {
                printf("%s\n", cwd);
            }
            else
            {
                perror("getcwd() error");
            }
        }
        else if (strlen(input) > 0)
        {
            // For now, printing a placeholder for unhandled external commands
            printf("octo-shell: command not found: %s\n", input);
        }
    }

    return 0;
}