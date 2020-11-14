#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "sm3.h"
// #define DEBUG
#define TIME
#define get_uint32_be(n,b)                   \
{                                            \
    (n) = ( (uint32_t) (b)[0] << 24 )        \
        | ( (uint32_t) (b)[1] << 16 )        \
        | ( (uint32_t) (b)[2] << 8  )        \
        | ( (uint32_t) (b)[3]   );           \
}

#define put_uint32_be(n, b)                  \
{                                           \
    (b)[0] = (uint8_t) ( (n) >> 24 );       \
    (b)[1] = (uint8_t) ( (n) >> 16 );       \
    (b)[2] = (uint8_t) ( (n) >>  8 );       \
    (b)[3] = (uint8_t) ( (n)       );       \
}

void show(uint8_t* buf, char* name)
{
  printf("%3s: ", name);
  for(int i=0; i<4; i++)
  {
    printf("%02x", buf[3-i]);
  }
}

void show_r(uint8_t* buf)
{
  for(int i=0; i<4; i++)
  {
    printf("%02x", buf[3-i]);
  }
}

void show_rr(uint8_t* buf)
{
  for(int i=0; i<4; i++)
  {
    printf("%02x", buf[i]);
  }
}



uint32_t T(int j) {
    return (j <= 15 ? 0x79cc4519 : 0x7a879d8a);
}

uint32_t circular_left(uint32_t t, int n)
{
    return (t << n) | (t >> (32-n));
}


uint32_t P0(uint32_t X)
{
    return X ^ circular_left(X, 9) ^ circular_left(X, 17);
}

uint32_t P1(uint32_t X)
{
    return X ^ circular_left(X, 15) ^ circular_left(X, 23);
}

uint32_t FF(uint32_t X, uint32_t Y, uint32_t Z, int j)
{
    if (j <= 15) 
        return (X ^ Y ^ Z);
    else
        return (X & Y) | (X & Z) | (Y & Z);
}

uint32_t GG(uint32_t X, uint32_t Y, uint32_t Z, int j)
{
    if (j <= 15) 
        return (X ^ Y ^ Z);
    else
        return (X & Y) | (~X & Z);
}

void SM3_Init(SM3_Context* ctx)
{
    ctx->state[0] = 0x7380166F;
    ctx->state[1] = 0x4914B2B9;
    ctx->state[2] = 0x172442D7;
    ctx->state[3] = 0xDA8A0600;
    ctx->state[4] = 0xA96F30BC;
    ctx->state[5] = 0x163138AA;
    ctx->state[6] = 0xE38DEE4D;
    ctx->state[7] = 0xB0FB0E4E;
}

void Padding(uint8_t* buf, uint64_t len)
{
    int reminder = 56 - ((len % 64) % 56);
    uint64_t pad_len = len * 8;
    uint8_t *pad = malloc(reminder);
    pad[0] = 0x80;
    for(int i = 1; i < reminder; i++)
    {
        pad[i] = 0;
    }
    uint32_t *high, *low;
    low = (uint32_t *)&pad_len;
    high = ((uint32_t *)(&pad_len)) + 1;
    #ifdef DEBUG
    printf("LOW: ");
    show_rr((uint8_t *)low);
    printf(" LOW_3: %02x", ((uint8_t)(*low) >> 24));
    printf(" LOW_2: %02x", ((uint8_t)(*low) >> 16));
    printf(" LOW_1: %02x", ((uint8_t)(*low) >> 8));
    printf(" LOW_0: %02x", ((uint8_t)(*low)     ));
    printf(" HIGH: ");
    show_rr((uint8_t *)high);
    printf("\n");
    #endif
    memcpy(buf+len, pad, reminder);
    put_uint32_be(*high, buf + len + reminder);
    put_uint32_be(*low, buf + len + reminder + 4);
}

void CF(SM3_Context* ctx, uint8_t buf[64]) 
{
    uint32_t _W[64], W[68];
    uint32_t store_r[8];
    for(int i = 0; i < 8; i++)
    {
        store_r[i] = ctx->state[i];
    }
    // store in big endian
    for(int i = 0; i < 16; i++)
    {
        get_uint32_be(W[i], buf + i * 4);
    }
    for(int j = 16; j < 68; j++)
    {
        W[j] = P1(W[j-16] ^ W[j-9] ^ circular_left(W[j-3], 15)) ^ circular_left(W[j-13], 7) ^ W[j-6];
    }
    for(int j=0; j < 64; j++)
    {
        _W[j] = W[j] ^ W[j+4];
    }
    #ifdef DEBUG
        printf("SHOW W:\n");
        for(int i=0; i < 8; i++)
        {
            for(int j=0; j < 8; j++)
            {
                show_r((uint8_t*)&W[i*8+j]); printf(" ");
            }
            printf("\n");
        }
        printf("\n");

        printf("SHOW W':\n");
        for(int i=0; i < 8; i++)
        {
            for(int j=0; j < 8; j++)
            {
                show_r((uint8_t*)&_W[i*8+j]); printf(" ");
            }
            printf("\n");
        }
        printf("\n");
    #endif
    // A 0 B 1 C 2 D 3 E 4 F 5 G 6 H 7
    uint32_t SS1, SS2, TT1, TT2;
    for(int j = 0; j < 64; ++j)
    {  
        #ifdef DEBUG
            show((uint8_t*)&ctx->state[0], "A");
            show((uint8_t*)&ctx->state[1], "B");
            show((uint8_t*)&ctx->state[2], "C");
            show((uint8_t*)&ctx->state[3], "D");
            show((uint8_t*)&ctx->state[4], "E");
            show((uint8_t*)&ctx->state[5], "F");
            show((uint8_t*)&ctx->state[6], "G");
            show((uint8_t*)&ctx->state[7], "H");
            printf("\n");
        #endif
        SS1 = circular_left(circular_left(ctx->state[0], 12) + ctx->state[4] + circular_left(T(j), j), 7);
        SS2 = SS1 ^ circular_left(ctx->state[0], 12);
        TT1 = FF(ctx->state[0], ctx->state[1], ctx->state[2], j) + ctx->state[3] + SS2 + _W[j];
        TT2 = GG(ctx->state[4], ctx->state[5], ctx->state[6], j) + ctx->state[7] + SS1 + W[j];
        ctx->state[3] = ctx->state[2];
        ctx->state[2] = circular_left(ctx->state[1], 9);
        ctx->state[1] = ctx->state[0];
        ctx->state[0] = TT1;
        ctx->state[7] = ctx->state[6];
        ctx->state[6] = circular_left(ctx->state[5], 19);
        ctx->state[5] = ctx->state[4];
        ctx->state[4] = P0(TT2);
    }
    for(int i = 0; i < 8; i++)
    {
        ctx->state[i] ^= store_r[i];
    }
}

void SM3_Hash(SM3_Context* ctx, uint8_t* buf, int len, uint8_t* output)
{
    int total_len = len + (64 - (len % 64)); // msg + pad + uint64_t_len
    int round = total_len / 64;
    SM3_Init(ctx);
    Padding(buf, len);
    #ifdef DEBUG
    printf("\nFINAL: ");
    show_rr(buf+60);
    printf("\n");
    printf("SHOW PADDING:\n");
    for(int i=0; i < 2; i++)
    {
        for(int j=0; j < 8; j++)
        {
            show_rr(&buf[(i*8+j)*4]); printf(" ");
        }
        printf("\n");
    }
    printf("\n");
    #endif
    for(int i = 0; i < round; i++)
    {
        CF(ctx, buf);
        buf += SM3_BlockLen;
    }
    for(int j = 0; j < 8; j++)
    {
        put_uint32_be(ctx->state[j], output+j*4);
    }
}

void SM3_File(SM3_Context* ctx, char* filename, uint8_t* output)
{
    FILE *pfile = NULL;
    pfile = fopen(filename, "rb");
	uint8_t * data;
    int file_length = 0, total_length = 0;
    if (pfile == NULL)
	{
		perror("open failed!");
	}
    fseek(pfile, 0, SEEK_END);
	file_length = ftell(pfile);
    total_length = (file_length + (64 - (file_length % 64)));
    data = (uint8_t *)malloc((total_length+1) * sizeof(char));
    rewind(pfile);
	file_length = fread(data, 1, file_length, pfile);
    #ifdef DEBUG
        printf("total_length: %d\n", total_length);
        printf("SHOW MESSAGES:\n");
        for(int i=0; i < 2; i++)
        {
            for(int j=0; j < 8; j++)
            {
                show_rr(&data[(i*8+j)*4]); printf(" ");
            }
            printf("\n");
        }
        printf("\n");
    #endif
    #ifdef TIME
        struct timeval start, end;
        gettimeofday(&start, NULL);
    #endif
    SM3_Hash(ctx, data, file_length, output);
    #ifdef TIME
        gettimeofday(&end, NULL);
        double timeuse = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec); 
        timeuse = timeuse / (double)1e6;
        printf("Hash speed: %.2f Mbps\n", (double)(16*8)/timeuse);
    #endif
}

int main()
{
    char filename[50] = "input.txt";
    SM3_Context ctx;
    uint8_t hash[32];
    SM3_File(&ctx, filename, hash);
    printf("SM3: "); 
    for(int i = 0; i < 8; i++) 
    {
        show_rr(hash + 4*i); printf(" ");
    }
    FILE *pfile;
    pfile = fopen("./output.txt", "wb");
    fwrite(hash, 1, 32, pfile);
    fclose(pfile);
}






