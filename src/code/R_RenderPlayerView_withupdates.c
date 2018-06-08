void R_RenderPlayerView (player_t *player) {	
	[...]
	NetUpdate ();					// check for new console commands
	R_RenderBSPNode (numnodes-1);	
	NetUpdate ();					// check for new console commands
	R_DrawPlanes ();
	NetUpdate ();					// check for new console commands
	R_DrawMasked ();
	NetUpdate ();					// check for new console commands
}