void R_DrawMasked (void) {
  vissprite_t  *spr;
  drawseg_t   *ds;
  
  R_SortVisSprites ();

  if (vissprite_p > vissprites) {
    // draw all vissprites back to front
    for (spr = vsprsortedhead.next; spr != &vsprsortedhead; spr=spr->next)
      R_DrawSprite (spr);
  }

  // render any remaining masked mid textures
  for (ds=ds_p-1 ; ds >= drawsegs ; ds--)
    if (ds->maskedtexturecol)
      R_RenderMaskedSegRange (ds, ds->x1, ds->x2);


  // draw the psprites on top of everything
  if (!viewangleoffset)    // don't draw on side views
    R_DrawPlayerSprites ();
}