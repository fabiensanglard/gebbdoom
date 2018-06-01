void ST_doPaletteStuff(void) {
  int palette;
  int cnt = plyr->damagecount;
  ...
  if (cnt) {
    palette = (cnt+7)>>3;
    if (palette >= NUMREDPALS) palette = NUMREDPALS-1;
    palette += STARTREDPALS;
  } else {
  	...
  }
  byte *pal = W_CacheLumpNum (lu_palette, PU_CACHE)+palette*768;
  I_SetPalette (pal);
  ...
}