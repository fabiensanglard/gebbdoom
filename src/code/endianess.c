short ShortSwap (short dat) {
    return ((dat&0xff)<<8) | 
           ((dat>>8)&0xff);
}

long LongSwap (long dat){
    return  (((unsigned)dat)>>24) |
            ((dat>>8)&0xff00)     |
            ((dat<<8)&0xff0000)   |
            (dat<<24);
}

#ifdef __BIG_ENDIAN__
    short ShortSwap(short);
    long LongSwap(long);
    #define SHORT(x)        ShortSwap(x)
    #define LONG(x)         LongSwap(x)
#else
    #define SHORT(x)        (x)
    #define LONG(x)         (x)
#endif