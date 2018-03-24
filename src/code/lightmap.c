lighttable_t*	ds_colormap;


void DrawRoutine {
  // Select lightmap.
  int lightMapId = X % 16;	
  ds_colormap = palette + lightMapId * 256;
  
  // Draw using lightmap indirection.
  *dest++ = ds_colormap[ds_source[spot]];
}
