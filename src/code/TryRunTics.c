void TryRunTics (void) {
    int         availabletics;
    int         entertic;
    static int  oldentertics;

    // decide how many tics to run.
    entertic = I_GetTime ();
    realtics = entertic - oldentertics;
    int counts = realtics;    
    
    // Run as many tics as necessary.
    while (counts--){
        M_Ticker ();
        G_Ticker ();
        gametic++;
        NetUpdate ();    
    }
}