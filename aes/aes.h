#ifndef _AES_H_
#define _AES_H_

#include <stdint.h>

// to define aes mode macros for enabling and disabling AES encryption CBC mode
#define AES_BLOCKLEN 16
#ifndef CBC
    #define CBC 1
#endif

#ifndef ECB
    #define ECB 1
#endif

#define AES128 1
#define AES_KEYLEN 16
#define AES_keyExpSize 176

struct AES_ctx {
    uint8_t RoundKey[AES_keyExpSize];
#if (defined(CBC) && (CBC==1))
    uint8_t Iv[AES_BLOCKLEN];
#endif
};

// if cbc mode is used; set iv and initialize iv and key
void AES_init_ctx(struct AES_ctx* ctx, const uint8_t* key);
#if (defined(CBC) && (CBC==1))
void AES_init_ctx_iv(struct AES_ctx* ctx, const uint8_t* key, const uint8_t* iv);
void AES_ctx_set_iv(struct AES_ctx* ctx, const uint8_t* iv);
#endif

#if defined(ECB) && (ECB == 1)
void AES_ECB_encrypt(const struct AES_ctx* ctx, uint8_t* msg);
void AES_ECB_decrypt(const struct AES_ctx* ctx, uint8_t* cipher);
#endif


#if defined(CBC) && (CBC==1)
void AES_CBC_encrypt(struct AES_ctx* ctx, uint8_t* msg, uint32_t length);
void AES_CBC_decrypt(struct AES_ctx* ctx, uint8_t* cipher, uint32_t length);
#endif

#endif
