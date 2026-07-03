// By defining the global public parameters (DH_PRIME and DH_GENERATOR) and the function prototype for modular_exponentiation inside crypto.h,
//  you allow any other file (like src/nittalk.c) to use these cryptographic parameters simply by adding #include "crypto.h". The other files don't need to know how the math works; they just need to know it exists.

#ifndef CRYPTO_H
#define CRYPTO_H

#include <stdint.h>
#include <stddef.h>

// Global Public Parameters for Diffie-Hellman Key Exchange
#define DH_PRIME 9973  // A small prime for demonstration; normally much larger
#define DH_GENERATOR 5 // Generator base 'g'

/**
 * @brief Computes (base^exp) % mod safely without 64-bit integer overflow.
 * Uses the Square-and-Multiply (Binary Exponentiation) method.
 */
uint64_t modular_exponentiation(uint64_t base, uint64_t exp, uint64_t mod);

#endif // CRYPTO_H