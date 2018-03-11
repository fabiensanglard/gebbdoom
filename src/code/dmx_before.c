void SFX_SetOrigin(
  SFX_HANDLE  handle,  // Handle received from play
  int         x,       // Left-Right Positioning
  int         volume   // Volume Level 1..127
);

int SFX_PlayPatch(
  void    *patch,    // Patch to play
  int      x,        // Left-Right Positioning
  int      pitch,    // 0..128..255  -1Oct..C..+1Oct  
  int      volume,   // Volume Level 1..127
  int      priority  // Priority, 0=Lowest
);