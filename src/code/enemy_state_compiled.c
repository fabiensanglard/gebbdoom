typedef struct {
    spritenum_t  sprite;
    long         frame;
    long         tics;
    void         (*action) ();
    statenum_t   nextstate;
    long         misc1, misc2;
} state_t;
state_t    states[NUMSTATES] = {     // = NUMSTATES = 1109
    {SPR_TROO,8,8,NULL,S_TROO_DIE2,0,0},    // S_TROO_DIE1
    {SPR_TROO,9,8,A_Scream,S_TROO_DIE3,0,0},// S_TROO_DIE2
    {SPR_TROO,10,6,NULL,S_TROO_DIE4,0,0},   // S_TROO_DIE3
    {SPR_TROO,11,6,A_Fall,S_TROO_DIE5,0,0}, // S_TROO_DIE4
    {SPR_TROO,12,-1,NULL,S_NULL,0,0},       // S_TROO_DIE5
}