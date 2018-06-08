typedef void (*think_t) ();

typedef struct thinker_s {
    struct thinker_s  *prev, *next;
    think_t           function;
} thinker_t;