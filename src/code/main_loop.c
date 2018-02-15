void main (int argc, char **argv) {
   void D_DoomMain (void} {
      FindResponseFile ();
      IdentifyVersion ();   // WTF IS THIS?
      
      V_Init ();
      M_LoadDefaults ();  
      Z_Init ();
      M_Init ();
      R_Init ();
      P_Init ();
      I_Init ();
      D_CheckNetGame ();
      S_Init (sfxVolume*8,musicVolume*8);
      HU_Init ();   // WTF Headup display?
      ST_Init ();

      D_DoomLoop ();  // never returns
   }
}