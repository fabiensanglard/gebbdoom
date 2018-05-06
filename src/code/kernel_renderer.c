// doomdef.h
#define SCREENWIDTH     320
#define SCREENHEIGHT    200
extern byte *screens[5];

// v_video.c
byte* base = I_AllocLow (SCREENWIDTH*SCREENHEIGHT*4);
for (i=0 ; i<4 ; i++)
    screens[i] = base + i*SCREENWIDTH*SCREENHEIGHT;

// st_stuff.c
screens[4] = (byte *) Z_Malloc(ST_WIDTH*ST_HEIGHT, PU_STATIC, 0);

