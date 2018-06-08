void G_Ticker (void) {
    [...]
    switch (gamestate) { 
        case GS_LEVEL: 
            P_Ticker ();  // Update actors
            ST_Ticker (); // Status Bar
            AM_Ticker (); // Auto Map 
            HU_Ticker (); // HUD            
            break; 
    [...] 
}
