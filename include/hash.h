#ifndef HASH_H
#define HASH_H

#include <stdint.h>

/* Hash output size */
#define HASH_HEX_LEN 65

/* Store 4 hash states */
typedef struct {
    uint64_t h1;
    uint64_t h2;
    uint64_t h3;
    uint64_t h4;
} HashState;

/* Initialize hash values */
void hash_init(HashState *hs);

/* Update hash with one character */
void hash_update(HashState *hs, unsigned char ch);

/* Convert hash to hex string */
void hash_finalize(const HashState *hs, char *out);

/* Hash a full string */
void hash_string(const char *input, char *out);

#endif /* HASH_H */
