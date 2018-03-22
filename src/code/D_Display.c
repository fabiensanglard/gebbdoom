void D_Display (void) {
  R_ExecuteSetViewSize ();

  // do 2D drawing
  switch (gamestate) {
    case GS_LEVEL:ST_Drawer (); break;
    case GS_INTERMISSION: WI_Drawer (); break;
    case GS_FINALE: F_Drawer (); break;
    case GS_DEMOSCREEN: D_PageDrawer (); break; 
  }

  // draw buffered stuff to screen
  I_UpdateNoBlit ();

  // draw 3D view
  if (gamestate != oldgamestate && gamestate != GS_LEVEL)
    R_RenderPlayerView (&players[displayplayer]);

  HU_Drawer ();

  I_SetPalette (W_CacheLumpName ("PLAYPAL",PU_CACHE));

  R_FillBackScreen (); // Background when not full screen

  M_Drawer ();  // menu is drawn even on top of everything
  NetUpdate (); // send out any new accumulation

  I_FinishUpdate ();  // page flip or blit buffer
}