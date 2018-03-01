#ifdef __BIG_ENDIAN__
long LongSwap(long);
#define LONG(x)         LongSwap(x)
#else
#define LONG(x)         (x)
#endif
