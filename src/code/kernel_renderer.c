// Four framebuffers + one status bar buffer.
// doomdef.h
extern byte *screens[5];

// v_video.c
byte* base = I_AllocLow (SCREENWIDTH*SCREENHEIGHT*4);
for (i=0 ; i<4 ; i++)
    screens[i] = base + i*SCREENWIDTH*SCREENHEIGHT;

screens[4] = (byte *) Z_Malloc(ST_WIDTH*ST_HEIGHT, PU_STATIC, 0);

// Dirty box used to keep track of updates.
enum {BOXTOP,BOXBOTTOM,BOXLEFT,BOXRIGHT};
extern int dirtybox[4];
// TODO: Find framebuffer declaration