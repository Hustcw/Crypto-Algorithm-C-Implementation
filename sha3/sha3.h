#include<stdint.h>

#define KECCAKF_ROUNDS 24
#define ROTL64(x, n) (((x) << (n)) | ((x) >> (64 - (n))))

typedef struct {
    union {
        uint8_t b[200]; // 8-bit
        uint64_t q[25]; // 64-bit
    } state;
    int pt, r, hashlen;
} SHA3_Context;


// compression function for sha-3
void keccakf(uint64_t st[25]);

void sha3_init(SHA3_Context* ctx, int mdlen);
void sha3_update(SHA3_Context* ctx, const uint8_t *data, int len);
void sha3_final(uint8_t *md, SHA3_Context* ctx); // digest to md

void sha3_data(uint8_t *data, int len, uint8_t* md, int mdlen);
