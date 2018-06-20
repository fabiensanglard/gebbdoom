typedef struct {
    spritenum_t  sprite;
    long         frame;
    long         tics;
    void         (*action) ();
    statenum_t   nextstate;
    long         misc1, misc2;
} state_t;

state_t    states[NUMSTATES] = {     // = NUMSTATES = 1109
// [...]
{SPR_TROO,0,10,A_Look,S_TROO_STND2,0,0} // S_TROO_STND
{SPR_TROO,1,10,A_Look,S_TROO_STND,0,0}, // S_TROO_STND2

{SPR_TROO,0,3,A_Chase,S_TROO_RUN2,0,0}, // S_TROO_RUN1
{SPR_TROO,0,3,A_Chase,S_TROO_RUN3,0,0}, // S_TROO_RUN2
{SPR_TROO,1,3,A_Chase,S_TROO_RUN4,0,0}, // S_TROO_RUN3
{SPR_TROO,1,3,A_Chase,S_TROO_RUN5,0,0}, // S_TROO_RUN4
{SPR_TROO,2,3,A_Chase,S_TROO_RUN6,0,0}, // S_TROO_RUN5
{SPR_TROO,2,3,A_Chase,S_TROO_RUN7,0,0}, // S_TROO_RUN6
{SPR_TROO,3,3,A_Chase,S_TROO_RUN8,0,0}, // S_TROO_RUN7
{SPR_TROO,3,3,A_Chase,S_TROO_RUN1,0,0}, // S_TROO_RUN8

{SPR_TROO,4,8,A_FaceTarget,S_TROO_ATK2,0,0},// S_TROO_ATK1
{SPR_TROO,5,8,A_FaceTarget,S_TROO_ATK3,0,0},// S_TROO_ATK2
{SPR_TROO,6,6,A_TroopAttack,S_TROO_RUN1,0,0},// S_TROO_ATK3
// [...]
}