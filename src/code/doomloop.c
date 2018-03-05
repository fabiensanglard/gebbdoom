void D_DoomLoop (void) {
	I_InitGraphics ();
	while (1) {                                                               
		I_StartFrame ();   // frame syncronous IO operations
		TryRunTics ();   // will run at least one tic
		S_UpdateSounds (players[consoleplayer].mo);// move positional sounds
		D_Display ();
	}
}