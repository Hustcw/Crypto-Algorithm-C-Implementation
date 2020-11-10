#include <stdint.h>


#define SM3_BlockLen 64
typedef struct SM3_Context {
    uint32_t state[8];
} SM3_Context;

void SM3_Init(SM3_Context* ctx);
void SM3_Hash(SM3_Context* ctx, uint8_t* buf, int len, uint8_t* output);
void CF(SM3_Context* ctx, uint8_t buf[64]);
