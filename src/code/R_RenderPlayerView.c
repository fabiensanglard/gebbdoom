void R_RenderPlayerView (player_t *player) {  
  R_RenderBSPNode (numnodes-1); // root node is last  
  R_DrawPlanes ();              // Draw visplanes
  R_DrawMasked ();    
}
