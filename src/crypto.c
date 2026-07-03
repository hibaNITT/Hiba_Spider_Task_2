

// Separating the Square-and-Multiply mathematics from your networking code

// Implementing Modular Exponentiation Helper
#include "crypto.h"
#include <stdlib.h>
#include <time.h>

uint64_t modular_exponentiation(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base = base % mod; // Handle cases where base >= mod

    while (exp > 0)
    {
        // If the current bit of the exponent is 1, multiply the base with the result
        if (exp & 1)
        {
            // Use __uint128_t to prevent overflow during intermediate multiplication
            result = (uint64_t)(((__uint128_t)result * base) % mod);
        }

        // Square the base for the next bit position
        base = (uint64_t)(((__uint128_t)base * base) % mod);

        // Shift exponent right to process the next bit
        exp >>= 1;
    }
    return result;
}

#define PRIME 9973
#define GENERATOR 5

// Different every execution.
uint32_t generate_private_key(void)
{
    return (rand() % (PRIME - 2000)) + 1000;
}

// Computes g^private mod p using our existing modular exponentiation function.
uint32_t generate_public_key(uint32_t private_key)
{
    return modular_exponentiation(
        GENERATOR,
        private_key,
        PRIME);
}

// Computes received_public^private mod p Again using our existing function.
uint32_t generate_shared_secret(
    uint32_t received_public_key,
    uint32_t private_key)
{
    return modular_exponentiation(
        received_public_key,
        private_key,
        PRIME);
}

uint8_t generate_keystream_byte(uint32_t *state)
{
    *state = (*state * 1664525u) + 1013904223u;

    return (uint8_t)(*state & 0xFF);
}