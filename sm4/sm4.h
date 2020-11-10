#include <stdint.h>

#define SM4_BlockLen 4
#define SM4KeyLen 16
#define KeyExpSize 32
#define IvSize 4


typedef struct SM4_context {
    uint32_t RoundKey[KeyExpSize];
    uint32_t Iv[IvSize]; // using cbc mode
} SM4_Context;

// initiallize the sm4_ctx
void SM4_Init(SM4_Context* sm4_ctx, uint8_t* key, uint8_t* iv);

// cbc encryption
void SM4_Encrypt_CBC(SM4_Context* sm4_ctx, uint8_t* msg, int length);

// cbc decryption
void SM4_Decrypt_CBC(SM4_Context* sm4_ctx, uint8_t* cipher, int length);

void KeyExpansion(uint32_t* RoundKey, uint32_t* Key);

