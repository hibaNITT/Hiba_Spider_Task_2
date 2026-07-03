#include <stdio.h>
#include <string.h>
#include "shell.h"

#include <time.h>
#include <stdlib.h>

int main()
{
    srand((unsigned int)time(NULL));
    char input[2000]; // allocation of space to hold user input

    // REPL Loop
    while (1)
    {
        printf("octo-shell$ ");

        fflush(stdout); // Ensures the prompt displays immediately

        // Exit loop if something goes wrong or EOF is reached
        // Read up to 2000bytes of text from standard input (stdin)
        if (fgets(input, sizeof(input), stdin) == NULL)
        {
            break;
        }

        // removing the trailing newline character '\n' added by hitting Enter
        // strcspn scans our input string and tells us the exact position index
        //  where \n is located. We then overwrite that index with a null-terminator byte (\0),
        input[strcspn(input, "\n")] = '\0';

        // Check if the user wants to leave the shell
        if (strcmp(input, "exit") == 0)
        {
            break;
        }

        // If it's not 'exit', hand the input to our execution brain - the function prototype is mentioned in shell.h
        // this func is defined inn execute.c
        tokenize_and_execute(input);
    }

    printf("Goodbye\n");
    return 0;
}