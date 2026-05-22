#include "hash.h"
#include <stdio.h>
#include <string.h>

/* Starting values for hash states */
#define SEED1  5381ULL
#define SEED2  7919ULL
#define SEED3  1237ULL
#define SEED4  99991ULL

/* Initialize all hash values */
void hash_init(HashState *hs)
{
    hs->h1 = SEED1;
    hs->h2 = SEED2;
    hs->h3 = SEED3;
    hs->h4 = SEED4;
}

/* Update hash using one character */
void hash_update(HashState *hs, unsigned char ch)
{
    hs->h1 = ((hs->h1 << 5)  + hs->h1) ^ (uint64_t)ch;
    hs->h2 = ((hs->h2 << 7)  + hs->h2) ^ (uint64_t)ch;
    hs->h3 = ((hs->h3 << 3)  + hs->h3) ^ (uint64_t)ch;
    hs->h4 = ((hs->h4 << 11) + hs->h4) ^ (uint64_t)ch;
}

/* Convert hash values into hex string */
void hash_finalize(const HashState *hs, char *out)
{
    snprintf(out, HASH_HEX_LEN,
             "%016llx%016llx%016llx%016llx",
             (unsigned long long)hs->h1,
             (unsigned long long)hs->h2,
             (unsigned long long)hs->h3,
             (unsigned long long)hs->h4);
}

/* Full hashing process */
void hash_string(const char *input, char *out)
{
    HashState hs;

    /* Start hash */
    hash_init(&hs);

    /* Process every character */
    for (size_t i = 0; input[i] != '\0'; i++) {
        hash_update(&hs, (unsigned char)input[i]);
    }

    /* Generate final hash string */
    hash_finalize(&hs, out);
}
