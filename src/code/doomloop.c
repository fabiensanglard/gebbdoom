void D_DoomLoop (void) {
	I_InitGraphics ();
	while (1) {                                                               
		I_StartFrame ();   // frame synchronous IO operations.
		TryRunTics ();     // Simulate based on I/O and A.I.
		S_UpdateSounds (); // Generate audio.
		D_Display ();      // Generate video.
	}
}