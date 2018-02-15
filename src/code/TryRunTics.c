void TryRunTics (void) {
	NetUpdate ();

    // decide how many tics to run
	counts = realtics+1;	
	frameon++;

	while (counts--){
		M_Ticker ();
		G_Ticker ();
		gametic++;
		NetUpdate ();	
	}
}