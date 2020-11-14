#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "sha3.h"
// define SHA3 function below
#define Nr 24
// #define DEBUG
#define TIME

void show_rr(uint8_t* buf, int len)
{
  for(int i=0; i<len; i++)
  {
    printf("%02x", buf[i]);
  }
}

void keccakf(uint64_t st[25])
{
    //temp variables
    uint64_t t1, t2, bc[5];
    
    const int keccak_const1[24] = {
        10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4,
        15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1
    };
    const int keccak_const2[24] = {
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
    for(int r = 0; r < Nr; r++)
    {
        // theta implementation
        for(int i=0; i<5; i++)
        {
            bc[i] = st[i] ^ st[i+5] ^ st[i+10] ^ st[i+15] ^ st[i+20];
        }

        for(int i=0; i < 5; i++)
        {
            t1 = bc[(i+4) % 5] ^ ROTL64(bc[(i+1) % 5], 1);
            for(int j = 0; j < 25; j+=5)
            {
                st[j+i] ^= t1;
            }
        }

        // Rho Pi
        t1 = st[1];
        for (int i = 0; i < 24; i++) {
            t2 = keccak_const1[i];
            bc[0] = st[t2];
            st[t2] = ROTL64(t1, keccak_const2[i]);
            t1 = bc[0];
        }

        //  Chi
        for (int j = 0; j < 25; j += 5) {
            for (int i = 0; i < 5; i++)
                bc[i] = st[j + i];
            for (int i = 0; i < 5; i++)
                st[j + i] ^= (~bc[(i + 1) % 5]) & bc[(i + 2) % 5];
        }

        //  Iota
        st[0] ^= keccakf_rndc[r];
    }
}


void sha3_init(SHA3_Context* ctx, int hashlen)
{
    for(int i = 0; i < 25; i++) {
        ctx->state.q[i] = 0;
    }
    ctx->hashlen = hashlen;
    ctx->pt = 0;
    ctx->r = 200 - hashlen * 2;
}

void sha3_update(SHA3_Context* ctx, const uint8_t* data, int len)
{
    int r = ctx->r;
    for(int i = 0; i < len; i++)
    {
        ctx->state.b[ctx->pt++] ^= data[i];
        if(ctx->pt == r)
        {
            ctx->pt = 0;
            keccakf(ctx->state.q);
        }
    }
}

void sha3_final(uint8_t* hash, SHA3_Context* ctx)
{
    ctx->state.b[ctx->pt] ^= 0x06; // little endiness
    ctx->state.b[(ctx->r)-1] ^= 0x80; // little endiness
    #ifdef DEBUG
    printf("state: ");
    show_rr(ctx->state.b, 200);
    printf("\n");
    #endif
    keccakf(ctx->state.q);
    #ifdef DEBUG
    printf("state: ");
    show_rr(ctx->state.b, 200);
    printf("\n");
    #endif
    for(int i=0; i < ctx->hashlen; i++)
    {
        hash[i] = ctx->state.b[i];
    }
    return;
}

void sha3_data(uint8_t* data, int len, uint8_t* hash, int hashlen)
{
    SHA3_Context ctx;
    sha3_init(&ctx, hashlen);
    sha3_update(&ctx, data, len);
    sha3_final(hash, &ctx);
}

void sha3_file(char* filename, uint8_t* hash)
{
    FILE *pfile = NULL;
    pfile = fopen(filename, "rb");
	uint8_t* data;
    int file_length = 0;
    if (pfile == NULL)
	{
		perror("open failed!");
	}
    fseek(pfile, 0, SEEK_END);
	file_length = ftell(pfile);
    // int real_len = file_length + (32 - file_length % 32);
    data = (uint8_t *)malloc((file_length+1) * sizeof(char));
    rewind(pfile);
	file_length = fread(data, 1, file_length, pfile);
    #ifdef TIME
    struct timeval start, end;
    gettimeofday(&start, NULL);
    #endif
    sha3_data(data, file_length, hash, 32);
    #ifdef TIME
        gettimeofday(&end, NULL);
        double timeuse = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec); 
        timeuse = timeuse / (double)1e6;
        printf("Hash speed: %.2f Mbps\n", (double)(16*8/1024.0)/timeuse);
    #endif
}

int main()
{
    char filename[50] = "input.txt";
    uint8_t hash[32];
    int hashlen = 32;
    sha3_file(filename, hash);
    printf("SHA3: "); 
    show_rr(hash, 32);
    FILE *pfile;
    pfile = fopen("./output.txt", "wb");
    fwrite(hash, 1, 32, pfile);
    fclose(pfile);
}






