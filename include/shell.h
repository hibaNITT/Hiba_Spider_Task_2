// Function prototypes & global definitions
// This file keeps our definitions clean and tells our modular files how to talk to each other.

#ifndef SHELL_H // is shell is not defined yet
#define SHELL_H

// Designing the Strict 72-Byte Packet Layout

#include <stdint.h>

// Ensure the structure matches exactly 72 bytes over the wire
struct __attribute__((packed)) RadioHeader
{
    char verification[4];  // Must be "NIT\x00"
    char filename[64];     // Target storage name
    uint32_t payload_size; // Total size of incoming file in bytes
};

// Prototypes for Level 2 commands
void handle_nittalk(char **args);

// Function Prototype: execution brain

void tokenize_and_execute(char *input);

#endif