void I_FinishUpdate (void) { 
  static  int     lasttic; 
  int             tics, i; 
 
  // page flip 
  outpw(CRTC_INDEX, CRTC_STARTHIGH+(destscreen&0xff00); 
 
  destscreen += 0x4000; 
  if ( (int)destscreen == 0xac000) 
    destscreen = (byte *)0xa0000; 
} 