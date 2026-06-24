#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
  // Flushing after every printf
  setbuf(stdout, NULL);

  while (1)
  {
    // TODO: Uncomment the code below to pass the first stage
    printf("$ ");

    char input[1000];

    // reading input
    if (!fgets(input, sizeof(input), stdin))
      break;

    // Safely remove trailing newline
    input[strcspn(input, "\n")] = '\0';

    // Split input into tokens
    char *args[100];
    int arg_count = 0;
    char *token = strtok(input, " ");
    while (token != NULL && arg_count < 99)
    {
      args[arg_count++] = token;
      token = strtok(NULL, " ");
    }
    args[arg_count] = NULL;

    if (arg_count == 0)
      continue;

    // Builtins
    if (strcmp(args[0], "exit") == 0)
    {
      break;
    }
    else if (strcmp(args[0], "echo") == 0) // 
    {
      // printing all arguments after "echo"
      for (int i = 1; i < arg_count; i++)
      {
        printf("%s%s", args[i], (i < arg_count - 1) ? " " : "");
      }
      printf("\n");
    }
    else if (strcmp(args[0], "type") == 0)
    {
      if (arg_count < 2)
      {
        printf("type: missing argument\n");
        continue;
      }

      if (strcmp(args[1], "echo") == 0 ||
          strcmp(args[1], "exit") == 0 ||
          strcmp(args[1], "type") == 0)
      {
        printf("%s is a shell builtin\n", args[1]);
      }
      else
      {
        int found = 0;

        // Search PATH
        char *path = strdup(getenv("PATH"));
        char *dir = strtok(path, ":");

        while (dir != NULL)
        {
          char fpath[1024];
          snprintf(fpath, sizeof(fpath), "%s/%s", dir, args[1]);

          if (access(fpath, X_OK) == 0) // check if executable
          {
            printf("%s is %s\n", args[1], fpath);
            found = 1;
            break;
          }
          dir = strtok(NULL, ":");
        }
        free(path);

        if (!found)
        {
          printf("%s: not found\n", args[1]);
        }
      }
    }
    else
    {
      // External program execution
      pid_t pid = fork();
      if (pid == 0)
      {
        execvp(args[0], args);
        // If execvp fails:
        printf("%s: command not found\n", args[0]);
        exit(1);
      }
      else
      {
        wait(NULL);
      }
    }
  }

  return 0;
}

// getenv("PATH") → gets the list of directories where executables live.
// strdup(...) → makes a copy so we can safely split it.
// strtok(path, ":") → splits the PATH into directories.
// snprintf(fpath, ...) → builds a full path like /bin/ls.
// access(fpath, X_OK) → checks if the file exists and is executable.
// actually run external programs when they’re found. That means instead of just printing
// "cat is /usr/bin/cat", your shell should fork a child process and use execvp() (or similar) to run the program with its arguments.
// execvp() automatically searches $PATH for the executable.
// Arguments are passed correctly (args[0] is program name, args[1..] are arguments).
// If the program exists, its own output appears in your shell.
