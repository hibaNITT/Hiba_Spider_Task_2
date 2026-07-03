

// Separating the Square-and-Multiply mathematics from your networking code

// Implementing Modular Exponentiation Helper
#include "crypto.h"

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