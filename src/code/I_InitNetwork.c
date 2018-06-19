doomcom_t doomcom;

void I_InitNetwork (void)  { 
  int    i; 
 
  i = M_CheckParm ("-net"); 
  if (!i) { // single player game
    doomcom = malloc (sizeof (*doomcom) ); 
    memset (doomcom, 0, sizeof(*doomcom) ); 
    netgame = false; 
    doomcom->id = DOOMCOM_ID; 
    doomcom->numplayers = doomcom->numnodes = 1; 
    doomcom->deathmatch = false; 
    doomcom->consoleplayer = 0; 
    doomcom->ticdup = 1; 
    doomcom->extratics = 0; 
    return; 
  } 
 
  netgame = true;  // multiplayer game
  doomcom = (doomcom_t *)atoi(myargv[i+1]);
}   