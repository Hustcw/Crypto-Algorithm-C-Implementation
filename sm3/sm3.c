#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>

#include "sm3.h"

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

uint32_t circular_left(uint32_t t, int n)
{
    return (t << n) | (t >> (32-n));
}
