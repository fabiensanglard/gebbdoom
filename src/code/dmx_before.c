int SFX_PlayPatch(
  void    *patch,    // Patch to play
  int      x,        // Left-Right Positioning
  int      pitch,    // 0..128..255  -1Oct..C..+1Oct  
  int      volume,   // Volume Level 1..127
  int      priority  // Priority, 0=Lowest
);