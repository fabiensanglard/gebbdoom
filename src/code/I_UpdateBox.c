void I_UpdateBox (int x, int y, int width, int height) { 
  int     ofs; 
  byte    *source; 
  short   *dest; 
  int     p,x1, x2; 
  int     srcdelta, destdelta; 
  int     wwide; 
 
  x1 = x>>3; 
  x2 = (x+width)>>3; 
  wwide = x2-x1+1; 
 
  ofs = y*SCREENWIDTH+(x1<<3); 
  srcdelta = SCREENWIDTH - (wwide<<3); 
  destdelta = PLANEWIDTH/2 - wwide; 
  outp (SC_INDEX, SC_MAPMASK); 
 
  for (p = 0 ; p < 4 ; p++) {      // For each VGA bank
    outp (SC_INDEX+1, 1<<p);       // Select bank
    source = screens[0] + ofs + p; // Read from FB #0
    dest = (short *)(destscreen + (ofs>>2)); 
    for (y=0 ; y<height ; y++) {  
      for (x=wwide ; x ; x--) { 
        *dest++ = *source + (source[4]<<8); 
        source += 8; 
      } 
      source+=srcdelta; 
      dest+=destdelta; 
    } 
  } 
} 