#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "shell.h"

int main()
{
    char input[MAX_INPUT_SIZE];

    while (1)
    {
        // Print custom prompt
        printf("octo-shell$ ");
        fflush(stdout);

        //         When you read input in C using fgets, pressing Ctrl+D in the terminal signals end-of-file (EOF) on stdin.
        //          That means there’s no more input to read. In practice:
        //            fgets will return NULL when EOF is reached.

        // Read input and check for EOF (Ctrl+D)
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            printf("\n");
            break;
        }

        // Clean trailing newline
        input[strcspn(input, "\n")] = '\0';

        // Skipping empty inputs
        if (strlen(input) == 0)
        {
            continue;
        }

        // Intercept fast hardcoded exit
        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting octo-shell cleanly...\n");
            break;
        }

        // Pass to our execution engine
        tokenize_and_execute(input);
    }

    return 0;
}