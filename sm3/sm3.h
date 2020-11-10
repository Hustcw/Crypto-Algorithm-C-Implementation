#include <stdint.h>

typedef struct SM3_Context SM3_Context
{
    uint32_t state[8];
} SM3_Context;

void SM3_Init(SM3_Context* ctx)
{
    
}