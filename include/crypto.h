// By defining the global public parameters (DH_PRIME and DH_GENERATOR) and the function prototype for modular_exponentiation inside crypto.h,
//  you allow any other file (like src/nittalk.c) to use these cryptographic parameters simply by adding #include "crypto.h". The other files don't need to know how the math works; they just need to know it exists.

// How does the compiler know:

// what the function returns?
// how many parameters it takes?
// That's why we put function declarations (prototypes) in header files.

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

uint32_t generate_private_key(void);

uint32_t generate_public_key(uint32_t private_key);

uint32_t generate_shared_secret(
    uint32_t received_public_key,
    uint32_t private_key);

#endif // CRYPTO_H