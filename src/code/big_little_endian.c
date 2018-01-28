#ifdef __BIG_ENDIAN__
short ShortSwap(short);
long LongSwap(long);
#define LONG(x)         LongSwap(x)
#else
#define LONG(x)         (x)
#endif
