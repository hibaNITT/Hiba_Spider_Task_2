// Function prototypes & global definitions
// This file keeps our definitions clean and tells our modular files how to talk to each other.

#ifndef SHELL_H
#define SHELL_H

#define MAX_INPUT_SIZE 2000
#define MAX_ARGS 64

// Core Execution Engine
void tokenize_and_execute(char *input);

#endif