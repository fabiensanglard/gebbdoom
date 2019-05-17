int W_CheckNumForName (char *name) {
  union {
      char s[9];
      int  x[2];
  } name8;
  int         v1,v2;
  lumpinfo_t  *lump_p;

  // make the name into two integers for easy comparison
  strncpy (name8.s,name,8);
  name8.s[8] = 0;    // in case the name was a full 8 chars
  strupr (name8.s);  // case insensitive

  v1 = name8.x[0];
  v2 = name8.x[1];

  // scan backwards so patch lump files take precedence
  lump_p = lumpinfo + numlumps;

  while (lump_p-- != lumpinfo)
    if (*(int *) lump_p->name    == v1 && 
        *(int *)&lump_p->name[4] == v2)
        return lump_p - lumpinfo;
  return -1;
}