#include<string.h>
#include<stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "sm4.h"
// define SM4 function below
#define Nr 32
#define Nk 4
// #define DEBUG
#define get_uint32_be(n,b)                   \
{                                            \
    (n) = ( (uint32_t) (b)[0] << 24 )        \
        | ( (uint32_t) (b)[1] << 16 )        \
        | ( (uint32_t) (b)[2] << 8  )        \
        | ( (uint32_t) (b)[3]   );           \
}

#define put_uint32_be(n,b)                  \
{                                           \
    (b)[0] = (uint8_t) ( (n) >> 24 );       \
    (b)[1] = (uint8_t) ( (n) >> 16 );       \
    (b)[2] = (uint8_t) ( (n) >>  8 );       \
    (b)[3] = (uint8_t) ( (n)       );       \
}

static const uint8_t Sbox[256] = {
0xd6, 0x90, 0xe9, 0xfe, 0xcc, 0xe1, 0x3d, 0xb7, 0x16, 0xb6, 0x14, 0xc2, 0x28, 0xfb, 0x2c, 0x05, 
0x2b, 0x67, 0x9a, 0x76, 0x2a, 0xbe, 0x04, 0xc3, 0xaa, 0x44, 0x13, 0x26, 0x49, 0x86, 0x06, 0x99, 
0x9c, 0x42, 0x50, 0xf4, 0x91, 0xef, 0x98, 0x7a, 0x33, 0x54, 0x0b, 0x43, 0xed, 0xcf, 0xac, 0x62, 
0xe4, 0xb3, 0x1c, 0xa9, 0xc9, 0x08, 0xe8, 0x95, 0x80, 0xdf, 0x94, 0xfa, 0x75, 0x8f, 0x3f, 0xa6, 
0x47, 0x07, 0xa7, 0xfc, 0xf3, 0x73, 0x17, 0xba, 0x83, 0x59, 0x3c, 0x19, 0xe6, 0x85, 0x4f, 0xa8, 
0x68, 0x6b, 0x81, 0xb2, 0x71, 0x64, 0xda, 0x8b, 0xf8, 0xeb, 0x0f, 0x4b, 0x70, 0x56, 0x9d, 0x35, 
0x1e, 0x24, 0x0e, 0x5e, 0x63, 0x58, 0xd1, 0xa2, 0x25, 0x22, 0x7c, 0x3b, 0x01, 0x21, 0x78, 0x87, 
0xd4, 0x00, 0x46, 0x57, 0x9f, 0xd3, 0x27, 0x52, 0x4c, 0x36, 0x02, 0xe7, 0xa0, 0xc4, 0xc8, 0x9e, 
0xea, 0xbf, 0x8a, 0xd2, 0x40, 0xc7, 0x38, 0xb5, 0xa3, 0xf7, 0xf2, 0xce, 0xf9, 0x61, 0x15, 0xa1, 
0xe0, 0xae, 0x5d, 0xa4, 0x9b, 0x34, 0x1a, 0x55, 0xad, 0x93, 0x32, 0x30, 0xf5, 0x8c, 0xb1, 0xe3, 
0x1d, 0xf6, 0xe2, 0x2e, 0x82, 0x66, 0xca, 0x60, 0xc0, 0x29, 0x23, 0xab, 0x0d, 0x53, 0x4e, 0x6f, 
0xd5, 0xdb, 0x37, 0x45, 0xde, 0xfd, 0x8e, 0x2f, 0x03, 0xff, 0x6a, 0x72, 0x6d, 0x6c, 0x5b, 0x51, 
0x8d, 0x1b, 0xaf, 0x92, 0xbb, 0xdd, 0xbc, 0x7f, 0x11, 0xd9, 0x5c, 0x41, 0x1f, 0x10, 0x5a, 0xd8, 
0x0a, 0xc1, 0x31, 0x88, 0xa5, 0xcd, 0x7b, 0xbd, 0x2d, 0x74, 0xd0, 0x12, 0xb8, 0xe5, 0xb4, 0xb0, 
0x89, 0x69, 0x97, 0x4a, 0x0c, 0x96, 0x77, 0x7e, 0x65, 0xb9, 0xf1, 0x09, 0xc5, 0x6e, 0xc6, 0x84, 
0x18, 0xf0, 0x7d, 0xec, 0x3a, 0xdc, 0x4d, 0x20, 0x79, 0xee, 0x5f, 0x3e, 0xd7, 0xcb, 0x39, 0x48
};

/* System parameter */
static const uint32_t FK[4] = {0xa3b1bac6,0x56aa3350,0x677d9197,0xb27022dc};

/* fixed parameter */
static const uint32_t CK[32] =
{
    0x00070e15,0x1c232a31,0x383f464d,0x545b6269,
    0x70777e85,0x8c939aa1,0xa8afb6bd,0xc4cbd2d9,
    0xe0e7eef5,0xfc030a11,0x181f262d,0x343b4249,
    0x50575e65,0x6c737a81,0x888f969d,0xa4abb2b9,
    0xc0c7ced5,0xdce3eaf1,0xf8ff060d,0x141b2229,
    0x30373e45,0x4c535a61,0x686f767d,0x848b9299,
    0xa0a7aeb5,0xbcc3cad1,0xd8dfe6ed,0xf4fb0209,
    0x10171e25,0x2c333a41,0x484f565d,0x646b7279
};


void show_rk(uint8_t* buf, char* name)
{
  printf("%3s: ", name);
  for(int i=0; i<4; i++)
  {
    printf("%02x", buf[3-i]);
  }
  printf("\n");
}

uint32_t circular_left(uint32_t t, int n)
{
    return (t << n) | (t >> (32-n));
}

uint32_t ExpT(uint32_t t)
{
    uint8_t A[4], B[4];
    uint32_t r;
    put_uint32_be(t, A);
    for(int i = 0; i < 4; i++)
    {
        B[i] = Sbox[A[i]];
    }
    get_uint32_be(r, B);
    return r ^ circular_left(r, 13) ^ circular_left(r, 23);
}

uint32_t T(uint32_t t)
{
    uint8_t A[4], B[4];
    uint32_t _B;
    put_uint32_be(t, A);
    for(int i = 0; i < 4; i++)
    {
        B[i] = Sbox[A[i]];
    }
    get_uint32_be(_B, B);
    return _B ^ circular_left(_B, 2) ^ circular_left(_B, 10) ^ circular_left(_B, 18) ^ circular_left(_B, 24);
}

void KeyExpansion(uint32_t* RoundKey, uint8_t* KEY)
{
    uint32_t key[4];
    for(int i = 0; i < 4; i++)
    {
        get_uint32_be(key[i], KEY+4*i)
    }
    uint32_t K[36];
    for(int i = 0; i < 4; i++)
    {
        K[i] = key[i] ^ FK[i];
    }

    for(int i = 0; i < 32; i++)
    {
        K[i+4] = K[i] ^ ExpT(K[i+1] ^ K[i+2] ^ K[i+3] ^ CK[i]);
        RoundKey[i] = K[i+4];
    }
}

// initiallize the sm4_ctx
void SM4_Init(SM4_Context* sm4_ctx, uint8_t* key, uint8_t* iv)
{
    KeyExpansion(sm4_ctx->RoundKey, key);
    memcpy(sm4_ctx->Iv, iv, SM4KeyLen);
}

void Cipher(SM4_Context* sm4_ctx, uint8_t* input)
{
    uint32_t buf[4];
    for(int i = 0; i < 4; i++)
    {
        get_uint32_be(buf[i], input+4*i);
    }
    uint32_t tmp_buf[36];
    for(int i = 0; i < 4; i++)
    {
        tmp_buf[i] = buf[i];
    }
    for(int i = 0; i< 32; i++)
    {
        tmp_buf[i+4] = tmp_buf[i] ^ T( tmp_buf[i+1] ^ tmp_buf[i+2] ^ tmp_buf[i+3] ^ sm4_ctx->RoundKey[i]);
    }
#ifdef DEBUG
    for(int i = 0; i < 32; i++)
    {
        show_rk((uint8_t *)(tmp_buf+i+4), "X");
    }
#endif
    buf[0] = tmp_buf[35];
    buf[1] = tmp_buf[34];
    buf[2] = tmp_buf[33];
    buf[3] = tmp_buf[32];
    for(int i = 0; i < 4; i++)
    {
        put_uint32_be(buf[i], input+4*i);
    }
}

void InvCipher(SM4_Context* sm4_ctx, uint8_t* input)
{
    uint32_t buf[4];
    for(int i = 0; i < 4; i++)
    {
        get_uint32_be(buf[i], input+4*i);
    }
    uint32_t tmp_buf[36];
    for(int i = 0; i < 4; i++)
    {
        tmp_buf[i] = buf[i];
    }
    for(int i = 0; i< 32; i++)
    {
        tmp_buf[i+4] = tmp_buf[i] ^ T( tmp_buf[i+1] ^ tmp_buf[i+2] ^ tmp_buf[i+3] ^ sm4_ctx->RoundKey[31-i]);
    }
    buf[0] = tmp_buf[35];
    buf[1] = tmp_buf[34];
    buf[2] = tmp_buf[33];
    buf[3] = tmp_buf[32];
    for(int i = 0; i < 4; i++)
    {
        put_uint32_be(buf[i], input+4*i);
    }
}

static void XorWithIv(uint8_t * buf, uint8_t* Iv)
{
    uint8_t i;
    for (i = 0; i < IvSize; i++)
    {
        buf[i] ^= Iv[i];
    }
}

void padding(uint8_t *buf, int length)
{
    int pad_len = 16 - (length % 16);
    uint8_t *pad = malloc(16);
    for(int i = 0; i < pad_len; i++)
    {
        pad[i] = pad_len;
    }
    memcpy(buf+length, pad, pad_len);
}

// cbc encryption
void SM4_Encrypt_CBC(SM4_Context* sm4_ctx, uint8_t* msg, int length)
{
    uint8_t* Iv = sm4_ctx-> Iv;
    for(int i = 0; i < length; i+= SM4_BlockLen)
    {
        XorWithIv(msg, Iv);
        Cipher(sm4_ctx, msg);
        Iv = msg;
        msg += SM4_BlockLen;
    }
}

// cbc decryption
void SM4_Decrypt_CBC(SM4_Context* sm4_ctx, uint8_t* cipher, int length)
{
    uint8_t* Iv = sm4_ctx->Iv;
    uint8_t storeNextIv[SM4_BlockLen];
    for(int i = 0; i < length; i+=SM4_BlockLen)
    {
        memcpy(storeNextIv, cipher, SM4_BlockLen);
        InvCipher(sm4_ctx, cipher);
        XorWithIv(cipher, Iv);
        memcpy(Iv, storeNextIv, SM4_BlockLen);
        cipher += SM4_BlockLen;
    }
}

void show(uint8_t* buf, char* name)
{
  printf("%3s: ", name);
  for(int i=0; i<16; i++)
  {
    printf("%02x", buf[i]);
  }
  printf("\n");
}

void get_iv(uint8_t* iv)
{
  int i = 0, j = 0;
  srand((unsigned)time(NULL));
  for(i = 0; i < 4; ++i)
  {
    int tmp = rand();
    for(j = 0; j < 4; ++j)
    {
      (iv)[i * 4 + j] = (tmp)&(0xff);
      tmp >>= 8;
    }
  }
}


int main()
{
    FILE *pfile = NULL;
    pfile = fopen("./input.txt", "rb");
	uint8_t * data;
    uint8_t * original_data;
    int file_length = 0, total_length = 0;
    if (pfile == NULL)
	{
		return 1;
	}
    fseek(pfile, 0, SEEK_END);
	file_length = ftell(pfile);
    if (file_length % 16 != 0 || file_length < 16) 
    {
        perror("输入必须大于316字节，且为16字节的倍数!\n");
    }
    total_length = (file_length + (16 - (file_length % 16)));
	data = (uint8_t *)malloc((total_length+1) * sizeof(char));
    original_data = (uint8_t *)malloc((total_length+1) * sizeof(char));
	rewind(pfile);
	file_length = fread(data, 1, file_length, pfile);
	padding(data, file_length);
	fclose(pfile);
    uint8_t key[] = { 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
    struct SM4_context ctx;
    uint8_t iv[]  = { 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f };
    get_iv(iv);
    show(key, "key");
    show(iv, "iv");
    memcpy((char*)original_data, (char*)data, total_length);
    struct timeval start, end;
    gettimeofday(&start, NULL);
    SM4_Init(&ctx, key, iv);
    SM4_Encrypt_CBC(&ctx, data, total_length);
    gettimeofday(&end, NULL);
    double timeuse = (end.tv_sec-start.tv_sec)*1000000+(end.tv_usec-start.tv_usec); 
    timeuse = timeuse / (double)1e6;
    printf("Encryption speed: %.2f Mps\n", (double)(16*8/1024.0)/timeuse);
    pfile = fopen("./output.txt", "wb");
    fwrite(data, 1, total_length, pfile);
	fclose(pfile);
    SM4_Decrypt_CBC(&ctx, data, total_length);
    int flag = 0;
    for(int i = 0; i < total_length; i++)
    {
        if (data[i] != original_data[i])
        {
            flag = 1;
            break;
        }
    }
    if (flag)
    {
        // for(int i= 0; i < file_length; ++i)
        // {
        //     printf("%c", data[i]);
        // }
        printf("\ndecryption failed!\n");
    }
    else
    {
        printf("decryption succeeded!\n");
    }
    return 0;
}


// int main()
// {
//     uint8_t key[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
//     // uint8_t key[] = { 0x67, 0x45, 0x23, 0x01, 0xef, 0xcd, 0xab, 0x89, 0x98, 0xba, 0xdc, 0xfe, 0x10, 0x32, 0x54, 0x76 };
//     // uint8_t iv[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
//     struct SM4_context ctx;
//     uint8_t in[] = { 0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef, 0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10 };
//     KeyExpansion(ctx.RoundKey, key);
//     for(int i = 0; i < 32; i++)
//     {
//         show_rk((uint8_t *)&(ctx.RoundKey[i]), "rk");
//     }
//     for(int i = 0; i < 1e6; i++)
//         Cipher(&ctx, in);
//     // InvCipher(&ctx, in);
//     show(in, "encryption");
// }

