void D_DoomMain (void) {
   FindResponseFile (); // Search doom.wad, doom1.wad...
   IdentifyVersion ();  // shareware or registered?
   
   V_Init ();           // Video system.
   M_LoadDefaults ();   // Load params from default.cfg
   Z_Init ();           // Zone Memory Allocator
   M_Init ();           // Menu
   R_Init ();           // Renderer
   P_Init ();           // gamePlay
   I_Init ();           // Implementation dependant
   D_CheckNetGame ();   // 
   S_Init ();           // Sound 
   HU_Init ();          // HUD
   ST_Init ();          // Status Bar

   D_DoomLoop ();       // never returns
}                                            /* d_main.c */