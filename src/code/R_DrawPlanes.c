void R_DrawPlanes (void) {
  visplane_t  *pl;
  int         light;
  int         x, stop;
  int         angle;
        
  for (pl = visplanes ; pl < lastvisplane ; pl++) {
    
    if (pl->minx > pl->maxx)
      continue;

    // sky flat
    [...] // Special case where perspective is disabled.
    
    // regular flat
    ds_source = W_CacheLumpNum(firstflat + flattranslation[pl->picnum],PU_STATIC);
    planeheight = abs(pl->height-viewz);
    light = (pl->lightlevel >> LIGHTSEGSHIFT)+extralight;
    planezlight = zlight[light];

    pl->top[pl->maxx+1] = 0xff;
    pl->top[pl->minx-1] = 0xff;
    
    stop = pl->maxx + 1;
    for (x=pl->minx ; x<= stop ; x++)
      R_MakeSpans (x,pl->top[x-1],pl->bottom[x-1] ,
      pl->top[x],pl->bottom[x]);
    
    Z_ChangeTag (ds_source, PU_CACHE);
  }
}