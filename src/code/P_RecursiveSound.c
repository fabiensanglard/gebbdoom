void P_RecursiveSound (sector_t *sec, int soundblocks)
{
  int      i;
  line_t    *check;
  sector_t  *other;
  
  // wake up all monsters in this sector
  if (sec->validcount == validcount && 
  	  sec->soundtraversed <= soundblocks+1)
    return;    // already flooded

  sec->validcount = validcount;
  sec->soundtraversed = soundblocks+1;
  sec->soundtarget = soundtarget;
  
  for (i=0 ;i<sec->linecount ; i++) {
    check = sec->lines[i];
    if (! (check->flags & ML_TWOSIDED) )  // Portal?
      continue;  // No, this is a wall, skipping.
    P_LineOpening (check); // fixed_t openrange = 0 if door closed.
    if (openrange <= 0)
      continue;    // closed door
    if ( sides[ check->sidenum[0] ].sector == sec)
      other = sides[ check->sidenum[1] ] .sector;
    else
      other = sides[ check->sidenum[0] ].sector;
    if (check->flags & ML_SOUNDBLOCK)
    {
      if (!soundblocks) // Blocking noise?
        P_RecursiveSound (other, 1); 
    }
    else // Flooding to next sector !
      P_RecursiveSound (other, soundblocks);
  }
}