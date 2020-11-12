#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "sha3.h"
// define SHA3 function below
#define Nr 24

void keccakf(uint64_t st[25])
{
    //temp variables
    int i,j,k;
    uint64_t t, bc[5];
    const int keccakf_piln[24] = {
        10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4,
        15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1
    };
    const int keccakf_rotc[24] = {
        1,  3,  6,  10, 15, 21, 28, 36, 45, 55, 2,  14,
        27, 41, 56, 8,  25, 43, 62, 18, 39, 61, 20, 44
    };
     // constants
    const uint64_t keccakf_rndc[24] = {
        0x0000000000000001, 0x0000000000008082, 0x800000000000808a,
        0x8000000080008000, 0x000000000000808b, 0x0000000080000001,
        0x8000000080008081, 0x8000000000008009, 0x000000000000008a,
        0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
        0x000000008000808b, 0x800000000000008b, 0x8000000000008089,
        0x8000000000008003, 0x8000000000008002, 0x8000000000000080,
        0x000000000000800a, 0x800000008000000a, 0x8000000080008081,
        0x8000000000008080, 0x0000000080000001, 0x8000000080008008
    };
    // round function implementation
    for(int r; r < Nr; r++)
    {
        // theta implementation
        for(int i=0; i<5; i++)
        {
            bc[i] = st[i] ^ st[i+5] ^ st[i+10] ^ st[i+15] ^ st[i+20];
        }

        for(int i=0; i < 5; i++)
        {
            t = bc[(i+4) % 5] ^ ROTL64(bc[(i+1) % 5], 1);
            for(int j = 0; j < 25; j+=5)
            {
                st[j+i] ^= t;
            }
        }

        // Rho Pi
        t = st[1];
        for (i = 0; i < 24; i++) {
            j = keccakf_piln[i];
            bc[0] = st[j];
            st[j] = ROTL64(t, keccakf_rotc[i]);
            t = bc[0];
        }

        //  Chi
        for (j = 0; j < 25; j += 5) {
            for (i = 0; i < 5; i++)
                bc[i] = st[j + i];
            for (i = 0; i < 5; i++)
                st[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
        }

        //  Iota
        st[0] ^= keccakf_rndc[r];
    }
}

void sha3_init(SHA3_Context* ctx, int mdlen)
{
    for(int i = 0; i < 25; i++) {
        ctx->state.q[i] = 0;
    }
    ctx->mdlen = mdlen;
    ctx->pt = 0;
    ctx->r = 200 - mdlen * 2;
}

void sha3_update(SHA3_Context* ctx, const void* data, int len)
{
    return;
}




