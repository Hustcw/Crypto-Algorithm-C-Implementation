```c
SS1 = circular_left(circular_left(ctx->state[0], 12) + ctx->state[4] + circular_left(T(j), j), 7);
```
- 看文档的写的时候忘记加上circular_left(T(j)), debug了非常久...