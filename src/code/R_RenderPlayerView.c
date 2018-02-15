void D_Display (void) {
	void R_RenderPlayerView (player_t *player) {	
		R_SetupFrame (player);
		R_ClearClipSegs ();
		R_ClearDrawSegs ();
		R_ClearPlanes ();
		R_ClearSprites ();
		NetUpdate ();					// check for new console commands
		R_RenderBSPNode (numnodes-1);	// the head node is the last node output	
		NetUpdate ();					// check for new console commands
		R_DrawPlanes ();                // Draw visplanes
		NetUpdate ();					// check for new console commands
		R_DrawMasked ();
		NetUpdate ();					// check for new console commands
	}
}