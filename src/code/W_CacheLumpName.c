void* W_CacheLumpNum(int lump, int tag) {
  byte *ptr;
  if ((unsigned)lump >= numlumps)
    I_Error ("W_CacheLumpNum: %i >= numlumps",lump);
        
  if (!lumpcache[lump]) {
    //printf ("cache miss on lump %i\n",lump);
    ptr = Z_Malloc(W_LumpLength (lump), tag, &lumpcache[lump]);
    W_ReadLump(lump, lumpcache[lump]);
  } else {
    //printf ("cache hit on lump %i\n",lump);
    Z_ChangeTag(lumpcache[lump],tag);
  }
    
  return lumpcache[lump];
}