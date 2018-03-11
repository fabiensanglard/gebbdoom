#define    STARTBONUSPALS  9
#define    NUMBONUSPALS    4

void ST_doPaletteStuff(void) {
  int palette;
  int cnt = plyr->damagecount;
  ...
  else if (plyr->bonuscount) {
    palette = (plyr->bonuscount+7)>>3;
    if (palette >= NUMBONUSPALS) palette = NUMBONUSPALS-1;
    palette += STARTBONUSPALS;
  }
  	...
  byte *pal = W_CacheLumpNum (lu_palette, PU_CACHE)+palette*768;
  I_SetPalette (pal);
  ...
}